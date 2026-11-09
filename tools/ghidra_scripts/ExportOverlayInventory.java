// Export the PSX plugin's analysis, counting each file/address as a distinct identity.
// @category CoolBoarders2
import ghidra.app.script.GhidraScript;
import ghidra.framework.Application;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import com.google.gson.*;
import java.nio.file.*;
import java.security.MessageDigest;
import java.util.*;
import java.io.ByteArrayOutputStream;

public class ExportOverlayInventory extends GhidraScript {
    private String sha(byte[] bytes) throws Exception {
        return HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(bytes));
    }
    private String hex(long value) { return String.format("0x%08X", value); }
    public void run() throws Exception {
        String[] args = getScriptArgs();
        Path directory = Path.of(args[0]);
        Files.createDirectories(directory);
        JsonObject plan = JsonParser.parseString(Files.readString(directory.resolve("plan.json"))).getAsJsonObject();
        String name = args.length > 1 ? args[1] : currentProgram.getName();
        JsonObject item = plan.getAsJsonObject("programs").getAsJsonObject(name);
        if (item == null) throw new IllegalStateException("Program not in overlay plan: " + name);
        long start = Long.decode(item.get("load_address").getAsString());
        Address scope = name.equals("SCUS_943.58") ? toAddr(start) : currentProgram.getMemory().getBlock(name).getStart();
        int size = item.get("size").getAsInt();
        byte[] original = Files.readAllBytes(Path.of(item.get("container_source").getAsString()));
        byte[] expected = name.equals("SCUS_943.58") ? Arrays.copyOfRange(original, 2048, original.length) : original;
        byte[] mapped = new byte[size];
        if (currentProgram.getMemory().getBytes(scope, mapped) != size || !Arrays.equals(expected, mapped))
            throw new IllegalStateException("Imported bytes differ from original for " + name);
        byte[] boot = Files.readAllBytes(Path.of("/inputs/files/SCUS_943.58"));
        byte[] resident = new byte[boot.length - 2048];
        if (currentProgram.getMemory().getBytes(toAddr(0x80010000L), resident) != resident.length ||
            !Arrays.equals(resident, Arrays.copyOfRange(boot, 2048, boot.length)))
            throw new IllegalStateException("Resident boot bytes changed");
        String version = currentProgram.getOptions(ghidra.program.model.listing.Program.PROGRAM_INFO).getString("PsyQ Version", "");
        if (!version.replace(".", "").equals("370")) throw new IllegalStateException("Unexpected plugin PsyQ version: " + version);
        // Use the plugin's own names and naming transformations, not a new matcher.
        Set<String> sdkNames = new HashSet<>();
        Path psyq = Path.of("/opt/ghidra/Ghidra/Extensions/ghidra_psx_ldr/data/psyq/370");
        try (var files = Files.list(psyq)) {
            for (Path file : files.filter(p -> p.toString().endsWith(".json")).toList()) {
                for (JsonElement element : JsonParser.parseString(Files.readString(file)).getAsJsonArray()) {
                    JsonObject obj = element.getAsJsonObject();
                    if (!obj.has("sig")) continue;
                    String prefix = obj.get("name").getAsString().replace(".", "_") + "_";
                    for (JsonElement label : obj.getAsJsonArray("labels")) {
                        String symbol = label.getAsJsonObject().get("name").getAsString();
                        if (!symbol.startsWith("loc_")) sdkNames.add(symbol.replace("text_", prefix));
                    }
                }
            }
        }
        JsonArray functions = new JsonArray();
        int sdk = 0, codeBytes = 0;
        FunctionIterator iterator = currentProgram.getFunctionManager().getFunctions(true);
        while (iterator.hasNext()) {
            monitor.checkCancelled();
            Function function = iterator.next();
            long entry = function.getEntryPoint().getOffset();
            if (entry < start || entry >= start + size || !function.getEntryPoint().getAddressSpace().equals(scope.getAddressSpace())) continue;
            JsonObject row = new JsonObject();
            row.addProperty("identity", name + ":" + hex(entry));
            row.addProperty("address", hex(entry));
            row.addProperty("name", function.getName());
            row.addProperty("symbol_source", function.getSymbol().getSource().toString());
            boolean isSdk = sdkNames.contains(function.getName());
            row.addProperty("classification", isSdk ? "sdk" : "unidentified");
            if (isSdk) sdk++;
            ByteArrayOutputStream bytes = new ByteArrayOutputStream();
            JsonArray ranges = new JsonArray();
            boolean inside = true;
            for (AddressRange range : function.getBody().getAddressRanges()) {
                long low = range.getMinAddress().getOffset(), high = range.getMaxAddress().getOffset();
                if (!range.getMinAddress().getAddressSpace().equals(scope.getAddressSpace()) || low < start || high >= start + size) { inside = false; continue; }
                byte[] body = new byte[(int)range.getLength()];
                currentProgram.getMemory().getBytes(range.getMinAddress(), body);
                bytes.write(body);
                JsonArray extent = new JsonArray(); extent.add(hex(low)); extent.add(hex(high + 1)); ranges.add(extent);
            }
            row.add("ranges", ranges);
            row.addProperty("body_inside_file", inside);
            row.addProperty("body_bytes", bytes.size());
            row.addProperty("body_sha256", sha(bytes.toByteArray()));
            codeBytes += bytes.size();
            functions.add(row);
        }
        JsonObject report = new JsonObject();
        report.addProperty("run_id", plan.get("run_id").getAsString());
        report.addProperty("program", currentProgram.getName());
        report.addProperty("image", name);
        report.addProperty("native_overlay", !name.equals("SCUS_943.58"));
        report.addProperty("address_space", scope.getAddressSpace().getName());
        report.addProperty("source_sha256", sha(original));
        report.addProperty("mapped_sha256", sha(mapped));
        report.addProperty("plugin_psyq_version", version);
        report.addProperty("analysis_timed_out", false);
        report.addProperty("functions", functions.size());
        report.addProperty("sdk_functions", sdk);
        report.addProperty("unidentified_functions", functions.size() - sdk);
        report.addProperty("function_body_bytes", codeBytes);
        report.add("entries", functions);
        // Smoke-check the native decompiler for each imported overlay entry.
        if (!name.equals("SCUS_943.58")) {
            Function entry = getFunctionAt(scope.add(4));
            if (entry == null) throw new IllegalStateException("Missing overlay entry function");
            DecompInterface decompiler = new DecompInterface();
            try {
                if (!decompiler.openProgram(currentProgram)) throw new IllegalStateException(decompiler.getLastMessage());
                DecompileResults result = decompiler.decompileFunction(entry, 60, monitor);
                if (!result.decompileCompleted()) throw new IllegalStateException("Overlay entry decompiler failed: " + result.getErrorMessage());
                report.addProperty("entry_decompiles", true);
            } finally { decompiler.dispose(); }
        }
        Files.writeString(directory.resolve(name + ".json"), new GsonBuilder().setPrettyPrinting().create().toJson(report) + "\n");
        println("Inventory complete: " + name + ", " + functions.size() + " functions, " + sdk + " SDK");
    }
}
