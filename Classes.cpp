Sol Object;           map<string, StandardSolFunction> ObjectMethods;
Sol Nothing;          map<string, StandardSolFunction> NothingMethods;
Sol Class;            map<string, StandardSolFunction> ClassMethods;

Sol Lambda;           map<string, StandardSolFunction> LambdaMethods;
Sol Vector;           map<string, StandardSolFunction> VectorMethods;
Sol Symbol;           map<string, StandardSolFunction> SymbolMethods;
Sol SymbolDictionary; map<string, StandardSolFunction> SymbolDictionaryMethods;
Sol BuiltinMethod;    map<string, StandardSolFunction> BuiltinMethodMethods;
Sol Integer;          map<string, StandardSolFunction> IntegerMethods;
Sol Double;           map<string, StandardSolFunction> DoubleMethods;
Sol Character;        map<string, StandardSolFunction> CharacterMethods;
Sol String;           map<string, StandardSolFunction> StringMethods;

struct {
  string name;
  Sol &klass;
  map<string, StandardSolFunction> &methods;
} Classes[] = {
  {"Object",           Object,           ObjectMethods},
  {"Nothing",          Nothing,          NothingMethods},
  {"Class",            Class,            ClassMethods},
  {"Integer",          Integer,          IntegerMethods},
  {"Double",           Double,           DoubleMethods},
  {"Character",        Character,        CharacterMethods},
  {"Lambda",           Lambda,           LambdaMethods},
  {"String",           String,           StringMethods},
  {"Vector",           Vector,           VectorMethods},
  {"Symbol",           Symbol,           SymbolMethods},
  {"SymbolDictionary", SymbolDictionary, SymbolDictionaryMethods},
  {"BuiltinMethod",    BuiltinMethod,    BuiltinMethodMethods},
};

Sol True, False;

struct {
  string name;
  Sol &klass;
} ConstantObjects[] = {
  {"True", True},
  {"False", False},
};
