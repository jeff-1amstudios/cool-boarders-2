// Export existing analysis without changing function definitions or reanalyzing.
// @category CoolBoarders2
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.address.AddressRangeIterator;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressIterator;
import ghidra.program.model.listing.CodeUnit;
import ghidra.program.model.listing.CodeUnitIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.util.StringPropertyMap;
import com.google.gson.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.HexFormat;

public class ExportBoundaryEvidence extends GhidraScript {
    private String clean(String value) {
        if (value == null) return "";
        return value.replace('\t', ' ').replace('\n', ' ');
    }

    public void run() throws Exception {
        Path directory = Path.of(getScriptArgs()[0]);
        Files.createDirectories(directory);
        byte[] payload = new byte[0x40800];
        int read = currentProgram.getMemory().getBytes(toAddr(0x80010000L), payload);
        if (read != payload.length) throw new IllegalStateException("Incomplete EXE mapping");
        String digest = HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(payload));
        Files.writeString(directory.resolve("program.tsv"),
            "program\tlanguage\tpayload_start\tpayload_size\tpayload_sha256\n"
            + clean(currentProgram.getName()) + "\t" + currentProgram.getLanguageID()
            + "\t80010000\t" + payload.length + "\t" + digest + "\n");
        StringBuilder functions = new StringBuilder("entry\tname\tsource\tstart\tend_inclusive\tnamespace\tcomment\trepeatable_comment\n");
        FunctionIterator iterator = currentProgram.getFunctionManager().getFunctions(true);
        while (iterator.hasNext()) {
            monitor.checkCancelled();
            Function function = iterator.next();
            // This is the resident boot boundary export, not the overlay inventory.
            if (!function.getEntryPoint().getAddressSpace().equals(currentProgram.getAddressFactory().getDefaultAddressSpace())) continue;
            AddressRangeIterator ranges = function.getBody().getAddressRanges(true);
            while (ranges.hasNext()) {
                AddressRange range = ranges.next();
                functions.append(function.getEntryPoint()).append('\t')
                    .append(clean(function.getName())).append('\t')
                    .append(function.getSymbol().getSource()).append('\t')
                    .append(range.getMinAddress()).append('\t')
                    .append(range.getMaxAddress()).append('\t')
                    .append(clean(function.getParentNamespace().getName(true))).append('\t')
                    .append(clean(function.getComment())).append('\t')
                    .append(clean(function.getRepeatableComment())).append('\n');
            }
        }
        Files.writeString(directory.resolve("functions.tsv"), functions);
        StringBuilder references = new StringBuilder("from\tto\ttype\tsource\n");
        CodeUnitIterator units = currentProgram.getListing().getCodeUnits(true);
        while (units.hasNext()) {
            monitor.checkCancelled();
            CodeUnit unit = units.next();
            if (!unit.getAddress().getAddressSpace().equals(currentProgram.getAddressFactory().getDefaultAddressSpace())) continue;
            for (Reference reference : unit.getReferencesFrom()) {
                if (!reference.getToAddress().getAddressSpace().equals(currentProgram.getAddressFactory().getDefaultAddressSpace())) continue;
                references.append(reference.getFromAddress()).append('\t')
                    .append(reference.getToAddress()).append('\t')
                    .append(reference.getReferenceType()).append('\t')
                    .append(reference.getSource()).append('\n');
            }
        }
        Files.writeString(directory.resolve("references.tsv"), references);
        StringPropertyMap reviews = currentProgram.getUsrPropertyManager().getStringPropertyMap("CB2ReviewedBoundaryUnits");
        if (reviews != null) {
            JsonObject catalog = JsonParser.parseString(reviews.getString(toAddr(0x80010000L))).getAsJsonObject();
            JsonObject regions = new JsonObject();
            AddressIterator addresses = reviews.getPropertyIterator();
            while (addresses.hasNext()) {
                Address address = addresses.next();
                if (address.getOffset() == 0x80010000L) continue;
                JsonObject region = JsonParser.parseString(reviews.getString(address)).getAsJsonObject();
                int size = region.get("size").getAsInt();
                byte[] bytes = new byte[size];
                if (currentProgram.getMemory().getBytes(address, bytes) != size) throw new IllegalStateException("Incomplete reviewed region");
                String key = region.has("instruction_sha256") ? "instruction_sha256" : "bytes_sha256";
                String sha = HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(bytes));
                if (!sha.equals(region.get(key).getAsString())) throw new IllegalStateException("Stale reviewed bytes at " + address);
                if (region.get("kind").getAsString().equals("standalone")) {
                    Function function = getFunctionAt(address);
                    Address end = toAddr(Long.decode(region.get("end_exclusive").getAsString()) - 1);
                    if (function == null || function.getBody().getNumAddresses() != size || !function.getBody().contains(address, end)) {
                        throw new IllegalStateException("Reviewed function body changed at " + address);
                    }
                }
                regions.add(region.get("address").getAsString(), region);
            }
            catalog.add("regions", regions);
            Files.writeString(directory.resolve("reviewed-units.json"), new GsonBuilder().setPrettyPrinting().create().toJson(catalog) + "\n");
        }
        // Headless script errors may be logged without a nonzero process exit.
        // Publish a completion record only after every check and file succeeds.
        JsonObject complete = new JsonObject();
        complete.addProperty("payload_sha256", digest);
        for (String file : new String[]{"program.tsv", "functions.tsv", "references.tsv", "reviewed-units.json"}) {
            Path path = directory.resolve(file);
            if (Files.exists(path)) complete.addProperty(file,
                HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(path))));
        }
        Files.writeString(directory.resolve("export-complete.json"), new GsonBuilder().setPrettyPrinting().create().toJson(complete) + "\n");
        println("Read-only boundary evidence exported to " + directory);
    }
}
