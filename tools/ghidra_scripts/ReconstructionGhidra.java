// Fresh pseudocode and transactional feedback for supervised reconstruction.
// @category CoolBoarders2
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.app.util.cparser.C.CParser;
import ghidra.app.cmd.function.ApplyFunctionSignatureCmd;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;
import com.google.gson.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.security.MessageDigest;
import java.util.*;

public class ReconstructionGhidra extends GhidraScript {
    private String sha(byte[] bytes) throws Exception {
        return HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(bytes));
    }
    private Address address(String image, String value) {
        long offset = Long.decode(value);
        return image.equals("SCUS_943.58") ? toAddr(offset)
            : currentProgram.getMemory().getBlock(image).getStart().getAddressSpace().getAddress(offset);
    }
    private void verify(JsonObject request) throws Exception {
        for (var entry : request.getAsJsonObject("references").entrySet()) {
            byte[] original = Files.readAllBytes(Path.of("/inputs/files", entry.getKey()));
            if (!sha(original).equals(entry.getValue().getAsString()))
                throw new IllegalStateException("Pinned reference differs: " + entry.getKey());
            byte[] expected = entry.getKey().equals("SCUS_943.58") ? Arrays.copyOfRange(original, 2048, original.length) : original;
            byte[] mapped = new byte[expected.length];
            if (currentProgram.getMemory().getBytes(address(entry.getKey(), entry.getKey().equals("SCUS_943.58")
                    ? "0x80010000" : "0x8005AB68"), mapped) != mapped.length || !Arrays.equals(mapped, expected))
                throw new IllegalStateException("Mapped original differs: " + entry.getKey());
        }
        ByteArrayOutputStream body = new ByteArrayOutputStream();
        for (var element : request.getAsJsonArray("ranges")) {
            var range = element.getAsJsonArray();
            int length = (int)(Long.decode(range.get(1).getAsString()) - Long.decode(range.get(0).getAsString()));
            byte[] bytes = new byte[length];
            if (currentProgram.getMemory().getBytes(address(request.get("image").getAsString(), range.get(0).getAsString()), bytes) != length)
                throw new IllegalStateException("Missing function bytes");
            body.write(bytes);
        }
        if (!sha(body.toByteArray()).equals(request.get("body_sha256").getAsString()))
            throw new IllegalStateException("Target body differs");
    }
    private JsonObject signature(Function function) {
        JsonObject row = new JsonObject();
        row.addProperty("address", function.getEntryPoint().toString());
        row.addProperty("name", function.getName());
        row.addProperty("prototype", function.getPrototypeString(false, false));
        row.addProperty("signature_source", function.getSignatureSource().toString());
        row.addProperty("calling_convention", function.getCallingConventionName());
        JsonArray parameters = new JsonArray();
        for (var p : function.getParameters()) {
            JsonObject item = new JsonObject(); item.addProperty("name", p.getName());
            item.addProperty("type", p.getDataType().getPathName());
            item.addProperty("storage", p.getVariableStorage().toString()); parameters.add(item);
        }
        row.add("parameters", parameters); return row;
    }
    private JsonObject export(JsonObject request, Function function, Path directory) throws Exception {
        // Reject stale body metadata rather than quietly decompiling another extent.
        var ranges = function.getBody().getAddressRanges();
        JsonArray actual = new JsonArray();
        while (ranges.hasNext()) {
            var range = ranges.next(); JsonArray pair = new JsonArray();
            pair.add(String.format("0x%08X", range.getMinAddress().getOffset()));
            pair.add(String.format("0x%08X", range.getMaxAddress().getOffset() + 1)); actual.add(pair);
        }
        if (!actual.equals(request.getAsJsonArray("ranges"))) throw new IllegalStateException("Saved function extent changed; refresh inventory");
        DecompInterface decompiler = new DecompInterface();
        try {
            if (!decompiler.openProgram(currentProgram)) throw new IllegalStateException(decompiler.getLastMessage());
            var result = decompiler.decompileFunction(function, 60, monitor);
            if (!result.decompileCompleted() || result.getDecompiledFunction() == null)
                throw new IllegalStateException("Decompile failed: " + result.getErrorMessage());
            Files.writeString(directory.resolve("target.ghidra.c"), result.getDecompiledFunction().getC());
            JsonObject report = signature(function);
            report.addProperty("inferred_prototype", result.getDecompiledFunction().getSignature());
            report.addProperty("warnings", result.getErrorMessage());
            Set<Address> seen = new HashSet<>(); JsonArray callees = new JsonArray(), globals = new JsonArray();
            var instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
            while (instructions.hasNext()) {
                for (var reference : instructions.next().getReferencesFrom()) {
                    Address to = reference.getToAddress(); if (!seen.add(to)) continue;
                    Function callee = getFunctionAt(to);
                    if (reference.getReferenceType().isCall() && callee != null) {
                        callees.add(signature(callee));
                    } else if (reference.getReferenceType().isData()) {
                        var data = getDataAt(to);
                        if (data != null) {
                            JsonObject row = new JsonObject(); row.addProperty("address", to.toString());
                            row.addProperty("name", getSymbolAt(to) == null ? "" : getSymbolAt(to).getName());
                            row.addProperty("type", data.getDataType().getPathName()); globals.add(row);
                        }
                    }
                }
            }
            report.add("callees", callees); report.add("globals", globals);
            if (request.has("sync_types") && request.get("sync_types").getAsBoolean()) {
                // Cache all saved types once, rather than only this target's dependencies.
                List<DataType> types = new ArrayList<>();
                var allTypes = currentProgram.getDataTypeManager().getAllDataTypes();
                while (allTypes.hasNext()) types.add(allTypes.next());
                StringWriter header = new StringWriter();
                new DataTypeWriter(currentProgram.getDataTypeManager(), header).write(types, monitor);
                Files.writeString(directory.resolve("ghidra-types.h"), header.toString());
            }
            return report;
        } finally { decompiler.dispose(); }
    }
    private JsonObject update(JsonObject request, Function function, boolean apply) throws Exception {
        int transaction = currentProgram.startTransaction("Reviewed matching reconstruction feedback");
        boolean success = false;
        JsonObject report = new JsonObject();
        try (var archive = FileDataTypeManager.openFileArchive(new File("/opt/ghidra/Ghidra/Extensions/ghidra_psx_ldr/data/psyq370.gdt"), false);
             var staging = new StandAloneDataTypeManager("CB2Matching", currentProgram.getDataTypeManager().getDataOrganization())) {
            CParser parser = new CParser(staging, true, new DataTypeManager[]{currentProgram.getDataTypeManager(), archive});
            String declarations = request.get("declarations").getAsString();
            parser.parse(new ByteArrayInputStream(declarations.getBytes(StandardCharsets.UTF_8)));
            if (!parser.didParseSucceed()) throw new IllegalStateException(parser.getParseMessages());
            // Never silently replace a previously established type layout.
            var iterator = staging.getAllDataTypes();
            while (iterator.hasNext()) {
                DataType candidate = iterator.next();
                DataType previous = currentProgram.getDataTypeManager().getDataType(candidate.getPathName());
                if (previous != null && !(candidate instanceof FunctionDefinition) && !previous.isEquivalent(candidate))
                    throw new IllegalStateException("Conflicting existing type: " + candidate.getPathName());
            }
            // Retain authored layouts even when only used by local variables;
            // signature/global application alone imports dependencies only.
            iterator = staging.getAllDataTypes();
            while (iterator.hasNext()) {
                DataType candidate = iterator.next();
                if (!(candidate instanceof FunctionDefinition) && !candidate.getName().startsWith("CB2GlobalType_"))
                    currentProgram.getDataTypeManager().addDataType(candidate, DataTypeConflictHandler.DEFAULT_HANDLER);
            }
            var parsed = parser.getFunctions().get(request.get("name").getAsString());
            if (!(parsed instanceof FunctionDefinition definition)) throw new IllegalStateException("Target prototype missing");
            if (function.hasCustomVariableStorage()) throw new IllegalStateException("Custom ABI requires separate review");
            var command = new ApplyFunctionSignatureCmd(function.getEntryPoint(), definition, SourceType.USER_DEFINED);
            if (!command.applyTo(currentProgram, monitor)) throw new IllegalStateException(command.getStatusMsg());
            JsonArray globals = new JsonArray();
            for (var element : request.getAsJsonArray("globals")) {
                JsonObject global = element.getAsJsonObject(); String name = global.get("name").getAsString();
                DataType dt = parser.getTypes().get(global.get("type_alias").getAsString());
                if (dt instanceof TypeDef alias) dt = alias.getBaseDataType();
                if (dt == null || dt.getLength() <= 0) throw new IllegalStateException("Global declaration missing: " + name);
                Address at = address(global.get("image").getAsString(), global.get("address").getAsString());
                var old = getDataAt(at);
                boolean unknownExtent = dt instanceof Array array && array.getNumElements() == 0;
                if (unknownExtent) {
                    // C's [] supplies an element type, not an allocation size. Ghidra's
                    // zero-element array reports a one-byte placeholder from getLength().
                    // Type one element as a prefix; never infer the table's capacity.
                    Array array = (Array) dt;
                    dt = new ArrayDataType(array.getDataType(), 1, array.getElementLength());
                    // Preserve a previously established extent for the same element type.
                    if (old != null && old.getDataType() instanceof Array previous &&
                            previous.getNumElements() > 0 &&
                            previous.getElementLength() == array.getElementLength() &&
                            previous.getDataType().isEquivalent(array.getDataType()))
                        dt = previous;
                }
                if (old != null && old.getLength() > dt.getLength()) throw new IllegalStateException("Global would truncate existing data: " + name);
                // Explicit reviewed globals may replace existing data, but never instructions.
                if (currentProgram.getListing().getInstructions(new ghidra.program.model.address.AddressSet(at, at.add(dt.getLength()-1)), true).hasNext())
                    throw new IllegalStateException("Global overlaps instructions: " + name);
                var data = DataUtilities.createData(currentProgram, at, dt, -1, DataUtilities.ClearDataMode.CLEAR_ALL_CONFLICT_DATA);
                if (unknownExtent) {
                    String note = "Authored array has unknown total extent ([]); the applied type covers only a known prefix, not the table capacity.";
                    String comment = data.getComment(ghidra.program.model.listing.CodeUnit.PLATE_COMMENT);
                    if (comment == null || !comment.contains(note))
                        data.setComment(ghidra.program.model.listing.CodeUnit.PLATE_COMMENT,
                            comment == null || comment.isBlank() ? note : comment + "\n" + note);
                }
                createLabel(at, name, true);
                JsonObject appliedGlobal = new JsonObject();
                appliedGlobal.addProperty("name", name);
                appliedGlobal.addProperty("type", dt.getDisplayName());
                appliedGlobal.addProperty("length", dt.getLength());
                appliedGlobal.addProperty("unknown_extent", unknownExtent);
                globals.add(appliedGlobal);
            }
            verify(request);
            report.add("globals", globals);
            report.add("function", signature(function)); report.addProperty("applied", apply);
            success = apply;
        } finally { currentProgram.endTransaction(transaction, success); }
        return report;
    }
    public void run() throws Exception {
        Path directory = Path.of(getScriptArgs()[0]); String mode = getScriptArgs()[1];
        if (!Set.of("export", "plan", "apply").contains(mode)) throw new IllegalArgumentException("Unknown mode");
        byte[] payload = Files.readAllBytes(directory.resolve("request.json"));
        JsonObject request = JsonParser.parseString(new String(payload, StandardCharsets.UTF_8)).getAsJsonObject();
        verify(request);
        Function function = getFunctionAt(address(request.get("image").getAsString(), request.get("address").getAsString()));
        if (function == null) throw new IllegalStateException("Missing selected function");
        JsonObject result = mode.equals("export") ? export(request, function, directory) : update(request, function, mode.equals("apply"));
        verify(request);
        result.addProperty("request_sha256", sha(payload)); result.addProperty("mode", mode);
        JsonObject files = new JsonObject();
        if (mode.equals("export")) {
            files.addProperty("target.ghidra.c", sha(Files.readAllBytes(directory.resolve("target.ghidra.c"))));
            if (request.has("sync_types") && request.get("sync_types").getAsBoolean())
                files.addProperty("ghidra-types.h", sha(Files.readAllBytes(directory.resolve("ghidra-types.h"))));
        }
        result.add("files", files);
        Files.writeString(directory.resolve("complete.json"), new GsonBuilder().setPrettyPrinting().create().toJson(result) + "\n");
        println("Reconstruction Ghidra " + mode + " complete");
    }
}
