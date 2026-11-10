// Read-only comparison of the pinned SDK catalog, PsyQ archive and saved program.
// @category CoolBoarders2
import ghidra.app.script.GhidraScript;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.Function;
import com.google.gson.*;
import java.nio.file.*;
import java.security.MessageDigest;
import java.util.*;

public class AuditSdkTypes extends GhidraScript {
    private String sha(byte[] bytes) throws Exception {
        return HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(bytes));
    }
    private JsonObject type(DataType dt) {
        JsonObject row = new JsonObject();
        row.addProperty("name", dt.getName());
        row.addProperty("path", dt.getPathName());
        row.addProperty("kind", dt.getClass().getSimpleName());
        row.addProperty("length", dt.getLength());
        if (dt instanceof TypeDef alias) row.addProperty("base_type", alias.getBaseDataType().getPathName());
        if (dt instanceof Array array) {
            row.addProperty("element_type", array.getDataType().getPathName());
            row.addProperty("element_count", array.getNumElements());
        }
        if (dt instanceof Composite composite) {
            JsonArray fields = new JsonArray();
            for (DataTypeComponent field : composite.getDefinedComponents()) {
                JsonObject f = new JsonObject();
                f.addProperty("name", field.getFieldName());
                f.addProperty("offset", field.getOffset());
                f.addProperty("length", field.getLength());
                f.addProperty("type", field.getDataType().getPathName());
                fields.add(f);
            }
            row.add("fields", fields);
        }
        if (dt instanceof FunctionDefinition definition)
            row.addProperty("prototype", definition.getPrototypeString());
        return row;
    }
    private DataType base(DataType dt) {
        while (dt instanceof TypeDef alias) dt = alias.getBaseDataType();
        return dt;
    }
    private boolean equivalent(Function function, FunctionDefinition definition, boolean decay) {
        var actual = function.getParameters();
        var expected = definition.getArguments();
        if (actual.length != expected.length || function.hasVarArgs() != definition.hasVarArgs()
                || !function.getReturnType().isEquivalent(definition.getReturnType())) return false;
        for (int i = 0; i < actual.length; i++) {
            DataType a = actual[i].getDataType(), e = expected[i].getDataType();
            if (a.isEquivalent(e)) continue;
            a = base(a); e = base(e);
            if (decay && a instanceof Pointer pointer && e instanceof Array array
                    && pointer.getDataType().isEquivalent(array.getDataType())) continue;
            return false;
        }
        return true;
    }
    private void functions(String image, JsonObject catalog, DataTypeManager archive, JsonArray rows) {
        var space = image.equals("SCUS_943.58") ? currentProgram.getAddressFactory().getDefaultAddressSpace()
            : currentProgram.getMemory().getBlock(image).getStart().getAddressSpace();
        for (var entry : catalog.entrySet()) {
            JsonObject row = new JsonObject();
            row.addProperty("identity", image + ":" + entry.getKey());
            row.addProperty("sdk_name", entry.getValue().getAsString());
            var address = space.getAddress(Long.decode(entry.getKey()));
            Function function = getFunctionAt(address);
            List<DataType> candidates = new ArrayList<>();
            archive.findDataTypes(entry.getValue().getAsString(), candidates);
            List<FunctionDefinition> definitions = new ArrayList<>();
            JsonArray prototypes = new JsonArray();
            for (DataType candidate : candidates)
                if (candidate instanceof FunctionDefinition definition) {
                    definitions.add(definition); prototypes.add(definition.getPrototypeString());
                }
            row.add("archive_prototypes", prototypes);
            if (function == null) row.addProperty("status", "missing_function_entry");
            else {
                row.addProperty("program_name", function.getName());
                row.addProperty("program_prototype", function.getPrototypeString(false, false));
                row.addProperty("signature_source", function.getSignatureSource().toString());
                row.addProperty("calling_convention", function.getCallingConventionName());
                row.addProperty("custom_storage", function.hasCustomVariableStorage());
                JsonArray params = new JsonArray();
                for (var parameter : function.getParameters()) {
                    JsonObject p = new JsonObject();
                    p.addProperty("name", parameter.getName());
                    p.addProperty("type", parameter.getDataType().getPathName());
                    p.addProperty("storage", parameter.getVariableStorage().toString());
                    params.add(p);
                }
                row.add("parameters", params);
                row.addProperty("status", definitions.isEmpty() ? "no_archive_prototype"
                    : definitions.stream().anyMatch(d -> equivalent(function, d, false)) ? "types_agree"
                    : definitions.stream().anyMatch(d -> equivalent(function, d, true)) ? "parameter_array_decay" : "types_differ");
            }
            rows.add(row);
        }
    }
    public void run() throws Exception {
        Path directory = Path.of(getScriptArgs()[0]);
        Path catalogPath = directory.resolve("catalog.json");
        JsonObject catalog = JsonParser.parseString(Files.readString(catalogPath)).getAsJsonObject();
        if (!currentProgram.getLanguageID().toString().equals("PSX:LE:32:default"))
            throw new IllegalStateException("Unexpected program language");
        String version = currentProgram.getOptions(ghidra.program.model.listing.Program.PROGRAM_INFO)
            .getString("PsyQ Version", "");
        if (!version.equals(catalog.get("version").getAsString()))
            throw new IllegalStateException("SDK version differs from catalog");
        Map<String, JsonObject> images = new LinkedHashMap<>();
        images.put("SCUS_943.58", catalog);
        for (var entry : catalog.getAsJsonObject("overlays").entrySet())
            images.put(entry.getKey(), entry.getValue().getAsJsonObject());
        for (var image : images.entrySet()) {
            byte[] original = Files.readAllBytes(Path.of("/inputs/files", image.getKey()));
            if (!sha(original).equals(image.getValue().get("reference_sha256").getAsString()))
                throw new IllegalStateException("Reference hash differs: " + image.getKey());
            boolean boot = image.getKey().equals("SCUS_943.58");
            byte[] expected = boot ? Arrays.copyOfRange(original, 2048, original.length) : original;
            var start = boot ? toAddr(0x80010000L) : currentProgram.getMemory().getBlock(image.getKey()).getStart();
            byte[] mapped = new byte[expected.length];
            if (currentProgram.getMemory().getBytes(start, mapped) != mapped.length || !Arrays.equals(mapped, expected))
                throw new IllegalStateException("Mapped bytes differ: " + image.getKey());
        }
        Path archivePath = Path.of("/opt/ghidra/Ghidra/Extensions/ghidra_psx_ldr/data/psyq370.gdt");
        JsonObject report = new JsonObject();
        report.addProperty("schema_version", 1);
        report.addProperty("sdk_version", version);
        report.addProperty("archive_sha256", sha(Files.readAllBytes(archivePath)));
        report.addProperty("catalog_sha256", sha(Files.readAllBytes(catalogPath)));
        report.addProperty("scope", "Saved database signatures, not locally inferred decompiler signatures. Type agreement checks return/parameter types and variadic flag; it does not prove ABI storage, SDK identity or original source types.");
        try (FileDataTypeManager archive = FileDataTypeManager.openFileArchive(archivePath.toFile(), false)) {
            JsonArray types = new JsonArray(), functions = new JsonArray();
            var iterator = archive.getAllDataTypes();
            while (iterator.hasNext()) {
                monitor.checkCancelled();
                DataType dt = iterator.next();
                JsonObject row = type(dt);
                DataType saved = currentProgram.getDataTypeManager().getDataType(dt.getPathName());
                row.addProperty("program_definition_status", saved == null ? "absent_at_archive_path"
                    : saved.isEquivalent(dt) ? "equivalent" : "differs");
                types.add(row);
            }
            for (var image : images.entrySet())
                functions(image.getKey(), image.getValue().getAsJsonObject("functions"), archive, functions);
            report.add("archive_types", types);
            report.add("sdk_functions", functions);
            JsonArray programTypes = new JsonArray();
            var programIterator = currentProgram.getDataTypeManager().getAllDataTypes();
            while (programIterator.hasNext()) programTypes.add(type(programIterator.next()));
            report.add("program_types", programTypes);
        }
        Files.writeString(directory.resolve("audit.json"), new GsonBuilder().setPrettyPrinting().create().toJson(report) + "\n");
        Files.writeString(directory.resolve("complete.json"), "{\"audit_sha256\":\"" + sha(Files.readAllBytes(directory.resolve("audit.json"))) + "\"}\n");
        println("SDK type audit complete");
    }
}
