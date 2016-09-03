import java.util.*;

class SolArguments {
    String prompt = "> ";
    boolean SolBase = true;
    boolean REPL = false;
    boolean REPLPrint = true;
    ArrayList<String> files = new ArrayList<String>();
    SolArguments(String arguments[]) {
	for(String argument : arguments)
	    if(argument.charAt(0) != '-')
		files.add(argument);	    
    }
    public ArrayList<String> getFiles() {
	return files;
    }
}
