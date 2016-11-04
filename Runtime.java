import java.io.*;
import java.nio.*;
import java.net.*;
import java.util.*;
import java.util.regex.*;

public class Runtime {
    public static void initialize() {
	Classes.ObjectMethods.put(Message.utilityParse("printeronos"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    System.out.println("-------- " + self + " | " + Sol.getClass(self) + " -------");
		    for(Map.Entry<String,Sol> variable : self.getVariables().entrySet())
			System.out.println(variable.getKey() + " --> " + variable.getValue());
		    System.out.println("--------------------------------------");
		    return null;
		}});
	Classes.ObjectMethods.put(Message.utilityParse("send:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    return Sol.send(self, message, new Sol[]{});
		}});
	Classes.ObjectMethods.put(Message.utilityParse("send:with:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    ArrayList<Sol> actualArguments = (ArrayList<Sol>)arguments[1].getData(Classes.Vector);
		    Sol[] argumentsArray = actualArguments.toArray(new Sol[actualArguments.size()]);
		    return Sol.send(self, message, argumentsArray);
		}});
        Classes.ObjectMethods.put(Message.utilityParse("send:with:as:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    ArrayList<Sol> actualArguments = (ArrayList<Sol>)arguments[1].getData(Classes.Vector);
		    Sol[] argumentsArray = actualArguments.toArray(new Sol[actualArguments.size()]);
		    return Sol.send(self, message, argumentsArray, arguments[2]);
		}});
	Classes.ObjectMethods.put(Message.utilityParse("send:as:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    return Sol.send(self, message, arguments[1]);
		}});
	Classes.ObjectMethods.put(Message.utilityParse("superSend:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    return Sol.superSend(self, message, new Sol[]{});
		}});
	Classes.ObjectMethods.put(Message.utilityParse("superSend:with:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    ArrayList<Sol> actualArguments = (ArrayList<Sol>)arguments[1].getData(Classes.Vector);
		    Sol[] argumentsArray = actualArguments.toArray(new Sol[actualArguments.size()]);
		    return Sol.superSend(self, message, argumentsArray);
		}});
	Classes.ObjectMethods.put(Message.utilityParse("initialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return self;
		}});
	Classes.ObjectMethods.put(Message.utilityParse("class"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return Sol.getClass(self);
		}});
	Classes.ObjectMethods.put(Message.utilityParse("class:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    self.setClass(arguments[0]);
		    return self;
		}});
	Classes.ObjectMethods.put(Message.utilityParse("id"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Integer, self.hashCode());
		}});
	Classes.ObjectMethods.put(Message.utilityParse("=="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return self == arguments[0] ? Classes.True.get() : Classes.False.get();
		}});
	Classes.ObjectMethods.put(Message.utilityParse("!="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return self != arguments[0] ? Classes.True.get() : Classes.False.get();
		}});
	Classes.ObjectMethods.put(Message.utilityParse("class:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    self.setClass(arguments[0]);
		    return self;
		}});
	Classes.ObjectMethods.put(Message.utilityParse("print"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return Sol.send(self, "printString");
		}});
	Classes.ObjectMethods.put(Message.utilityParse("printString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String string = (String)Sol.send(self, "toString").getData(Classes.String);
		    System.out.print(string);
		    return self;
		}});
	Classes.ObjectMethods.put(Message.utilityParse("printLine"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Sol.send(self, "print");
		    System.out.println();
		    return self;
		}});
	Classes.ObjectMethods.put(Message.utilityParse("printStringLine"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Sol.send(self, "printString");
		    System.out.println();
		    return self;
		}});
	Classes.ObjectMethods.put(Message.utilityParse("doesNotUnderstand:with:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    ArrayList<Sol> actualArguments = (ArrayList<Sol>)arguments[1].getData(Classes.Vector);
		    Sol[] argumentsArray = actualArguments.toArray(new Sol[actualArguments.size()]);
		    String exceptionMessage =
			"An instance of " + Sol.getTypeName(self) + " " +
			"could not understand the message " + message.toString() + ".";
		    throw new RuntimeException(exceptionMessage);
		}});
        Classes.ObjectMethods.put(Message.utilityParse("doesNotUnderstand:with:as:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    ArrayList<Sol> actualArguments = (ArrayList<Sol>)arguments[1].getData(Classes.Vector);
		    Sol[] argumentsArray = actualArguments.toArray(new Sol[actualArguments.size()]);
		    Sol contextClass = arguments[2];
		    String exceptionMessage =
			"An instance of " + Sol.getTypeName(self) + " " + 
			"(" + Sol.getClassTypeName(contextClass) + ") " +
			"could not understand the message " + message.toString() + ".";
		    throw new RuntimeException(exceptionMessage);
		}});
	Classes.ObjectMethods.put(Message.utilityParse("understands:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    for(Sol clazz = Sol.getClass(self); clazz != null;
			clazz = clazz.getVariable("superclass")) {
			Sol methodsObject = clazz.getVariable("methods");
			if(methodsObject == null)
			    continue;
			HashMap<Message,Sol> methodMap =
			    (HashMap<Message,Sol>)methodsObject.getData(Classes.MessageDictionary);
			if(methodMap.containsKey(message))
			    return Classes.True.get();			
		    }
		    return Classes.False.get();
		}});
	Classes.ObjectMethods.put(Message.utilityParse("implementingClassOf:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    for(Sol clazz = Sol.getClass(self); clazz != null;
			clazz = clazz.getVariable("superclass")) {
			Sol methodsObject = clazz.getVariable("methods");
			if(methodsObject == null)
			    continue;
			HashMap<Message,Sol> methodMap =
			    (HashMap<Message,Sol>)methodsObject.getData(Classes.MessageDictionary);
			if(methodMap.containsKey(message))
			    return clazz;
		    }
		    return null;
		}});
	//----------------------------------------------------------------------
	Classes.NothingMethods.put(Message.utilityParse("send:"),
				  Classes.ObjectMethods.get(Message.utilityParse("send:")));
	Classes.NothingMethods.put(Message.utilityParse("send:with:"),
				  Classes.ObjectMethods.get(Message.utilityParse("send:with:")));
	Classes.NothingMethods.put(Message.utilityParse("send:with:as:"),
				  Classes.ObjectMethods.get(Message.utilityParse("send:with:as:")));
	Classes.NothingMethods.put(Message.utilityParse("superSend:"),
				  Classes.ObjectMethods.get(Message.utilityParse("superSend:")));
	Classes.NothingMethods.put(Message.utilityParse("superSend:as:"),
				  Classes.ObjectMethods.get(Message.utilityParse("superSend:as:")));
	Classes.NothingMethods.put(Message.utilityParse("doesNotUnderstand:with:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    ArrayList<Sol> actualArguments = (ArrayList<Sol>)arguments[1].getData(Classes.Vector);
		    Sol[] argumentsArray = actualArguments.toArray(new Sol[actualArguments.size()]);
		    throw new RuntimeException("Nothing could not understand the message " + message.toString() + ".");
		}});
        Classes.NothingMethods.put(Message.utilityParse("doesNotUnderstand:with:as:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    ArrayList<Sol> actualArguments = (ArrayList<Sol>)arguments[1].getData(Classes.Vector);
		    Sol[] argumentsArray = actualArguments.toArray(new Sol[actualArguments.size()]);
		    Sol contextClass = arguments[2];
		    String exceptionMessage =
			"Nothing " +
			"(" + Sol.getClassTypeName(contextClass) + ") " +
			"could not understand the message " + message.toString() + ".";
		    throw new RuntimeException(exceptionMessage);
		}});
	Classes.NothingMethods.put(Message.utilityParse("understands:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    return Classes.NothingMethods.containsKey(message) ? Classes.True.get() : Classes.False.get();
		}});
	Classes.NothingMethods.put(Message.utilityParse("=="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return arguments[0] == null ? Classes.True.get() : Classes.False.get();
		}});
	Classes.NothingMethods.put(Message.utilityParse("!="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return arguments[0] != null ? Classes.True.get() : Classes.False.get();
		}});	
	Classes.NothingMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.String, "nothing");
		}});	
	//----------------------------------------------------------------------
	Classes.MetaclassClassMethods.put(Message.utilityParse("new:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Sol metaclass = Sol.send(self, "new");
		    metaclass.setVariable("uniqueInstance", arguments[0]);
		    return metaclass;
		}});		
	//----------------------------------------
	Classes.MetaclassMethods.put(Message.utilityParse("initialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    self.setVariable("uniqueInstance", null);
		    return self;
		}});
	Classes.MetaclassMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Sol uniqueInstance = self.getVariable("uniqueInstance");
		    String uniqueInstanceString =
			Sol.send(uniqueInstance, new String[]{"implementingClassOf"},
				 new Sol[]{new Sol(Classes.Message, new UnaryMessage("toString"))})
			== Classes.Object.get() ? "<Anonymous object>" :
			(String)Sol.send(uniqueInstance, "toString").getData(Classes.String);
		    return new Sol(Classes.String, uniqueInstanceString + " class");
		}});
	//----------------------------------------------------------------------
	Classes.ClassMethods.put(Message.utilityParse("initialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Sol metaclass = Sol.send(Classes.Metaclass.get(), new String[]{"new"}, new Sol[]{self});
		    self.setClass(metaclass);
		    metaclass.setVariable("superclass", Sol.getClass(self.getVariable("superclass")));
		    self.setVariable("name", new Sol(Classes.String, "<Anonymous class>"));
		    return self;
		}});
	Classes.DescriptionMethods.put(Message.utilityParse("basicNew"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(self);
		}});
	Classes.ClassMethods.put(Message.utilityParse("new"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Sol object = new Sol(self);
		    LinkedList<Sol> classes = new LinkedList<Sol>();
		    for(Sol clazz = self; clazz != null; clazz = clazz.getVariable("superclass"))
			classes.addFirst(clazz);
		    for(Sol clazz : classes) {
			ArrayList<Sol> variables =
			    (ArrayList<Sol>)clazz.getVariable("variables").getData(Classes.Vector);
			for(Sol variable : variables) {
			    Message message = (Message)variable.getData(Classes.Message);
			    if(message instanceof UnaryMessage) {
				String variableName = ((UnaryMessage)message).getUnaryMessage();
				object.defineVariable(variableName);
			    } else
				throw new RuntimeException("Expected valid variable symbol.");
			}
			Sol.send(object, new UnaryMessage("initialize"), clazz);
		    }
		    return object;
		}});
	Classes.ClassMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return self.getVariable("name");
		}});
	//----------------------------------------------------------------------
	Classes.DescriptionMethods.put(Message.utilityParse("initialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    self.setVariable("superclass", Classes.Object.get());
		    self.setVariable("methods", new Sol(Classes.MessageDictionary, new HashMap<Message,Sol>()));
		    self.setVariable("variables", new Sol(Classes.Vector, new ArrayList<Sol>()));
		    return self;
		}});
	Classes.DescriptionMethods.put(Message.utilityParse("method:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return Sol.send(self.getVariable("methods"),
				    new String[]{"at"}, new Sol[]{arguments[0]}); 
		}});
	Classes.DescriptionMethods.put(Message.utilityParse("method:set:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Sol.send(self.getVariable("methods"),
			     new String[]{"at","set"}, new Sol[]{arguments[0], arguments[1]});
		    return self;
		}});
	Classes.DescriptionMethods.put(Message.utilityParse("defineAccessors"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return Sol.send(self, new String[]{"defineAccessorsFor"},
				    new Sol[]{self.getVariable("variables")});
		}});
	Classes.DescriptionMethods.put(Message.utilityParse("defineAccessorsFor:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Sol variablesObject = arguments[0];
		    Sol methodsObject = self.getVariable("methods");
		    ArrayList<Sol> variables = (ArrayList<Sol>)variablesObject.getData(Classes.Vector);
		    HashMap<Message,Sol> methodDictionary = (HashMap<Message,Sol>)methodsObject.getData(Classes.MessageDictionary);
		    String[] constructorKeywords = new String[variables.size()];
		    for(int i=0; i<variables.size(); i++) {
			Message message = (Message)variables.get(i).getData(Classes.Message);
			if(!(message instanceof UnaryMessage))
			    throw new RuntimeException("Expected valid unary message in class variables");
			UnaryMessage variable = (UnaryMessage)message;
			methodDictionary.put(variable,
					     Sol.send(variables.get(i), "asGetMethod"));
			methodDictionary.put(variable.toKeywordMessage(),
					     Sol.send(variables.get(i), "asSetMethod"));
			constructorKeywords[i] = variable.getUnaryMessage();
		    }
		    Message constructorMessage = new KeywordMessage(constructorKeywords);
		    methodDictionary.put(constructorMessage,
					 Sol.send(new Sol(Classes.Message, constructorMessage), "asSetMethod"));
		    return self;
		}});
	Classes.DescriptionMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return self.getVariable("name");
		}});
	//----------------------------------------------------------------------
	Classes.LambdaMethods.put(Message.utilityParse("valueIn:with:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Lambda lambda = (Lambda)self.getData();
		    Sol selfObject = arguments[0];
		    Sol argumentsObject = arguments[1];
		    ArrayList<Sol> actualArguments = (ArrayList<Sol>)argumentsObject.getData(Classes.Vector);
		    Sol[] argumentsArray = actualArguments.toArray(new Sol[actualArguments.size()]);
		    lambda.eval(selfObject, argumentsArray);
		    return null;
		}});
	Classes.LambdaMethods.put(Message.utilityParse("valueIn:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Lambda lambda = (Lambda)self.getData();
		    Sol selfObject = arguments[0];
		    lambda.eval(selfObject);
		    return null;
		}});
	Classes.LambdaMethods.put(Message.utilityParse("values:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Lambda lambda = (Lambda)self.getData();
		    Sol argumentsObject = arguments[0];
		    ArrayList<Sol> actualArguments = (ArrayList<Sol>)argumentsObject.getData(Classes.Vector);
		    Sol[] argumentsArray = actualArguments.toArray(new Sol[actualArguments.size()]);
		    lambda.eval(argumentsArray);
		    return null;
		}});
	Classes.LambdaMethods.put(Message.utilityParse("value:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Lambda lambda = (Lambda)self.getData();
		    lambda.eval(new Sol[]{arguments[0]});
		    return null;
		}});
	Classes.LambdaMethods.put(Message.utilityParse("value"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Lambda lambda = (Lambda)self.getData();
		    lambda.eval();
		    return null;
		}});
	//----------------------------------------------------------------------
	Classes.BuiltinMethodMethods.put(Message.utilityParse("valueIn:with:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolFunction function = (SolFunction)self.getData();
		    Sol selfObject = arguments[0];
		    Sol argumentsObject = arguments[1];
		    ArrayList<Sol> actualArguments = (ArrayList<Sol>)argumentsObject.getData(Classes.Vector);
		    Sol[] argumentsArray = actualArguments.toArray(new Sol[actualArguments.size()]);
		    return function.call(selfObject, argumentsArray);
		}});
	Classes.BuiltinMethodMethods.put(Message.utilityParse("valueIn:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolFunction function = (SolFunction)self.getData();
		    Sol selfObject = arguments[0];
		    return function.call(selfObject);
		}});
	Classes.BuiltinMethodMethods.put(Message.utilityParse("values:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolFunction function = (SolFunction)self.getData();
		    Sol argumentsObject = arguments[0];
		    ArrayList<Sol> actualArguments = (ArrayList<Sol>)argumentsObject.getData(Classes.Vector);
		    Sol[] argumentsArray = actualArguments.toArray(new Sol[actualArguments.size()]);
		    return function.call(argumentsArray);
		}});
	Classes.BuiltinMethodMethods.put(Message.utilityParse("value:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolFunction function = (SolFunction)self.getData();
		    return function.call(new Sol[]{arguments[0]});
		}});
	Classes.BuiltinMethodMethods.put(Message.utilityParse("value"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolFunction function = (SolFunction)self.getData();
		    return function.call();
		}});
	//----------------------------------------------------------------------
	Classes.MessageDictionaryMethods.put(Message.utilityParse("initialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    self.setData(new HashMap<Message,Sol>());
		    return self;
		}});
	Classes.MessageDictionaryMethods.put(Message.utilityParse("at:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    HashMap<Message,Sol> dictionary = (HashMap<Message,Sol>)self.getData();
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    return dictionary.get(message);
		}});
	Classes.MessageDictionaryMethods.put(Message.utilityParse("at:set:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    HashMap<Message,Sol> dictionary = (HashMap<Message,Sol>)self.getData();
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    return dictionary.put(message, arguments[1]);
		}});
	Classes.MessageDictionaryMethods.put(Message.utilityParse("remove:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    HashMap<Message,Sol> dictionary = (HashMap<Message,Sol>)self.getData();
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    dictionary.remove(message);
		    return self;
		}});
	Classes.MessageDictionaryMethods.put(Message.utilityParse("has:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    HashMap<Message,Sol> dictionary = (HashMap<Message,Sol>)self.getData();
		    Message message = (Message)arguments[0].getData(Classes.Message);
		    return dictionary.containsKey(message) ? Classes.True.get() : Classes.False.get();
		}});
	Classes.MessageDictionaryMethods.put(Message.utilityParse("size"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    HashMap<Message,Sol> dictionary = (HashMap<Message,Sol>)self.getData();
		    return new Sol(Classes.Integer, dictionary.size());
		}});
	Classes.MessageDictionaryMethods.put(Message.utilityParse("do:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    HashMap<Message,Sol> dictionary = (HashMap<Message,Sol>)self.getData();
		    for(Map.Entry<Message, Sol> entry : dictionary.entrySet()) {
			Sol keyObject = new Sol(Classes.Message, entry.getKey());
			ArrayList<Sol> lambdaArguments = new ArrayList<Sol>();
			lambdaArguments.add(keyObject);
			lambdaArguments.add(entry.getValue());	
			Sol.send(arguments[0], new String[]{"values"},
				 new Sol[]{new Sol(Classes.Vector, lambdaArguments)});
		    }
		    return null;
		}});
	Classes.MessageDictionaryMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    HashMap<Message,Sol> dictionary = (HashMap<Message,Sol>)self.getData();
		    boolean firstElement = true;
		    String s = "{";
		    for(Map.Entry<Message, Sol> entry : dictionary.entrySet()) {
			s += (firstElement ? "" : ", ") + entry.getKey().toString() + " -> " + 
			    (String)(Sol.send(entry.getValue(), "toString").getData(Classes.String));
			firstElement = false;
		    }
		    return new Sol(Classes.String, s + "}");
		}});
	//----------------------------------------------------------------------
	Classes.VectorMethods.put(Message.utilityParse("initialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    self.setData(new ArrayList<Sol>());
		    return self;
		}});
	Classes.VectorMethods.put(Message.utilityParse("at:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    ArrayList<Sol> vector = (ArrayList<Sol>)self.getData();
		    int index = (int)arguments[0].getData(Classes.Integer);
		    return vector.get(index);
		}});
	Classes.VectorMethods.put(Message.utilityParse("at:set:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    ArrayList<Sol> vector = (ArrayList<Sol>)self.getData();
		    int index = (int)arguments[0].getData(Classes.Integer);
		    return vector.set(index, arguments[1]);
		}});
	Classes.VectorMethods.put(Message.utilityParse("push:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    ArrayList<Sol> vector = (ArrayList<Sol>)self.getData();
		    vector.add(arguments[0]);
		    return self;
		}});
	Classes.VectorMethods.put(Message.utilityParse("push:at:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    ArrayList<Sol> vector = (ArrayList<Sol>)self.getData();
		    int index = (int)arguments[0].getData(Classes.Integer);
		    vector.add(index, arguments[1]);
		    return self;
		}});
	Classes.VectorMethods.put(Message.utilityParse("pop"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    ArrayList<Sol> vector = (ArrayList<Sol>)self.getData();
		    return vector.remove(vector.size() - 1);
		}});
	Classes.VectorMethods.put(Message.utilityParse("popAt:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    ArrayList<Sol> vector = (ArrayList<Sol>)self.getData();
		    int index = (int)arguments[0].getData(Classes.Integer);
		    return vector.remove(index);
		}});
	Classes.VectorMethods.put(Message.utilityParse("size"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    ArrayList<Sol> vector = (ArrayList<Sol>)self.getData();
		    return new Sol(Classes.Integer, vector.size());
		}});
	Classes.VectorMethods.put(Message.utilityParse("do:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    ArrayList<Sol> vector = (ArrayList<Sol>)self.getData();
		    for(Sol value : vector)
			Sol.send(arguments[0], new String[]{"value"}, new Sol[]{value});
		    return self;
		}});
	Classes.VectorMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    ArrayList<Sol> vector = (ArrayList<Sol>)self.getData();
		    if(vector.size() == 0)
			return new Sol(Classes.String, "[]");
		    String s = "[";
		    for(int i=0; i<vector.size()-1; i++) 
			s += (String)(Sol.send(vector.get(i), "toString").getData(Classes.String)) + ",";
		    s += (String)(Sol.send(vector.get(vector.size()-1), "toString").getData(Classes.String));
		    return new Sol(Classes.String, s + "]");
		}});
	//----------------------------------------------------------------------
	Classes.IntegerMethods.put(Message.utilityParse("initialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    self.setData(0);
		    return self;
		}});
	Classes.IntegerMethods.put(Message.utilityParse("toDouble"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Double, (double)(int)self.getData());
		}});
	Classes.IntegerMethods.put(Message.utilityParse("toCharacter"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Character, (char)(int)self.getData());
		}});
	Classes.IntegerMethods.put(Message.utilityParse("toByte"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Byte, (byte)(int)self.getData());
		}});
	Classes.IntegerMethods.put(Message.utilityParse("serialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Bytes, ByteBuffer.allocate(4).putInt((int)self.getData()).array());
		}});
	Classes.IntegerMethods.put(Message.utilityParse("+"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    int a = (int)self.getData();
		    if(arguments[0].typecheck(Classes.Double))
			return new Sol(Classes.Double, a + (double)arguments[0].getData());
		    if(!arguments[0].typecheck(Classes.Integer))
			arguments[0] = Sol.send(arguments[0], "toInteger");
		    return new Sol(Classes.Integer, a + (int)arguments[0].getData(Classes.Integer));
		}});
	Classes.IntegerMethods.put(Message.utilityParse("-"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    int a = (int)self.getData();
		    if(arguments[0].typecheck(Classes.Double))
			return new Sol(Classes.Double, a - (double)arguments[0].getData());
		    if(!arguments[0].typecheck(Classes.Integer))
			arguments[0] = Sol.send(arguments[0], "toInteger");
		    return new Sol(Classes.Integer, a - (int)arguments[0].getData(Classes.Integer));
		}});
	Classes.IntegerMethods.put(Message.utilityParse("*"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    int a = (int)self.getData();
		    if(arguments[0].typecheck(Classes.Double))
			return new Sol(Classes.Double, a * (double)arguments[0].getData());
		    if(!arguments[0].typecheck(Classes.Integer))
			arguments[0] = Sol.send(arguments[0], "toInteger");
		    return new Sol(Classes.Integer, a * (int)arguments[0].getData(Classes.Integer));
		}});
	Classes.IntegerMethods.put(Message.utilityParse("/"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    int a = (int)self.getData();
		    if(arguments[0].typecheck(Classes.Double))
			return new Sol(Classes.Double, a / (double)arguments[0].getData());
		    if(!arguments[0].typecheck(Classes.Integer))
			arguments[0] = Sol.send(arguments[0], "toInteger");
		    return new Sol(Classes.Integer, a / (int)arguments[0].getData(Classes.Integer));
		}});
	Classes.IntegerMethods.put(Message.utilityParse("=="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    int a = (int)self.getData();
		    if(arguments[0].typecheck(Classes.Double))
			return a == (double)arguments[0].getData() ?
			    Classes.True.get() : Classes.False.get();
		    if(!arguments[0].typecheck(Classes.Integer))
			arguments[0] = Sol.send(arguments[0], "toInteger");
		    return a == (int)arguments[0].getData(Classes.Integer) ?
			Classes.True.get() : Classes.False.get();
		}});
	Classes.IntegerMethods.put(Message.utilityParse("!="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    int a = (int)self.getData();
		    if(arguments[0].typecheck(Classes.Double))
			return a != (double)arguments[0].getData() ?
			    Classes.True.get() : Classes.False.get();
		    if(!arguments[0].typecheck(Classes.Integer))
			arguments[0] = Sol.send(arguments[0], "toInteger");
		    return a != (int)arguments[0].getData(Classes.Integer) ?
			Classes.True.get() : Classes.False.get();
		}});
	Classes.IntegerMethods.put(Message.utilityParse("<"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    int a = (int)self.getData();
		    if(arguments[0].typecheck(Classes.Double))
			return a < (double)arguments[0].getData() ?
			    Classes.True.get() : Classes.False.get();
		    if(!arguments[0].typecheck(Classes.Integer))
			arguments[0] = Sol.send(arguments[0], "toInteger");
		    return a < (int)arguments[0].getData(Classes.Integer) ?
			Classes.True.get() : Classes.False.get();
		}});
	Classes.IntegerMethods.put(Message.utilityParse("negate"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Integer, -(int)self.getData());
		}});
	Classes.IntegerMethods.put(Message.utilityParse("next"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Integer, 1+(int)self.getData());
		}});
	Classes.IntegerMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.String, Integer.toString((int)self.getData()));
		}});
	//----------------------------------------------------------------------
	Classes.DoubleMethods.put(Message.utilityParse("initialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    self.setData(0.0);
		    return self;
		}});
	Classes.DoubleMethods.put(Message.utilityParse("toInteger"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Integer, (int)(double)self.getData());
		}});
	Classes.DoubleMethods.put(Message.utilityParse("serialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Bytes, ByteBuffer.allocate(8).putDouble((double)self.getData()).array());
		}});
	Classes.DoubleMethods.put(Message.utilityParse("+"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    double a = (double)self.getData();
		    if(!arguments[0].typecheck(Classes.Double))
			arguments[0] = Sol.send(arguments[0], "toDouble");
		    return new Sol(Classes.Double, a + (double)arguments[0].getData(Classes.Double));
		}});
	Classes.DoubleMethods.put(Message.utilityParse("-"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    double a = (double)self.getData();
		    if(!arguments[0].typecheck(Classes.Double))
			arguments[0] = Sol.send(arguments[0], "toDouble");
		    return new Sol(Classes.Double, a - (double)arguments[0].getData(Classes.Double));
		}});
	Classes.DoubleMethods.put(Message.utilityParse("*"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    double a = (double)self.getData();
		    if(!arguments[0].typecheck(Classes.Double))
			arguments[0] = Sol.send(arguments[0], "toDouble");
		    return new Sol(Classes.Double, a * (double)arguments[0].getData(Classes.Double));
		}});
	Classes.DoubleMethods.put(Message.utilityParse("/"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    double a = (double)self.getData();
		    if(!arguments[0].typecheck(Classes.Double))
			arguments[0] = Sol.send(arguments[0], "toDouble");
		    return new Sol(Classes.Double, a / (double)arguments[0].getData(Classes.Double));
		}});
	Classes.DoubleMethods.put(Message.utilityParse("=="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    double a = (double)self.getData();
		    if(!arguments[0].typecheck(Classes.Double))
			arguments[0] = Sol.send(arguments[0], "toDouble");
		    return a == (double)arguments[0].getData(Classes.Double) ?
			Classes.True.get() : Classes.False.get();
		}});
	Classes.DoubleMethods.put(Message.utilityParse("!="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    double a = (double)self.getData();
		    if(!arguments[0].typecheck(Classes.Double))
			arguments[0] = Sol.send(arguments[0], "toDouble");
		    return a != (double)arguments[0].getData(Classes.Double) ?
			Classes.True.get() : Classes.False.get();
		}});
	Classes.DoubleMethods.put(Message.utilityParse("<"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    double a = (double)self.getData();
		    if(!arguments[0].typecheck(Classes.Double))
			arguments[0] = Sol.send(arguments[0], "toDouble");
		    return a < (double)arguments[0].getData(Classes.Double) ?
			Classes.True.get() : Classes.False.get();
		}});
	Classes.DoubleMethods.put(Message.utilityParse("negate"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Double, -(double)self.getData());
		}});
	Classes.DoubleMethods.put(Message.utilityParse("next"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Double, 1+(double)self.getData());
		}});
	Classes.DoubleMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.String, Double.toString((double)self.getData()));
		}});	
	//----------------------------------------------------------------------
	Classes.CharacterMethods.put(Message.utilityParse("initialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    self.setData('\0');
		    return self;
		}});
	Classes.CharacterMethods.put(Message.utilityParse("ascii"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Integer, (int)(char)self.getData());
		}});
	Classes.CharacterMethods.put(Message.utilityParse("toInteger"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Integer, (int)(char)self.getData());
		}});
	Classes.CharacterMethods.put(Message.utilityParse("toByte"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Byte, (byte)(char)self.getData());
		}});
	Classes.CharacterMethods.put(Message.utilityParse("serialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Bytes, ByteBuffer.allocate(1).putChar((char)self.getData()).array());
		}});
	Classes.CharacterMethods.put(Message.utilityParse("+"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    char a = (char)self.getData();
		    if(arguments[0].typecheck(Classes.Integer))
			return new Sol(Classes.Character, (char)(a + (int)arguments[0].getData()));
		    else if(arguments[0].typecheck(Classes.String))
			return new Sol(Classes.String, a + (String)arguments[0].getData());
		    arguments[0] = Sol.send(arguments[0], "toString");
		    return new Sol(Classes.String, a + (String)arguments[0].getData(Classes.String));
		}});
	Classes.CharacterMethods.put(Message.utilityParse("next"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Character, (char)(1 + (char)self.getData()));
		}});
	Classes.CharacterMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.String, '\'' + Character.toString((char)self.getData()) + '\'');
		}});
	Classes.CharacterMethods.put(Message.utilityParse("printString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    System.out.print((char)self.getData());
		    return self;
		}});
	Classes.CharacterMethods.put(Message.utilityParse("=="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    char a = (char)self.getData();
		    return a == (char)arguments[0].getData(Classes.Character) ?
			Classes.True.get() : Classes.False.get();
		}});
	Classes.CharacterMethods.put(Message.utilityParse("!="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    char a = (char)self.getData();
		    return a != (char)arguments[0].getData(Classes.Character) ?
			Classes.True.get() : Classes.False.get();
		}});
	Classes.CharacterMethods.put(Message.utilityParse("<"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    char a = (char)self.getData();
		    return a < (char)arguments[0].getData(Classes.Character) ?
			Classes.True.get() : Classes.False.get();
		}});	
	//----------------------------------------------------------------------
	Classes.StringMethods.put(Message.utilityParse("initialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    self.setData("");
		    return self;
		}});
	Classes.StringMethods.put(Message.utilityParse("serialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Bytes, ((String)self.getData()).getBytes());
		}});
	Classes.StringMethods.put(Message.utilityParse("+"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String a = (String)self.getData();
		    if(arguments[0].typecheck(Classes.String))
			return new Sol(Classes.String, a + (String)arguments[0].getData());
		    else if(arguments[0].typecheck(Classes.Character))
			return new Sol(Classes.String, a + (char)arguments[0].getData());
		    arguments[0] = Sol.send(arguments[0], "toString");
		    return new Sol(Classes.String, a + (String)arguments[0].getData(Classes.String));
		}});
	/*Classes.StringMethods.put(Message.utilityParse("+="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String a = (String)self.getData();
		    if(arguments[0].typecheck(Classes.String))
			a.concat((String)arguments[0].getData());
		    else if(arguments[0].typecheck(Classes.Character))
			a.concat(Character.toString((char)arguments[0].getData()));
		    else {
			arguments[0] = Sol.send(arguments[0], "toString");
			a.concat((String)arguments[0].getData());
		    }
		    return self;
		    }});*/
	Classes.StringMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.String, "\"" + (String)self.getData() + "\"");
		}});
	Classes.StringMethods.put(Message.utilityParse("printString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    System.out.print((String)self.getData());
		    return self;
		}});
	Classes.StringMethods.put(Message.utilityParse("size"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Integer, ((String)self.getData()).length());
		}});
	Classes.StringMethods.put(Message.utilityParse("at:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    int index = (int)arguments[0].getData(Classes.Integer);
		    return new Sol(Classes.Character, (char)((String)self.getData()).charAt(index));
		}});
	Classes.StringMethods.put(Message.utilityParse("do:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String a = (String)self.getData();
		    for(char c : a.toCharArray())
			Sol.send(arguments[0], new String[]{"value"}, new Sol[]{
				new Sol(Classes.Character, c)});
		    return self;
		}});
	Classes.StringMethods.put(Message.utilityParse("=="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String a = (String)self.getData();
		    return a.equals((String)arguments[0].getData(Classes.String)) ?
			Classes.True.get() : Classes.False.get();
		}});
	Classes.StringMethods.put(Message.utilityParse("!="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String a = (String)self.getData();
		    return !a.equals((String)arguments[0].getData(Classes.String)) ?
			Classes.True.get() : Classes.False.get();
		}});
	Classes.StringMethods.put(Message.utilityParse("compareTo:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String a = (String)self.getData();
		    return new Sol(Classes.Integer, a.compareTo((String)arguments[0].getData(Classes.String)));
		}});
	//----------------------------------------------------------------------
	Classes.MessageMethods.put(Message.utilityParse("=="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message a = (Message)self.getData();
		    return a.hashCode() == ((Message)arguments[0].getData(Classes.Message)).hashCode() ?
			Classes.True.get() : Classes.False.get();
		}});
	Classes.MessageMethods.put(Message.utilityParse("!="), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message a = (Message)self.getData();
		    return a.hashCode() != ((Message)arguments[0].getData(Classes.Message)).hashCode() ?
			Classes.True.get() : Classes.False.get();
		}});
	Classes.MessageMethods.put(Message.utilityParse("asSetMethod"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message a = (Message)self.getData();
		    String[] variables = new String[]{};
		    if(a instanceof UnaryMessage)
			variables = new String[]{((UnaryMessage)a).getUnaryMessage()};
		    else if(a instanceof BinaryMessage)
			variables = new String[]{((BinaryMessage)a).getBinaryMessage()};
		    else if(a instanceof KeywordMessage)
			variables = ((KeywordMessage)a).getKeywords();
		    Atom[] sequence = new Atom[variables.length];
		    for(int i=0; i<variables.length; i++)
			sequence[i] = new AssignmentAtom(variables[i],
							 new IdentifierAtom(variables[i]));
		    return new Sol(Classes.Lambda, new Lambda(new SequenceAtom(sequence),
							      variables, null, null));
		}});
	Classes.MessageMethods.put(Message.utilityParse("asGetMethod"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Message a = (Message)self.getData();
		    if(!(a instanceof UnaryMessage))
			throw new RuntimeException("Expected valid unary message.");
		    return new Sol(Classes.Lambda, new Lambda(new IdentifierAtom(((UnaryMessage)a).getUnaryMessage()),
							      new String[]{}, null, null));
		}});
	Classes.MessageMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.String, "#" + ((Message)self.getData()).toString());
		}});
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	Classes.ThreadClassMethods.put(Message.utilityParse("new:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Lambda lambda = (Lambda)arguments[0].getData(Classes.Lambda);
		    return new Sol(Classes.Thread, new SolThread(lambda));
		}});
	Classes.ThreadClassMethods.put(Message.utilityParse("current"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Thread, Thread.currentThread());
		}});
	Classes.ThreadClassMethods.put(Message.utilityParse("run:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Lambda lambda = (Lambda)arguments[0].getData(Classes.Lambda);
		    new SolThread(lambda).start();
		    return null;
		}});
	//----------------------------------------
	Classes.ThreadMethods.put(Message.utilityParse("run"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Thread thread = (Thread)self.getData();
		    thread.start(); return null;
		}});	
	Classes.ThreadMethods.put(Message.utilityParse("sleep:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Thread thread = (Thread)self.getData();
		    int time = (int)arguments[0].getData(Classes.Integer);
		    thread.sleep(time); return null;
		}});	
	Classes.ThreadMethods.put(Message.utilityParse("join"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Thread thread = (Thread)self.getData();
		    thread.join(); return null;
		}});	
	//----------------------------------------------------------------------
	Classes.OutputFileClassMethods.put(Message.utilityParse("open:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String file = (String)arguments[0].getData(Classes.String);
		    return new Sol(Classes.OutputFile, new FileOutputStream(file));
		}});
	Classes.OutputFileClassMethods.put(Message.utilityParse("append:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String file = (String)arguments[0].getData(Classes.String);
		    return new Sol(Classes.OutputFile, new FileOutputStream(file, true));
		}});
	//----------------------------------------
	Classes.OutputFileMethods.put(Message.utilityParse("write:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    FileOutputStream file = (FileOutputStream)self.getData();
		    if(arguments[0].typecheck(Classes.Byte))
			file.write((byte)arguments[0].getData()); 
		    else if(arguments[0].typecheck(Classes.Character))
			file.write((byte)(char)arguments[0].getData());
		    else {
			Sol bytes = Sol.send(arguments[0], "serialize");
			file.write((byte[])bytes.getData(Classes.Bytes));
		    }
		    return self;
		}});	
	Classes.OutputFileMethods.put(Message.utilityParse("writeLine:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Sol.send(self, new String[]{"write"}, new Sol[]{arguments[0]});
		    FileOutputStream file = (FileOutputStream)self.getData();
		    file.write('\n');
		    return self;
		}});	
	Classes.OutputFileMethods.put(Message.utilityParse("close"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    FileOutputStream file = (FileOutputStream)self.getData();
		    file.close(); return null;
		}});	
	//----------------------------------------------------------------------
	Classes.InputFileClassMethods.put(Message.utilityParse("open:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String file = (String)arguments[0].getData(Classes.String);
		    return new Sol(Classes.InputFile, new FileInputStream(file));
		}});
	//----------------------------------------
	Classes.InputFileMethods.put(Message.utilityParse("readCharacter"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return Sol.send(Sol.send(self, "read"), "toCharacter");
		}});	
	Classes.InputFileMethods.put(Message.utilityParse("read"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    FileInputStream file = (FileInputStream)self.getData();
		    return new Sol(Classes.Integer, file.read());
		}});
	Classes.InputFileMethods.put(Message.utilityParse("read:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    FileInputStream file = (FileInputStream)self.getData();
		    int number = (int)arguments[0].getData(Classes.Integer);
		    byte[] bytes = new byte[number];
		    if(file.read(bytes) == 0)
			return null;
		    return new Sol(Classes.Bytes, bytes);
		}});
	Classes.InputFileMethods.put(Message.utilityParse("readLine"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    FileInputStream file = (FileInputStream)self.getData();
		    StringBuilder stringBuilder = new StringBuilder();
		    int c;
		    for(c = file.read(); c != '\n' && c != -1; c = file.read())
			stringBuilder.append((char)c);
		    if(c == -1 && stringBuilder.length() == 0)
			return null;
		    return new Sol(Classes.String, stringBuilder.toString());
		}});
	Classes.InputFileMethods.put(Message.utilityParse("availableBytes"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    FileInputStream file = (FileInputStream)self.getData();
		    return new Sol(Classes.Integer, file.available());
		}});
	Classes.InputFileMethods.put(Message.utilityParse("hasCharacters"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    FileInputStream file = (FileInputStream)self.getData();
		    return file.available() > 0 ? Classes.True.get() : Classes.False.get();
		}});	
	Classes.InputFileMethods.put(Message.utilityParse("close"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    FileInputStream file = (FileInputStream)self.getData();
		    file.close(); return null;
		}});	
	//----------------------------------------------------------------------
	Classes.ByteMethods.put(Message.utilityParse("initialize"), new SolFunction() { 
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    self.setData((byte)0);
		    return self;
		}});
	Classes.ByteMethods.put(Message.utilityParse("serialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Bytes, ByteBuffer.allocate(1).put((byte)self.getData()));
		}});
	Classes.ByteMethods.put(Message.utilityParse("+"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    byte a = (byte)self.getData();
		    if(!arguments[0].typecheck(Classes.Byte))
			arguments[0] = Sol.send(arguments[0], "toByte");
		    return new Sol(Classes.Byte, a + (byte)arguments[0].getData(Classes.Byte));
		}});
	Classes.ByteMethods.put(Message.utilityParse("-"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    byte a = (byte)self.getData();
		    if(!arguments[0].typecheck(Classes.Byte))
			arguments[0] = Sol.send(arguments[0], "toByte");
		    return new Sol(Classes.Byte, a - (byte)arguments[0].getData(Classes.Byte));
		}});
	Classes.ByteMethods.put(Message.utilityParse("toInteger"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Integer, (int)(byte)self.getData());
		}});
	Classes.ByteMethods.put(Message.utilityParse("toCharacter"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Character, (char)(byte)self.getData());
		}});
	Classes.ByteMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.String, Byte.toString((byte)self.getData()));
		}});
	//----------------------------------------------------------------------
	Classes.BytesMethods.put(Message.utilityParse("serialize"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return self;
		}});
	Classes.BytesMethods.put(Message.utilityParse("at:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    int index = (int)arguments[0].getData(Classes.Integer);
		    byte[] bytes = (byte[])self.getData();
		    return new Sol(Classes.Byte, bytes[index]);
		}});
	Classes.BytesMethods.put(Message.utilityParse("size"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.Integer, ((byte[])self.getData()).length);
		}});
	Classes.BytesMethods.put(Message.utilityParse("asString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.String, new String((byte[])self.getData()));
		}});
	Classes.BytesMethods.put(Message.utilityParse("toString"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    return new Sol(Classes.String, "<" + Integer.toString(((byte[])self.getData()).length) + " bytes>");
		}});
	//----------------------------------------------------------------------
	Classes.SocketClassMethods.put(Message.utilityParse("host:port:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String address = (String)arguments[0].getData(Classes.String);
		    int port = (int)arguments[1].getData(Classes.Integer);
		    return new Sol(Classes.Socket, new SolSocket(address, port));
		}});
	//----------------------------------------
	Classes.SocketMethods.put(Message.utilityParse("send:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolSocket socket = (SolSocket)self.getData();
		    if(arguments[0].typecheck(Classes.Byte))
			socket.writeByte((byte)arguments[0].getData());
		    else if(arguments[0].typecheck(Classes.Character))
			socket.writeByte((byte)(char)arguments[0].getData());
		    else {
			Sol bytes = Sol.send(arguments[0], "serialize");
			socket.writeBytes((byte[])bytes.getData(Classes.Bytes));
		    }
		    return self;
		}});
	Classes.SocketMethods.put(Message.utilityParse("receiveAll"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolSocket socket = (SolSocket)self.getData();
		    byte[] bytes = socket.readBytes();
		    return bytes == null ? null : new Sol(Classes.Bytes, bytes);
		}});
	Classes.SocketMethods.put(Message.utilityParse("receiveLine"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolSocket socket = (SolSocket)self.getData();
		    String line = socket.readLine();
		    return line == null ? null : new Sol(Classes.String, line);
		}});
	Classes.SocketMethods.put(Message.utilityParse("receive"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolSocket socket = (SolSocket)self.getData();
		    return new Sol(Classes.Byte, (byte)socket.readByte());
		}});
	Classes.SocketMethods.put(Message.utilityParse("receive:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolSocket socket = (SolSocket)self.getData();
		    int count = (int)arguments[0].getData(Classes.Integer);
		    return new Sol(Classes.Bytes, (byte[])socket.readByte(count));
		}});
	Classes.SocketMethods.put(Message.utilityParse("availableBytes"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolSocket socket = (SolSocket)self.getData();
		    return new Sol(Classes.Integer, socket.availableBytes());
		}});
	Classes.SocketMethods.put(Message.utilityParse("hasData"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolSocket socket = (SolSocket)self.getData();
		    return socket.hasData() ? Classes.True.get() : Classes.False.get();
		}});
	Classes.SocketMethods.put(Message.utilityParse("isClosed"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolSocket socket = (SolSocket)self.getData();
		    return socket.isClosed() ? Classes.True.get() : Classes.False.get();
		}});
	Classes.SocketMethods.put(Message.utilityParse("isConnected"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolSocket socket = (SolSocket)self.getData();
		    return socket.isConnected() ? Classes.True.get() : Classes.False.get();
		}});
	Classes.SocketMethods.put(Message.utilityParse("close"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    SolSocket socket = (SolSocket)self.getData();
		    socket.close();
		    return null;
		}});
	//----------------------------------------------------------------------
	Classes.ServerSocketClassMethods.put(Message.utilityParse("port:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    int port = (int)arguments[0].getData(Classes.Integer);
		    return new Sol(Classes.ServerSocket, new ServerSocket(port));
		}});
	//----------------------------------------
	Classes.ServerSocketMethods.put(Message.utilityParse("listen"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    ServerSocket server = (ServerSocket)self.getData();
		    return new Sol(Classes.Socket, new SolSocket(server.accept()));
		}});
	Classes.ServerSocketMethods.put(Message.utilityParse("close"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    ServerSocket server = (ServerSocket)self.getData();
		    server.close();
		    return null;
		}});
	Classes.ServerSocketMethods.put(Message.utilityParse("isClosed"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    ServerSocket server = (ServerSocket)self.getData();
		    return server.isClosed() ? Classes.True.get() : Classes.False.get();
		}});
	//----------------------------------------------------------------------
	Classes.PatternClassMethods.put(Message.utilityParse("compile:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String pattern = (String)arguments[0].getData(Classes.String);		    
		    return new Sol(Classes.Pattern, Pattern.compile(pattern, Pattern.MULTILINE));
		}});
	Classes.PatternClassMethods.put(Message.utilityParse("compileInsensitive:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String pattern = (String)arguments[0].getData(Classes.String);		    
		    return new Sol(Classes.Pattern, Pattern.compile(Pattern.quote(pattern),
								    Pattern.MULTILINE ||
								    Pattern.CASE_INSENSITIVE));
		}});
	Classes.PatternClassMethods.put(Message.utilityParse("compileQuote:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String pattern = (String)arguments[0].getData(Classes.String);		    
		    return new Sol(Classes.Pattern, Pattern.compile(Pattern.quote(pattern),
								    Pattern.MULTILINE));
		}});
	Classes.PatternClassMethods.put(Message.utilityParse("replace:with:on:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String pattern     = (String)arguments[0].getData(Classes.String);
		    String replacement = (String)arguments[1].getData(Classes.String);
		    String data        = (String)arguments[2].getData(Classes.String);
		    Matcher matcher = Pattern.compile(pattern, Pattern.MULTILINE).matcher(data);
		    return new Sol(Classes.String, matcher.replaceAll(replacement));
		}});
	Classes.PatternClassMethods.put(Message.utilityParse("matches:on:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String pattern = (String)arguments[0].getData(Classes.String);
		    String data    = (String)arguments[1].getData(Classes.String);
		    Matcher matcher = Pattern.compile(pattern, Pattern.MULTILINE).matcher(data);
		    return matcher.matches() ? Classes.True.get() : Classes.False.get();
		}});
	Classes.PatternClassMethods.put(Message.utilityParse("match:on:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    String pattern = (String)arguments[0].getData(Classes.String);
		    String data    = (String)arguments[1].getData(Classes.String);
		    ArrayList<Sol> groups = new ArrayList<Sol>();
		    Matcher matcher = Pattern.compile(pattern, Pattern.MULTILINE).matcher(data);
		    if(!matcher.find()) return null;
		    for(int i=1; i<=matcher.groupCount(); i++)
			groups.add(new Sol(Classes.String, matcher.group(i)));
		    return new Sol(Classes.Vector, groups);
		}});
	//----------------------------------------
	Classes.PatternMethods.put(Message.utilityParse("matches:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Pattern pattern = ((Pattern)self.getData());
		    String data = (String)arguments[0].getData(Classes.String);
		    Matcher matcher = pattern.matcher(data);
		    return matcher.matches() ? Classes.True.get() : Classes.False.get();
		}});
	Classes.PatternMethods.put(Message.utilityParse("match:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Pattern pattern = ((Pattern)self.getData());
		    String data = (String)arguments[0].getData(Classes.String);
		    ArrayList<Sol> groups = new ArrayList<Sol>();
		    Matcher matcher = pattern.matcher(data);
		    if(!matcher.find()) return null;
		    for(int i=1; i<=matcher.groupCount(); i++)
			groups.add(new Sol(Classes.String, matcher.group(i)));
		    return new Sol(Classes.Vector, groups);
		}});
	Classes.PatternMethods.put(Message.utilityParse("replaceWith:in:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Pattern pattern = ((Pattern)self.getData());
		    String replacement = (String)arguments[0].getData(Classes.String);
		    String data = (String)arguments[1].getData(Classes.String);
		    Matcher matcher = pattern.matcher(data);
		    return new Sol(Classes.String, matcher.replaceAll(replacement));
		}});
	Classes.PatternMethods.put(Message.utilityParse("replaceFirstWith:in:"), new SolFunction() {
		public Sol call(Sol self, Sol[] arguments) throws Exception {
		    Pattern pattern = ((Pattern)self.getData());
		    String replacement = (String)arguments[0].getData(Classes.String);
		    String data = (String)arguments[1].getData(Classes.String);
		    Matcher matcher = pattern.matcher(data);
		    return new Sol(Classes.String, matcher.replaceFirst(replacement));
		}});
	    
    }
}
