// Read-only decompiler/listing evidence for selected resident or overlay functions.
// @category CoolBoarders2
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import java.nio.file.*;

public class ExportFunctionTrace extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        Path directory = Path.of(args[0]); Files.createDirectories(directory);
        DecompInterface decompiler = new DecompInterface();
        try {
            if (!decompiler.openProgram(currentProgram)) throw new IllegalStateException(decompiler.getLastMessage());
            for (int i = 1; i < args.length; i++) {
                String[] parts = args[i].split("::");
                var space = currentProgram.getAddressFactory().getAddressSpace(parts[0]);
                var address = space.getAddress(Long.parseUnsignedLong(parts[1], 16));
                var function = getFunctionAt(address);
                if (function == null) throw new IllegalStateException("No function at " + address);
                var result = decompiler.decompileFunction(function, 60, monitor);
                if (!result.decompileCompleted() || result.getDecompiledFunction() == null)
                    throw new IllegalStateException("Decompile failed: " + address + " " + result.getErrorMessage());
                StringBuilder text = new StringBuilder("// " + address + " " + function.getName() + "\n");
                text.append(result.getDecompiledFunction().getC()).append("\n/* Listing evidence:\n");
                var instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
                while (instructions.hasNext()) {
                    var instruction = instructions.next();
                    text.append(instruction.getAddress()).append(" ").append(instruction).append("\n");
                }
                text.append("*/\n");
                Files.writeString(directory.resolve(parts[0] + "_" + parts[1] + ".txt"), text);
                println("Trace exported: " + address);
            }
        } finally { decompiler.dispose(); }
    }
}
