import java.util.*;

public class Classes {
    public static Reference<Sol>
        Object                 = new Reference(new Sol()),
        ObjectClass            = new Reference(new Sol()),
        Nothing                = new Reference(new Sol()),
        NothingClass           = new Reference(new Sol()),
        Description            = new Reference(new Sol()),
        DescriptionClass       = new Reference(new Sol()),
        Class                  = new Reference(new Sol()),
        ClassClass             = new Reference(new Sol()),
        Metaclass              = new Reference(new Sol()),
        MetaclassClass         = new Reference(new Sol()),
        Lambda                 = new Reference(new Sol()),
        LambdaClass            = new Reference(new Sol()),
        BuiltinMethod          = new Reference(new Sol()),
        BuiltinMethodClass     = new Reference(new Sol()),
        MessageDictionary      = new Reference(new Sol()),
        MessageDictionaryClass = new Reference(new Sol()),
        Vector                 = new Reference(new Sol()),
        VectorClass            = new Reference(new Sol()),
        Integer                = new Reference(new Sol()),
        IntegerClass           = new Reference(new Sol()),
        Double                 = new Reference(new Sol()),
        DoubleClass            = new Reference(new Sol()),
        Character              = new Reference(new Sol()),
        CharacterClass         = new Reference(new Sol()),
        String                 = new Reference(new Sol()),
        StringClass            = new Reference(new Sol()),
        Message                = new Reference(new Sol()),
        MessageClass           = new Reference(new Sol());

    public static Reference<Sol>
        True  = new Reference(new Sol()),
        False = new Reference(new Sol());

    public static HashMap<Message,SolFunction>
        TrueMethods  = new HashMap<Message,SolFunction>(),
        FalseMethods = new HashMap<Message,SolFunction>();

    public static HashMap<Message,SolFunction>
        ObjectMethods                 = new HashMap<Message,SolFunction>(),
        ObjectClassMethods            = new HashMap<Message,SolFunction>(),
        NothingMethods                = new HashMap<Message,SolFunction>(),
        NothingClassMethods           = new HashMap<Message,SolFunction>(),
        DescriptionMethods            = new HashMap<Message,SolFunction>(),
        DescriptionClassMethods       = new HashMap<Message,SolFunction>(),
        ClassMethods                  = new HashMap<Message,SolFunction>(),
        ClassClassMethods             = new HashMap<Message,SolFunction>(),
        MetaclassMethods              = new HashMap<Message,SolFunction>(),
        MetaclassClassMethods         = new HashMap<Message,SolFunction>(),
        LambdaMethods                 = new HashMap<Message,SolFunction>(),
        LambdaClassMethods            = new HashMap<Message,SolFunction>(),
        BuiltinMethodMethods          = new HashMap<Message,SolFunction>(),
        BuiltinMethodClassMethods     = new HashMap<Message,SolFunction>(),
        VectorMethods                 = new HashMap<Message,SolFunction>(),
        VectorClassMethods            = new HashMap<Message,SolFunction>(),
        MessageDictionaryMethods      = new HashMap<Message,SolFunction>(),
        MessageDictionaryClassMethods = new HashMap<Message,SolFunction>(),
        IntegerMethods                = new HashMap<Message,SolFunction>(),
        IntegerClassMethods           = new HashMap<Message,SolFunction>(),
        DoubleMethods                 = new HashMap<Message,SolFunction>(),
        DoubleClassMethods            = new HashMap<Message,SolFunction>(),
        CharacterMethods              = new HashMap<Message,SolFunction>(),
        CharacterClassMethods         = new HashMap<Message,SolFunction>(),
        StringMethods                 = new HashMap<Message,SolFunction>(),
        StringClassMethods            = new HashMap<Message,SolFunction>(),
        MessageMethods                = new HashMap<Message,SolFunction>(),
        MessageClassMethods           = new HashMap<Message,SolFunction>();

    static class ClassEntry {
        public String name;
        public Reference<Sol> clazz;
        public HashMap<Message,SolFunction> methods;
        public Reference<Sol> metaclass;
        public HashMap<Message,SolFunction> classMethods;
        ClassEntry(String name, Reference<Sol> clazz, HashMap<Message,SolFunction> methods,
                   Reference<Sol> metaclass, HashMap<Message,SolFunction> classMethods) {
            this.name = name;
            this.clazz = clazz;
            this.methods = methods;
            this.metaclass = metaclass;
            this.classMethods = classMethods;
        }
    }

    static class ObjectEntry {
        public String name;
        public Reference<Sol> object;
        public HashMap<Message,SolFunction> methods;
        ObjectEntry(String name, Reference<Sol> object, HashMap<Message,SolFunction> methods) {
            this.name = name;
            this.object = object;
            this.methods = methods;
        }
    }

    public static ClassEntry[] classes = new ClassEntry[]{
        new ClassEntry("Object",            Object,            ObjectMethods,
                       ObjectClass,            ObjectClassMethods),
        new ClassEntry("Nothing",           Nothing,           NothingMethods,
                       NothingClass,           NothingClassMethods),
        new ClassEntry("Description",       Description,       DescriptionMethods,
                       DescriptionClass,       DescriptionClassMethods),
        new ClassEntry("Metaclass",         Metaclass,         MetaclassMethods,
                       MetaclassClass,         MetaclassClassMethods),
        new ClassEntry("Class",             Class,             ClassMethods,
                       ClassClass,             ClassClassMethods),
        new ClassEntry("Lambda",            Lambda,            LambdaMethods,
                       LambdaClass,            LambdaClassMethods),
        new ClassEntry("BuiltinMethod",     BuiltinMethod,     BuiltinMethodMethods,
                       BuiltinMethodClass,     BuiltinMethodClassMethods),
        new ClassEntry("Vector",            Vector,            VectorMethods,
                       VectorClass,            VectorClassMethods),
        new ClassEntry("MessageDictionary", MessageDictionary, MessageDictionaryMethods,
                       MessageDictionaryClass, MessageDictionaryClassMethods),
        new ClassEntry("Integer",           Integer,     IntegerMethods,
                       IntegerClass,           IntegerClassMethods),
        new ClassEntry("Double",            Double,      DoubleMethods,
                       DoubleClass,            DoubleClassMethods),
        new ClassEntry("Character",         Character,   CharacterMethods,
                       CharacterClass,         CharacterClassMethods),
        new ClassEntry("String",            String,      StringMethods,
                       StringClass,            StringClassMethods),
          new ClassEntry("Message",           Message,     MessageMethods,
                       MessageClass,           MessageClassMethods)
    };

    public static Environment GlobalEnvironment = new Environment();

    public static ObjectEntry[] objects = new ObjectEntry[]{
        new ObjectEntry("True",  True, TrueMethods),
        new ObjectEntry("False", False, FalseMethods)
    };
    static void initialize() {
        for(ObjectEntry objectEntry : objects)
            initializeObjectEntry(objectEntry);

        for(ClassEntry classEntry : classes)
            initializeClassEntry(classEntry);

        Object.get().setVariable("superclass", null);
        ObjectClass.get().setVariable("superclass", Class.get());

        Nothing.get().setVariable("superclass", null);
        NothingClass.get().setVariable("superclass", Class.get());

        Class.get().setVariable("superclass", Description.get());
        ClassClass.get().setVariable("superclass", DescriptionClass.get());

        Metaclass.get().setVariable("superclass", Description.get());
        MetaclassClass.get().setVariable("superclass", DescriptionClass.get());

        Description.get().getVariable("variables").setData(new ArrayList<Sol>(Arrays.asList(new Sol[]{
                        new Sol(Message, new UnaryMessage("superclass")),
                        new Sol(Message, new UnaryMessage("methods")),
                        new Sol(Message, new UnaryMessage("variables"))})));

        Metaclass.get().getVariable("variables").setData(new ArrayList<Sol>(Arrays.asList(new Sol[]{
                        new Sol(Message, new UnaryMessage("uniqueInstance"))})));

        Class.get().getVariable("variables").setData(new ArrayList<Sol>(Arrays.asList(new Sol[]{
                        new Sol(Message, new UnaryMessage("name"))})));

        initializeSeparateModules();
    }
    static void initializeObjectEntry(ObjectEntry objectEntry) {
        GlobalEnvironment.link(objectEntry.name, objectEntry.object);

        Sol metaclass = new Sol(Metaclass);
        metaclass.defineVariable("uniqueInstance", objectEntry.object.get());

        metaclass.defineVariable("superclass", Object.get());
        HashMap<Message,Sol> methodDictionary = new HashMap<Message,Sol>();
        metaclass.defineVariable("methods", new Sol(MessageDictionary, methodDictionary));
        metaclass.defineVariable("variables", new Sol(Vector, new ArrayList<Sol>()));
        for(Map.Entry<Message, SolFunction> entry : objectEntry.methods.entrySet())
            methodDictionary.put(entry.getKey(),
                                 new Sol(BuiltinMethod, entry.getValue()));

        objectEntry.object.get().setClass(metaclass);
    }
    static void initializeClassEntry(ClassEntry classEntry) {
        GlobalEnvironment.link(classEntry.name, classEntry.clazz);

        Reference<Sol> clazz = classEntry.clazz;
        Reference<Sol> metaclass = classEntry.metaclass;

        metaclass.get().setClass(Metaclass.get());

        metaclass.get().defineVariable("uniqueInstance", clazz.get());

        metaclass.get().defineVariable("superclass", Object.get());
        HashMap<Message,Sol> classMethodDictionary = new HashMap<Message,Sol>();
        metaclass.get().defineVariable("methods", new Sol(MessageDictionary, classMethodDictionary));
        metaclass.get().defineVariable("variables", new Sol(Vector, new ArrayList<Sol>()));
        for(Map.Entry<Message, SolFunction> entry : classEntry.classMethods.entrySet())
            classMethodDictionary.put(entry.getKey(),
                                 new Sol(BuiltinMethod, entry.getValue()));

        clazz.get().setClass(metaclass.get());

        clazz.get().defineVariable("name", new Sol(String, classEntry.name));

        clazz.get().defineVariable("superclass", Object.get());
        HashMap<Message,Sol> methodDictionary = new HashMap<Message,Sol>();
        clazz.get().defineVariable("methods", new Sol(MessageDictionary, methodDictionary));
        clazz.get().defineVariable("variables", new Sol(Vector, new ArrayList<Sol>()));
        for(Map.Entry<Message, SolFunction> entry : classEntry.methods.entrySet())
            methodDictionary.put(entry.getKey(),
                                 new Sol(BuiltinMethod, entry.getValue()));

        metaclass.get().defineVariable("superclass", ObjectClass.get());
    }
    //---SolEnvironment-----------------------------------------------------------
    public static Reference<Sol> SolEnvironment = new Reference(new Sol());
    public static Reference<Sol> SolEnvironmentClass = new Reference(new Sol());
    public static HashMap<Message,SolFunction>
        SolEnvironmentMethods = new HashMap<Message,SolFunction>(),
        SolEnvironmentClassMethods = new HashMap<Message,SolFunction>();

    //---Console-------------------------------------------------------------
    public static Reference<Sol> Console = new Reference(new Sol());
    public static HashMap<Message,SolFunction>
        ConsoleMethods = new HashMap<Message,SolFunction>();

    //---Thread-----------------------------------------------------------
    public static Reference<Sol> Thread = new Reference(new Sol());
    public static Reference<Sol> ThreadClass = new Reference(new Sol());
    public static HashMap<Message,SolFunction>
        ThreadMethods = new HashMap<Message,SolFunction>(),
        ThreadClassMethods = new HashMap<Message,SolFunction>();

    //---InputFile-----------------------------------------------------------
    public static Reference<Sol> InputFile = new Reference(new Sol());
    public static Reference<Sol> InputFileClass = new Reference(new Sol());
    public static HashMap<Message,SolFunction>
        InputFileMethods = new HashMap<Message,SolFunction>(),
        InputFileClassMethods = new HashMap<Message,SolFunction>();

    //---OutputFile----------------------------------------------------------
    public static Reference<Sol> OutputFile = new Reference(new Sol());
    public static Reference<Sol> OutputFileClass = new Reference(new Sol());
    public static HashMap<Message,SolFunction>
        OutputFileMethods = new HashMap<Message,SolFunction>(),
        OutputFileClassMethods = new HashMap<Message,SolFunction>();

    //---Byte----------------------------------------------------------------
    public static Reference<Sol> Byte = new Reference(new Sol());
    public static Reference<Sol> ByteClass = new Reference(new Sol());
    public static HashMap<Message,SolFunction>
        ByteMethods = new HashMap<Message,SolFunction>(),
        ByteClassMethods = new HashMap<Message,SolFunction>();

    //---Bytes--------------------------------------------------------------
    public static Reference<Sol> Bytes = new Reference(new Sol());
    public static Reference<Sol> BytesClass = new Reference(new Sol());
    public static HashMap<Message,SolFunction>
        BytesMethods = new HashMap<Message,SolFunction>(),
        BytesClassMethods = new HashMap<Message,SolFunction>();

    //---Socket-------------------------------------------------------------
    public static Reference<Sol> Socket = new Reference(new Sol());
    public static Reference<Sol> SocketClass = new Reference(new Sol());
    public static HashMap<Message,SolFunction>
        SocketMethods = new HashMap<Message,SolFunction>(),
        SocketClassMethods = new HashMap<Message,SolFunction>();

    //---ServerSocket-------------------------------------------------------
    public static Reference<Sol> ServerSocket = new Reference(new Sol());
    public static Reference<Sol> ServerSocketClass = new Reference(new Sol());
    public static HashMap<Message,SolFunction>
        ServerSocketMethods = new HashMap<Message,SolFunction>(),
        ServerSocketClassMethods = new HashMap<Message,SolFunction>();

    //---Pattern------------------------------------------------------------
    public static Reference<Sol> Pattern = new Reference(new Sol());
    public static HashMap<Message,SolFunction>
        PatternMethods = new HashMap<Message,SolFunction>();

    //---Exception-------------------------------------------------------
    public static Reference<Sol> Exception = new Reference(new Sol());
    public static Reference<Sol> ExceptionClass = new Reference(new Sol());
    public static HashMap<Message,SolFunction>
        ExceptionMethods = new HashMap<Message,SolFunction>(),
        ExceptionClassMethods = new HashMap<Message,SolFunction>();

    //---StringDictionary-------------------------------------------------------
    public static Reference<Sol> StringDictionary = new Reference(new Sol());
    public static Reference<Sol> StringDictionaryClass = new Reference(new Sol());
    public static HashMap<Message,SolFunction>
        StringDictionaryMethods = new HashMap<Message,SolFunction>(),
        StringDictionaryClassMethods = new HashMap<Message,SolFunction>();

    static void initializeSeparateModules() {
        initializeClassEntry(new ClassEntry("SolEnvironment", SolEnvironment, SolEnvironmentMethods,
                                            SolEnvironmentClass, SolEnvironmentClassMethods));
        initializeObjectEntry(new ObjectEntry("Console", Console, ConsoleMethods));
        initializeClassEntry(new ClassEntry("Thread", Thread, ThreadMethods,
                                            ThreadClass, ThreadClassMethods));
        initializeClassEntry(new ClassEntry("InputFile", InputFile, InputFileMethods,
                                            InputFileClass, InputFileClassMethods));
        initializeClassEntry(new ClassEntry("OutputFile", OutputFile, OutputFileMethods,
                                            OutputFileClass, OutputFileClassMethods));
        initializeClassEntry(new ClassEntry("Byte", Byte, ByteMethods,
                                            ByteClass, ByteClassMethods));
        initializeClassEntry(new ClassEntry("Bytes", Bytes, BytesMethods,
                                            BytesClass, BytesClassMethods));
        initializeClassEntry(new ClassEntry("Socket", Socket, SocketMethods,
                                            SocketClass, SocketClassMethods));
        initializeClassEntry(new ClassEntry("ServerSocket", ServerSocket, ServerSocketMethods,
                                            ServerSocketClass, ServerSocketClassMethods));
        initializeClassEntry(new ClassEntry("Exception", Exception, ExceptionMethods,
                                            ExceptionClass, ExceptionClassMethods));
        initializeObjectEntry(new ObjectEntry("Pattern", Pattern, PatternMethods));
        initializeClassEntry(new ClassEntry("StringDictionary", StringDictionary, StringDictionaryMethods,
                                            StringDictionaryClass, StringDictionaryClassMethods));
    }
}
