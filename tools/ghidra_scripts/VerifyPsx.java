// Verify the local PSX language, loader result, and native decompiler.
// @category CoolBoarders2
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.charset.StandardCharsets;

public class VerifyPsx extends GhidraScript {
    @Override
    public void run() throws Exception {
        if (!currentProgram.getLanguageID().toString().equals("PSX:LE:32:default")) {
            throw new IllegalStateException("PSX extension was not selected: " + currentProgram.getLanguageID());
        }
        Address entry = toAddr(0x800120fcL);
        if (!currentProgram.getMemory().contains(entry)) {
            throw new IllegalStateException("Boot entrypoint is not mapped");
        }
        Function function = getFunctionAt(entry);
        if (function == null) {
            disassemble(entry);
            function = createFunction(entry, "_start");
        }
        if (function == null) throw new IllegalStateException("No function at boot entrypoint");
        DecompInterface decompiler = new DecompInterface();
        try {
            if (!decompiler.openProgram(currentProgram)) {
                throw new IllegalStateException(decompiler.getLastMessage());
            }
            DecompileResults result = decompiler.decompileFunction(function, 60, monitor);
            if (!result.decompileCompleted() || result.getDecompiledFunction() == null) {
                throw new IllegalStateException("Decompiler failed: " + result.getErrorMessage());
            }
            String report = "PASS: PSX loader/language and native decompiler\n"
                + "Program: " + currentProgram.getName() + "\n"
                + "Language: " + currentProgram.getLanguageID() + "\n"
                + "Executable format: " + currentProgram.getExecutableFormat() + "\n"
                + "Image base: " + currentProgram.getImageBase() + "\n"
                + "Entry: " + entry + "\n"
                + "Function count (analysis estimate): " + currentProgram.getFunctionManager().getFunctionCount() + "\n\n"
                + result.getDecompiledFunction().getC();
            if (getScriptArgs().length > 0) {
                Files.writeString(Path.of(getScriptArgs()[0]), report, StandardCharsets.UTF_8);
            }
            println(report);
        }
        finally {
            decompiler.dispose();
        }
    }
}
