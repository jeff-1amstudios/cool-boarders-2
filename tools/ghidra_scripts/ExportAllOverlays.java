// Refresh inventory from the existing native overlay blocks without reanalysis.
// @category CoolBoarders2
import ghidra.app.script.GhidraScript;
import com.google.gson.JsonParser;
import java.nio.file.*;

public class ExportAllOverlays extends GhidraScript {
    public void run() throws Exception {
        String directory = getScriptArgs()[0];
        var plan = JsonParser.parseString(Files.readString(Path.of(directory, "plan.json"))).getAsJsonObject();
        for (String name : plan.getAsJsonObject("programs").keySet())
            runScript("ExportOverlayInventory.java", new String[]{directory, name});
    }
}
