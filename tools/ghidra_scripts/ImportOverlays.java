// Add the game's raw overlays as native overlay blocks in its existing program.
// @category CoolBoarders2
import ghidra.app.script.GhidraScript;
import ghidra.app.util.importer.MessageLog;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.SourceType;
import com.google.gson.*;
import java.nio.file.*;
import java.io.ByteArrayInputStream;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.util.*;
import psx.PsxAnalyzer;

public class ImportOverlays extends GhidraScript {
    private String sha(byte[] bytes) throws Exception {
        return HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(bytes));
    }
    public void run() throws Exception {
        Path directory = Path.of(getScriptArgs()[0]);
        JsonObject plan = JsonParser.parseString(Files.readString(directory.resolve("plan.json"))).getAsJsonObject();
        JsonObject programs = plan.getAsJsonObject("programs");
        for (var item : programs.entrySet()) {
            byte[] bytes = Files.readAllBytes(Path.of(item.getValue().getAsJsonObject().get("container_source").getAsString()));
            if (!sha(bytes).equals(item.getValue().getAsJsonObject().get("sha256").getAsString()))
                throw new IllegalStateException("Source changed: " + item.getKey());
        }
        var options = currentProgram.getOptions("Analyzers");
        boolean oldAnalyzer = options.getBoolean("PsyQ Signatures", true);
        boolean oldFirst = PsxAnalyzer.onlyFirst;
        options.setBoolean("PsyQ Signatures", false);
        int transaction = currentProgram.startTransaction("Import CB2 native overlays");
        boolean success = false;
        try {
            List<MemoryBlock> blocks = new ArrayList<>();
            for (var item : programs.entrySet()) {
                String name = item.getKey();
                if (name.equals("SCUS_943.58")) continue;
                JsonObject config = item.getValue().getAsJsonObject();
                byte[] bytes = Files.readAllBytes(Path.of(config.get("container_source").getAsString()));
                MemoryBlock block = currentProgram.getMemory().getBlock(name);
                if (block == null) {
                    block = currentProgram.getMemory().createInitializedBlock(name,
                        toAddr(Long.decode(config.get("load_address").getAsString())),
                        new ByteArrayInputStream(bytes), bytes.length, monitor, true);
                }
                if (!block.isOverlay() || block.getSize() != bytes.length) throw new IllegalStateException("Conflicting block: " + name);
                byte[] mapped = new byte[bytes.length];
                currentProgram.getMemory().getBytes(block.getStart(), mapped);
                if (!Arrays.equals(bytes, mapped)) throw new IllegalStateException("Overlay differs: " + name);
                block.setRead(true); block.setWrite(true); block.setExecute(true);
                currentProgram.getProgramContext().setValue(currentProgram.getRegister("gp"),
                    block.getStart(), block.getEnd(), new BigInteger("80050274", 16));
                if (getDataAt(block.getStart()) == null) createDWord(block.getStart());
                var entry = block.getStart().add(4);
                disassemble(entry);
                if (getFunctionAt(entry) == null) createFunction(entry, name.replace(".BIN", "") + "_entry");
                currentProgram.getSymbolTable().addExternalEntryPoint(entry);
                blocks.add(block);
                println("Native overlay mapped: " + block.getName() + " " + block.getStart() + "-" + block.getEnd());
            }
            // Analyze the newly added blocks, without scanning the resident SDK again.
            analyzeChanges(currentProgram);
            PsxAnalyzer.onlyFirst = false;
            for (MemoryBlock block : blocks) {
                monitor.checkCancelled();
                MessageLog log = new MessageLog();
                if (!new PsxAnalyzer().added(currentProgram, new AddressSet(block.getStart(), block.getEnd()), monitor, log))
                    throw new IllegalStateException("PSX analyzer failed for " + block.getName() + ": " + log);
                println("PSX plugin complete: " + block.getName());
            }
            analyzeChanges(currentProgram);
            runScript("AuditOverlayReferences.java", new String[]{directory.resolve("references.json").toString()});
            runScript("ExportAllOverlays.java", new String[]{directory.toString()});
            success = true;
        } finally {
            currentProgram.endTransaction(transaction, success);
            options.setBoolean("PsyQ Signatures", oldAnalyzer);
            PsxAnalyzer.onlyFirst = oldFirst;
        }
    }
}
