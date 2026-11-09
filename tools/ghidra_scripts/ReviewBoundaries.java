// Plan/apply reviewed semantic units. Does not auto-analyze or modify memory bytes.
// @category CoolBoarders2
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.util.StringPropertyMap;
import com.google.gson.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.HexFormat;
import java.util.LinkedHashMap;
import java.util.Map;

public class ReviewBoundaries extends GhidraScript {
    private static final String PROPERTY = "CB2ReviewedBoundaryUnits";
    private String hash(byte[] bytes) throws Exception {
        return HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(bytes));
    }
    private long number(String value) { return Long.decode(value); }
    private AddressSet body(JsonObject region) {
        return new AddressSet(toAddr(number(region.get("address").getAsString())),
            toAddr(number(region.get("end_exclusive").getAsString()) - 1));
    }
    private boolean exactBody(Function function, JsonObject region) {
        AddressSet expected = body(region);
        return function != null && function.getBody().getNumAddresses() == expected.getNumAddresses()
            && function.getBody().contains(expected.getMinAddress(), expected.getMaxAddress());
    }
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 3 || !(args[2].equals("plan") || args[2].equals("apply"))) {
            throw new IllegalArgumentException("ReviewBoundaries.java input.json output-directory plan|apply");
        }
        JsonObject catalog = JsonParser.parseString(Files.readString(Path.of(args[0]))).getAsJsonObject();
        if (catalog.get("schema_version").getAsInt() != 1) throw new IllegalStateException("Unsupported review schema");
        Path directory = Path.of(args[1]);
        Files.createDirectories(directory);
        byte[] original = Files.readAllBytes(Path.of("/inputs/files/SCUS_943.58"));
        if (!hash(original).equals(catalog.get("reference_sha256").getAsString())) {
            throw new IllegalStateException("Review catalog/reference fingerprint mismatch");
        }
        byte[] payload = new byte[original.length - 0x800];
        if (currentProgram.getMemory().getBytes(toAddr(0x80010000L), payload) != payload.length
                || !hash(payload).equals(hash(java.util.Arrays.copyOfRange(original, 0x800, original.length)))) {
            throw new IllegalStateException("Saved Ghidra program differs from pinned EXE payload");
        }
        Map<Long, JsonObject> standalone = new LinkedHashMap<>();
        JsonObject regions = catalog.getAsJsonObject("regions");
        for (Map.Entry<String, JsonElement> item : regions.entrySet()) {
            JsonObject region = item.getValue().getAsJsonObject();
            long start = number(region.get("address").getAsString());
            int size = region.get("size").getAsInt();
            if (!region.get("reviewed").getAsBoolean() || region.getAsJsonArray("evidence").size() == 0
                    || number(item.getKey()) != start || size <= 0
                    || number(region.get("end_exclusive").getAsString()) != start + size) {
                throw new IllegalStateException("Invalid reviewed region extent: " + item.getKey());
            }
            byte[] bytes = new byte[size];
            String key = region.has("instruction_sha256") ? "instruction_sha256" : "bytes_sha256";
            if (currentProgram.getMemory().getBytes(toAddr(start), bytes) != size
                    || !hash(bytes).equals(region.get(key).getAsString())) {
                throw new IllegalStateException("Stale reviewed region bytes: " + item.getKey());
            }
            if (region.get("kind").getAsString().equals("standalone")) standalone.put(start, region);
        }
        JsonArray actions = new JsonArray();
        int conflicts = 0;
        for (Map.Entry<Long, JsonObject> item : standalone.entrySet()) {
            Function existing = getFunctionAt(toAddr(item.getKey()));
            if (exactBody(existing, item.getValue())) continue;
            JsonObject action = new JsonObject();
            action.addProperty("entry", item.getValue().get("address").getAsString());
            action.addProperty("action", existing == null ? "create_function" : "adjust_function_body");
            JsonArray overlapping = new JsonArray();
            FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
            while (functions.hasNext()) {
                Function other = functions.next();
                long address = other.getEntryPoint().getOffset();
                if (address == item.getKey() || !other.getBody().intersects(body(item.getValue()))) continue;
                // Another reviewed function may currently have an overlong body;
                // only its explicitly reviewed replacement extent can justify trimming.
                if (standalone.containsKey(address) && !body(standalone.get(address)).intersects(body(item.getValue()))) continue;
                overlapping.add(other.getEntryPoint().toString());
            }
            if (overlapping.size() != 0) conflicts++;
            action.add("unreviewed_overlap_conflicts", overlapping);
            actions.add(action);
        }
        Gson gson = new GsonBuilder().setPrettyPrinting().create();
        JsonObject report = new JsonObject();
        report.addProperty("reference_sha256", catalog.get("reference_sha256").getAsString());
        report.addProperty("mode", args[2]);
        report.addProperty("reviewed_regions", regions.size());
        report.addProperty("standalone_regions", standalone.size());
        report.addProperty("conflicts", conflicts);
        report.add("function_actions", actions);
        Files.writeString(directory.resolve("review-plan.json"), gson.toJson(report) + "\n");
        if (args[2].equals("plan")) { println(gson.toJson(report)); return; }
        if (conflicts != 0) throw new IllegalStateException("Unreviewed overlaps; no changes applied");
        int transaction = currentProgram.startTransaction("Apply reviewed CB2 boundary units");
        boolean success = false;
        try {
            // First trim reviewed existing functions, then create missing functions.
            for (Map.Entry<Long, JsonObject> item : standalone.entrySet()) {
                Function existing = getFunctionAt(toAddr(item.getKey()));
                if (existing != null && !exactBody(existing, item.getValue())) existing.setBody(body(item.getValue()));
            }
            for (Map.Entry<Long, JsonObject> item : standalone.entrySet()) {
                Address start = toAddr(item.getKey());
                if (getFunctionAt(start) != null) continue;
                if (getInstructionAt(start) == null) {
                    DisassembleCommand command = new DisassembleCommand(start, body(item.getValue()), true);
                    if (!command.applyTo(currentProgram, monitor)) throw new IllegalStateException("Disassembly failed: " + start);
                }
                currentProgram.getFunctionManager().createFunction(String.format("func_%08X", item.getKey()),
                    start, body(item.getValue()), SourceType.USER_DEFINED);
            }
            // Group records preserve multiple entry labels instead of inventing
            // overlapping standalone functions. Special-ABI regions stay explicit.
            currentProgram.getUsrPropertyManager().removePropertyMap(PROPERTY);
            StringPropertyMap map = currentProgram.getUsrPropertyManager().createStringPropertyMap(PROPERTY);
            JsonObject metadata = catalog.deepCopy();
            metadata.remove("regions");
            map.add(toAddr(0x80010000L), gson.toJson(metadata));
            for (Map.Entry<String, JsonElement> item : regions.entrySet()) {
                Address start = toAddr(number(item.getKey()));
                JsonObject region = item.getValue().getAsJsonObject();
                map.add(start, gson.toJson(region));
                currentProgram.getBookmarkManager().setBookmark(start, "Analysis", "CB2 Boundary Review",
                    region.get("kind").getAsString() + " through " + region.get("end_exclusive").getAsString()
                    + "; byte-bound review; source language unconfirmed");
            }
            byte[] after = new byte[payload.length];
            currentProgram.getMemory().getBytes(toAddr(0x80010000L), after);
            if (!hash(after).equals(hash(payload))) throw new IllegalStateException("Memory changed; rolling back");
            success = true;
        }
        finally { currentProgram.endTransaction(transaction, success); }
        report.addProperty("applied", success);
        Files.writeString(directory.resolve("review-apply.json"), gson.toJson(report) + "\n");
        println("Applied reviewed metadata; all initialized payload bytes unchanged");
    }
}
