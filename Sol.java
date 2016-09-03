import java.io.*;
import java.util.*;

public class Sol {
    Sol clazz;
    HashMap<String,Sol> variables;
    Object data;
    Sol() {
	this.variables = new HashMap<String,Sol>();
    }
    Sol(Reference<Sol> clazz) {
	this.clazz = clazz.get();
	this.variables = new HashMap<String,Sol>();
    }
    Sol(Sol clazz) {
	this.clazz = clazz;
	this.variables = new HashMap<String,Sol>();
    }
    Sol(Reference<Sol> clazz, Object data) {
	this(clazz.get(), data);
    }
    Sol(Sol clazz, Object data) {
	this.clazz = clazz;
	this.variables = new HashMap<String,Sol>();
	this.data = data;
    }
    public Sol getClazz() { return this.clazz; }
    public void setClass(Sol clazz) { this.clazz = clazz; }
    public boolean hasVariable(String variable) { return this.variables.containsKey(variable); }
    public Sol getVariable(String variable) { return this.variables.get(variable); }
    public void setVariable(String variable, Sol value) { this.variables.put(variable, value); }
    public void defineVariable(String variable) { this.variables.put(variable, null); }
    public void defineVariable(String variable, Sol value) { this.variables.put(variable, value); }
    public Object getData() { return this.data; }
    public boolean typecheck(Reference<Sol> clazz) {
	return this.clazz == clazz.get();
    }
    public Object getData(Reference<Sol> clazz) throws RuntimeException {
	if(this.clazz == clazz.get()) return this.data;
	else throw new RuntimeException("Typecheck failed."); // Check
    }
    public void setData(Object data) { this.data = data; }

    public static String getClassTypeName(Sol clazz) throws Exception {
	return (String)(send(clazz, "toString").getData(Classes.String));
    }
    public static String getTypeName(Sol object) throws Exception {
	return getClassTypeName(object == null ? Classes.Nothing.get() : object.clazz);
    }
    
    public static Sol sendWER(Sol self, Message message, Sol[] arguments) throws EvaluationRequest, Exception {
	if(self == null)
	    return sendWER(self, message, arguments, Classes.Nothing.get());
	else
	    return sendWER(self, message, arguments, self.clazz);
    }
    public static Sol sendWER(Sol self, Message message, Sol[] arguments, Sol startingClass) throws EvaluationRequest, Exception {
	if(startingClass == null)
	    throw new RuntimeException("Attempting to send " + message.toString() + " to a classless object.");
	for(Sol clazz = startingClass; clazz != null; clazz = clazz.getVariable("superclass")) {
	    Sol methodsObject = clazz.getVariable("methods");
	    HashMap<Message,Sol> methodMap = (HashMap<Message,Sol>)methodsObject.getData(Classes.MessageDictionary);
	    if(methodMap.containsKey(message)) {
		Sol method = methodMap.get(message);
		if(method == null)
		    break;
		else if(method.typecheck(Classes.BuiltinMethod)) {
		    if(message.getParametersNumber() != arguments.length)
			throw new RuntimeException("Attempting to send " + message.toString() + " to an instance of " +
						   getTypeName(self) + " with " +
						   Integer.toString(arguments.length) + " arguments, expected " +
						   Integer.toString(message.getParametersNumber()) + ".");
		    SolFunction function = (SolFunction)method.getData();
		    return function.call(self, arguments);
		} else if(method.typecheck(Classes.Lambda)) {
		    Lambda lambda = (Lambda)method.getData();
		    lambda.eval(self, arguments, clazz);
		} else if(method.typecheck(Classes.BuiltinMethod)) {
		    Sol argumentsObject = new Sol(Classes.Vector, new ArrayList<Sol>(Arrays.asList(arguments)));
		    return sendWER(method, new KeywordMessage(new String[]{"valueIn","with"}),
				   new Sol[]{self, argumentsObject});
		}
	    }
	}
	//throw new RuntimeException("Could not understand \"" + message.toString() + "\".");
	if((self == null ? Classes.Nothing.get() : self.clazz) == startingClass)
	    return sendWER(self, new KeywordMessage(new String[]{"doesNotUnderstand","with"}),
			   new Sol[]{new Sol(Classes.Message, message),
				     new Sol(Classes.Vector, new ArrayList<Sol>(Arrays.asList(arguments)))});
	else
	    return sendWER(self, new KeywordMessage(new String[]{"doesNotUnderstand","with","as"}),
			   new Sol[]{new Sol(Classes.Message, message),
				     new Sol(Classes.Vector, new ArrayList<Sol>(Arrays.asList(arguments))),
				     startingClass});
    }
    public static Sol send(Sol self, Message message, Sol[] arguments, Sol startingClass) throws Exception {
	try {
	    return sendWER(self, message, arguments, startingClass);
	} catch(EvaluationRequest evaluationRequest) {
	    return evaluationRequest.eval();
	}
    }
    public static Sol send(Sol self, Message message, Sol startingClass) throws Exception {
	return send(self, message, new Sol[]{}, startingClass);
    }
    public static Sol send(Sol self, Message message, Sol[] arguments) throws Exception {
	return send(self, message, arguments, false);
    }
    public static Sol superSend(Sol self, Message message, Sol[] arguments) throws Exception {
	return send(self, message, arguments, true);
    }
    public static Sol send(Sol self, Message message, Sol[] arguments, boolean superMessage) throws Exception {
	if(self == null)
	    return send(self, message, arguments, Classes.Nothing.get());
	else if(superMessage)
	    if(self.clazz == null)
		throw new Exception("Attempting to send " + message.toString() + " to a classless object.");
	    else
		return send(self, message, arguments, self.clazz.getVariable("superclass"));
	else
	    return send(self, message, arguments, self.clazz);	 
    }
    public static Sol send(Sol self, String message) throws Exception {
	return send(self, new UnaryMessage(message), new Sol[]{});
    }
    public static Sol send(Sol self, String keywords[], Sol[] arguments) throws Exception {
	return send(self, new KeywordMessage(keywords), arguments);
    }
    public static Sol superSend(Sol self, String message) throws Exception {
	return superSend(self, new UnaryMessage(message), new Sol[]{});
    }
    public static Sol superSend(Sol self, String[] keywords, Sol[] arguments) throws Exception {
	return superSend(self, new KeywordMessage(keywords), arguments);
    }
    //-----------------------------------------------------------------------------------------------------------
    public static int Version = 1;
    
    public static void printGreetings() {
	System.out.println("                 Sol");
	System.out.println("              Version " + Version);
	System.out.println("       Copyright (C) 2016 Iwilare");
	System.out.println();
    }
    public static Sol evalStream(Environment environment, CharacterStream stream) throws Exception {
	System.err.println("-------------- Tokenizer -------------");
		
	Tokenizer tokenizer = new Tokenizer(stream);
	
	Tokenizer.State tokenizerState = tokenizer.saveState();
	while(tokenizer.hasTokens()) {
	    System.err.println(tokenizer.getToken().toCompleteString());
	    tokenizer.nextToken();
	}
	tokenizer.restoreState(tokenizerState);
		
	System.err.println("--------------- Parser ---------------");
		
	Parser parser = new Parser(tokenizer);
	Atom atom = parser.parse();

	if(atom != null)
	    System.err.println(atom.toString());
	System.err.println("------------- Evaluation -------------");

	return Evaluator.eval(atom, environment);
    }
    public static void main(String[] args) {
	printGreetings();

	Environment environment = new Environment();
	SolArguments arguments = new SolArguments(args);
	
	Runtime.initialize();
	Classes.initialize();
	Classes.populate(environment);

	try { evalStream(environment, new FileStream("Sol.sol")); }
	catch(SolException c) { System.err.println(c.getMessage()); }
	catch(IOException c)  { System.err.println("Unable to find initalization file Sol.sol."); }
	catch(Exception c)    { c.printStackTrace(); }
	
	for(String filename : arguments.getFiles())
	    try { evalStream(environment, new FileStream(filename)); }
	    catch(SolException c) { System.err.println(c.getMessage()); }
	    catch(IOException c)  { System.err.println("Unable to find \"" + filename + "\""); }
	    catch(Exception c)    { c.printStackTrace(); }
		
	while(true)
	    try {
		System.out.print("> ");
		String line = new Scanner(System.in).nextLine();
		CharacterStream stream = new StringStream(line);
		Sol value = evalStream(environment, stream);
		if(value != null)
		    Sol.send(Sol.send(value, "toString"), "printLine");
	    }
	    catch(SolException c) { System.err.println(c.getMessage()); }
	    catch(Exception c)    { c.printStackTrace(); }
    }
}
