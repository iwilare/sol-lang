import java.util.*;

class SolArguments {
    String prompt = "> ";
    boolean SolInitialization = true;
    boolean REPL = false;
    boolean REPLPrint = true;

    ArrayList<String> files = new ArrayList<String>();
    SolArguments(String arguments[]) {
        for(String argument : arguments)
            if(argument.charAt(0) != '-')
                files.add(argument);
            else if(argument.equals("-no-init"))
                SolInitialization = false;
    }
    public ArrayList<String> getFiles() { return files; }
    public boolean evalSolInitialization() { return SolInitialization; }

}
