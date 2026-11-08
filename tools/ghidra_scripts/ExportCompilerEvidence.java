// Export analysis metadata and small functions for compiler fingerprinting.
// @category CoolBoarders2
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.framework.options.Options;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import java.nio.file.Files;
import java.nio.file.Path;

public class ExportCompilerEvidence extends GhidraScript {
    public void run() throws Exception {
        Path directory = Path.of(getScriptArgs()[0]);
        Files.createDirectories(directory);
        StringBuilder metadata = new StringBuilder();
        for (String group : currentProgram.getOptionsNames()) {
            Options options = currentProgram.getOptions(group);
            for (String name : options.getOptionNames()) {
                if (group.toLowerCase().contains("program") || name.toLowerCase().contains("psy")) {
                    metadata.append(group).append(" / ").append(name).append(" = ")
                        .append(options.getValueAsString(name)).append("\n");
                }
            }
        }
        Files.writeString(directory.resolve("metadata.txt"), metadata);
        StringBuilder functions = new StringBuilder("address\tname\tbytes\n");
        FunctionIterator iterator = currentProgram.getFunctionManager().getFunctions(true);
        while (iterator.hasNext()) {
            Function f = iterator.next();
            functions.append(f.getEntryPoint()).append("\t").append(f.getName()).append("\t")
                .append(f.getBody().getNumAddresses()).append("\n");
        }
        Files.writeString(directory.resolve("functions.tsv"), functions);
        DecompInterface decompiler = new DecompInterface();
        try {
            decompiler.openProgram(currentProgram);
            long[] probes = {0x80013a20L, 0x80013b30L, 0x80014388L, 0x800143e0L, 0x80014430L};
            for (long address : probes) {
                Function f = getFunctionAt(toAddr(address));
                if (f == null) continue;
                DecompileResults result = decompiler.decompileFunction(f, 30, monitor);
                if (result.decompileCompleted() && result.getDecompiledFunction() != null) {
                    Files.writeString(directory.resolve(String.format("%08x.c", address)),
                        result.getDecompiledFunction().getC());
                }
            }
        }
        finally { decompiler.dispose(); }
        println("Compiler evidence exported to " + directory);
    }
}
