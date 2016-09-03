import java.util.*;

public class Classes {
    public static Reference<Sol>
	Object    = new Reference(new Sol()),
	Nothing   = new Reference(new Sol()),
	Class     = new Reference(new Sol()),
	Lambda    = new Reference(new Sol()),
	BuiltinMethod = new Reference(new Sol()),
	MessageDictionary = new Reference(new Sol()),
	Vector    = new Reference(new Sol()),
	Integer   = new Reference(new Sol()),
	Double    = new Reference(new Sol()),
	Character = new Reference(new Sol()),
	String    = new Reference(new Sol()),
	Message   = new Reference(new Sol());

    public static Reference<Sol>
	True  = new Reference(new Sol()),
	False = new Reference(new Sol());

    public static HashMap<Message,SolFunction>
	ObjectMethods    = new HashMap<Message,SolFunction>(),
	NothingMethods   = new HashMap<Message,SolFunction>(),
	ClassMethods     = new HashMap<Message,SolFunction>(),
	LambdaMethods    = new HashMap<Message,SolFunction>(),
	BuiltinMethodMethods = new HashMap<Message,SolFunction>(),
	VectorMethods    = new HashMap<Message,SolFunction>(),
	MessageDictionaryMethods = new HashMap<Message,SolFunction>(),
	IntegerMethods   = new HashMap<Message,SolFunction>(),
	DoubleMethods    = new HashMap<Message,SolFunction>(),
	CharacterMethods = new HashMap<Message,SolFunction>(),
	StringMethods    = new HashMap<Message,SolFunction>(),
	MessageMethods   = new HashMap<Message,SolFunction>();

    static class ClassEntry {
	public String name;
	public Reference<Sol> clazz;
	public HashMap<Message,SolFunction> methods;
	ClassEntry(String name, Reference<Sol> clazz,
		   HashMap<Message,SolFunction> methods) {
	    this.name = name;
	    this.clazz = clazz;
	    this.methods = methods;
	}
    }

    static class ObjectEntry {
	public String name;
	public Reference<Sol> object;
	ObjectEntry(String name, Reference<Sol> object) {
	    this.name = name;
	    this.object = object;
	}
    }

    public static ClassEntry[] classes = new ClassEntry[]{
	new ClassEntry("Object",    Object,    ObjectMethods),
	new ClassEntry("Nothing",   Nothing,   NothingMethods),
	new ClassEntry("Class",     Class,     ClassMethods),
	new ClassEntry("Lambda",    Lambda,    LambdaMethods),
	new ClassEntry("BuiltinMethod", BuiltinMethod, BuiltinMethodMethods),
	new ClassEntry("Vector",    Vector,    VectorMethods),
	new ClassEntry("MessageDictionary", MessageDictionary, MessageDictionaryMethods),
	new ClassEntry("Integer",   Integer,   IntegerMethods),
	new ClassEntry("Double",    Double,    DoubleMethods),
	new ClassEntry("Character", Character, CharacterMethods),
	new ClassEntry("String",    String,    StringMethods),
	new ClassEntry("Message",    Message,    MessageMethods),
    };

    public static ObjectEntry[] objects = new ObjectEntry[]{
	new ObjectEntry("True",  True),
	new ObjectEntry("False", False),
    };
    static void populate(Environment environment) {
	for(ObjectEntry objectEntry : objects)
	    environment.link(objectEntry.name, objectEntry.object);
	for(ClassEntry classEntry : classes)
	    environment.link(classEntry.name, classEntry.clazz);
    }
    static void initialize() {
	for(ObjectEntry objectEntry : objects)
	    objectEntry.object.get().setClass(Object.get());
	for(ClassEntry classEntry : classes) {
	    Reference<Sol> clazz = classEntry.clazz;
	    clazz.get().setClass(Class.get());
	    clazz.get().defineVariable("superclass", Object.get());
	    HashMap<Message,Sol> methodDictionary = new HashMap<Message,Sol>();
	    clazz.get().defineVariable("methods", new Sol(MessageDictionary.get(),
							  methodDictionary));
	    clazz.get().defineVariable("instanceVariables",
				       new Sol(Vector.get(), new ArrayList<Sol>()));
	    clazz.get().defineVariable("name", new Sol(String.get(), classEntry.name));
	    for(Map.Entry<Message, SolFunction> entry : classEntry.methods.entrySet())
		methodDictionary.put(entry.getKey(),
				     new Sol(BuiltinMethod.get(), entry.getValue()));
	}
	Object.get().setVariable("superclass", null);
	Nothing.get().setVariable("superclass", null);
	Class.get().getVariable("instanceVariables").setData(new ArrayList<Sol>(Arrays.asList(new Sol[]{
			new Sol(Message.get(), new UnaryMessage("superclass")),
			new Sol(Message.get(), new UnaryMessage("methods")),
			new Sol(Message.get(), new UnaryMessage("instanceVariables")),
			new Sol(Message.get(), new UnaryMessage("name"))})));
    }
}
    
