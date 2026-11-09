// Audit actual flow references; do not bind a shared boot dispatch to one overlay.
// @category CoolBoarders2
import ghidra.app.script.GhidraScript;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.address.AddressSet;
import com.google.gson.*;
import java.nio.file.*;
import java.util.*;

public class AuditOverlayReferences extends GhidraScript {
    public void run() throws Exception {
        JsonObject report = new JsonObject();
        Path path = Path.of(getScriptArgs()[0]);
        var plan = JsonParser.parseString(Files.readString(path.getParent().resolve("plan.json"))).getAsJsonObject();
        report.addProperty("run_id", plan.get("run_id").getAsString());
        JsonObject counts = new JsonObject();
        JsonArray examples = new JsonArray(), bootCalls = new JsonArray(), unresolved = new JsonArray();
        int local = 0, resident = 0, wrongSpace = 0, other = 0;
        var defaults = currentProgram.getAddressFactory().getDefaultAddressSpace();
        var listing = currentProgram.getListing();
        for (MemoryBlock block : currentProgram.getMemory().getBlocks()) {
            if (!block.isOverlay()) continue;
            var iterator = listing.getInstructions(new AddressSet(block.getStart(), block.getEnd()), true);
            int localCount = 0, residentCount = 0, ambiguous = 0;
            while (iterator.hasNext()) {
                monitor.checkCancelled();
                Instruction instruction = iterator.next();
                for (var reference : instruction.getReferencesFrom()) {
                    if (!reference.getReferenceType().isFlow()) continue;
                    var to = reference.getToAddress();
                    if (to.getAddressSpace().equals(block.getStart().getAddressSpace())) { local++; localCount++; }
                    else if (to.getAddressSpace().equals(defaults) && to.getOffset() >= 0x80010000L && to.getOffset() < 0x8005AB68L) {
                        resident++; residentCount++;
                        var function = getFunctionAt(to);
                        if (examples.size() < 20 && function != null && !function.getName().startsWith("FUN_") && !function.getName().startsWith("func_")) {
                            JsonObject row = new JsonObject();
                            row.addProperty("from", instruction.getAddress().toString());
                            row.addProperty("to", to.toString()); row.addProperty("name", function.getName());
                            examples.add(row);
                        }
                    } else if (to.getAddressSpace().equals(defaults) && to.getOffset() >= block.getStart().getOffset() && to.getOffset() <= block.getEnd().getOffset()) {
                        wrongSpace++; ambiguous++;
                        JsonObject row = new JsonObject(); row.addProperty("from", instruction.getAddress().toString()); row.addProperty("to", to.toString()); unresolved.add(row);
                    } else other++;
                }
            }
            JsonObject row = new JsonObject(); row.addProperty("local_flow_references", localCount);
            row.addProperty("resident_flow_references", residentCount); row.addProperty("local_targets_in_default_space", ambiguous);
            counts.add(block.getName(), row);
        }
        var boot = listing.getInstructions(new AddressSet(toAddr(0x800120F4L), toAddr(0x800453A8L)), true);
        while (boot.hasNext()) {
            Instruction instruction = boot.next();
            for (var reference : instruction.getReferencesFrom()) {
                if (!reference.getReferenceType().isFlow()) continue;
                var to = reference.getToAddress();
                if (!to.getAddressSpace().equals(defaults) || to.getOffset() < 0x8005AB68L || to.getOffset() >= 0x80200000L) continue;
                JsonObject row = new JsonObject(); row.addProperty("from", instruction.getAddress().toString()); row.addProperty("to", to.toString());
                JsonArray candidates = new JsonArray();
                for (MemoryBlock block : currentProgram.getMemory().getBlocks()) {
                    if (!block.isOverlay() || to.getOffset() < block.getStart().getOffset() || to.getOffset() > block.getEnd().getOffset()) continue;
                    var destination = block.getStart().getAddressSpace().getAddress(to.getOffset());
                    if (getFunctionAt(destination) != null) candidates.add(destination.toString());
                }
                row.add("overlay_function_candidates", candidates); bootCalls.add(row);
            }
        }
        report.addProperty("same_overlay_flow_references", local); report.addProperty("resident_flow_references", resident);
        report.addProperty("local_targets_in_default_space", wrongSpace); report.addProperty("other_flow_references", other);
        report.add("overlays", counts); report.add("resident_symbol_examples", examples);
        report.add("boot_to_overlay_region", bootCalls); report.add("unresolved_local_references", unresolved);
        report.addProperty("scope", "Direct flow references only; unresolved indirect calls and computed pointers require context. No reference edits.");
        Files.writeString(path, new GsonBuilder().setPrettyPrinting().create().toJson(report) + "\n");
        println("Overlay references: local=" + local + ", resident=" + resident + ", wrong-space=" + wrongSpace + ", boot sites=" + bootCalls.size());
    }
}
