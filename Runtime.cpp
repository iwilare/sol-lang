void initializeClasses() {
  //map<MethodName, StandardSolFunction> ObjectMethods;
  ObjectMethods["send:"] = [](Sol self, vector<Sol> arguments) {
    string name = arguments[0]->getData<string>(Symbol);
    return self->send(name, {});
  };  
  ObjectMethods["send:with:"] = [](Sol self, vector<Sol> arguments) {
    string name = arguments[0]->getData<string>(Symbol);
    vector<Sol> actualArguments = arguments[1]->getData<vector<Sol> >(Vector);
    return self->send(name, actualArguments);
  };  
  ObjectMethods["send:with:as:"] = [](Sol self, vector<Sol> arguments) {
    string name = arguments[0]->getData<string>(Symbol);
    vector<Sol> actualArguments = arguments[1]->getData<vector<Sol> >(Vector);
    return self->send(name, actualArguments, arguments[2]);
  };  
  ObjectMethods["send:as:"] = [](Sol self, vector<Sol> arguments) {
    string name = arguments[0]->getData<string>(Symbol);
    return self->send(name, {}, arguments[1]);
  };  
  ObjectMethods["superSend:"] = [](Sol self, vector<Sol> arguments) {
    string name = arguments[0]->getData<string>(Symbol);
    return self->superSend(name, {});
  };
  ObjectMethods["superSend:with:"] = [](Sol self, vector<Sol> arguments) {
    string name = arguments[0]->getData<string>(Symbol);
    vector<Sol> actualArguments = arguments[1]->getData<vector<Sol> >(Vector);
    return self->superSend(name, actualArguments);
  };  
  ObjectMethods["initialize"] = [](Sol self, vector<Sol> arguments) {
    return self;
  };
  ObjectMethods["becomes:"] = [](Sol self, vector<Sol> arguments) {
    *(self) = *(arguments[0]);
    return self;
  };
  ObjectMethods["class"] = [](Sol self, vector<Sol> arguments) {
    return self->getClass();
  };
  ObjectMethods["class:"] = [](Sol self, vector<Sol> arguments) {
    self->setClass(arguments[0]);
    return self;
  };
  ObjectMethods["id"] = [](Sol self, vector<Sol> arguments) {
    return SolCreate(Integer, new int(*((int*)(&self))));
  };
  /*ObjectMethods["toString"] = [](Sol self, vector<Sol> arguments) {
    stringstream ss; ss<<(void*)self;
    string address = ss.str();
    return
    UTILITY_createString("<")
    ->send("+:", {self->klass->send("toString")})
    ->send("+:", {SolCreate(String, new string(" at "+address+">"))});
  };*/
  ObjectMethods["value"] = [](Sol self, vector<Sol> arguments) {
    return self;
  };
  ObjectMethods["isObject"] = [](Sol self, vector<Sol> arguments) {
    return True;
  };
  ObjectMethods["isBoolean"] = [](Sol self, vector<Sol> arguments) {
    return False;
  };
  ObjectMethods["isNumber"] = [](Sol self, vector<Sol> arguments) {
    return False;
  };
  ObjectMethods["isSymbol"] = [](Sol self, vector<Sol> arguments) {
    return False;
  };
  ObjectMethods["==:"] = [](Sol self, vector<Sol> arguments) {
    return self == arguments[0] ? True : False;
  };
  ObjectMethods["!=:"] = [](Sol self, vector<Sol> arguments) {
    return self->send("==:",{arguments[0]})->send("not",{});
  };
  ObjectMethods["print"] = [](Sol self, vector<Sol> arguments) {
    self->send("toString")->send("printString");
    return self;
  };
  ObjectMethods["printLine"] = [](Sol self, vector<Sol> arguments) {
    self->send("print");
    cout<<endl;
    return self;
  };
  ObjectMethods["delete"] = [](Sol self, vector<Sol> arguments) {
    return (Sol)nullptr;
  };
  ObjectMethods["doesNotUnderstand:with:"] = [](Sol self, vector<Sol> arguments) {
    string className = UTILITY_getObjectTypeName(self);
    string message = arguments[0]->getData<string>(Symbol);
    vector<Sol> actualArguments = arguments[1]->getData<vector<Sol> >(Vector);
    throw RuntimeException("An instance of " + className + " could not understand the message " + message + ".");
    return (Sol)nullptr;
  };
  ObjectMethods["doesNotUnderstand:with:as:"] = [](Sol self, vector<Sol> arguments) {
    string realClassName = UTILITY_getObjectTypeName(self);
    string message = arguments[0]->getData<string>(Symbol);
    vector<Sol> actualArguments = arguments[1]->getData<vector<Sol> >(Vector);
    string className = UTILITY_getClassName(arguments[2]);
    throw RuntimeException("An instance of " + className + " (" + realClassName + ") could not understand the message " + message + ".");
    return (Sol)nullptr;
  };
  ObjectMethods["error"] = [](Sol self, vector<Sol> arguments) {
    string error = self->send("toString")->getData<string>();
    throw RuntimeException(error);
    return (Sol)nullptr;
  };
  ObjectMethods["printeronos"] = [](Sol self, vector<Sol> arguments) {
    self->printeronos();
    return self;
  };
  
  //map<MethodName, StandardSolFunction> NothingMethods;
  NothingMethods["send:"]           = ObjectMethods["send:"];
  NothingMethods["send:with:"]      = ObjectMethods["send:with:"];
  NothingMethods["superSend:"]      = ObjectMethods["superSend:"];
  NothingMethods["superSend:with:"] = ObjectMethods["superSend:with:"];
  NothingMethods["doesNotUnderstand:with:"] = [](Sol self, vector<Sol> arguments) {
    string message = arguments[0]->getData<string>(Symbol);
    throw RuntimeException("Nothing could not understand the message " + message + ".");
    return (Sol)nullptr;
  };
  NothingMethods["doesNotUnderstand:with:as:"] = [](Sol self, vector<Sol> arguments) {
    string message = arguments[0]->getData<string>(Symbol);
    vector<Sol> actualArguments = arguments[1]->getData<vector<Sol> >(Vector);
    string className = UTILITY_getClassName(arguments[2]);
    throw RuntimeException("Nothing (" + className+ ") could not understand the message " + message + ".");
    return (Sol)nullptr;
  };
  NothingMethods["==:"] = [](Sol self, vector<Sol> arguments) {
    return self == nullptr ? True : False;
  };
  NothingMethods["!=:"] = [](Sol self, vector<Sol> arguments) {
    return self == nullptr ? False : True;
  };

  //map<MethodName, StandardSolFunction> ClassMethods;
  ClassMethods["initialize"] = [](Sol self, vector<Sol> arguments) {
    self->setVariable("superclass", Object);
    self->setVariable("methods", SymbolDictionary->send("new"));
    self->setVariable("instanceVariables", Vector->send("new"));
    self->setVariable("name", SolCreate(String, new string("<Anonymous class>")));
    return self;
  };
  ClassMethods["superclass"] = [](Sol self, vector<Sol> arguments) {
    return self->getVariable("superclass");
  };
  ClassMethods["superclass:"] = [](Sol self, vector<Sol> arguments) {
    self->setVariable("superclass", arguments[0]);
    return self;
  };
  ClassMethods["method:"] = [](Sol self, vector<Sol> arguments) {
    return self->getVariable("methods")->send("at:", {arguments[0]});
  };
  ClassMethods["method:set:"] = [](Sol self, vector<Sol> arguments) {
    return self->getVariable("methods")->send("at:set:", {arguments[0], arguments[1]});
  };
  ClassMethods["methods"] = [](Sol self, vector<Sol> arguments) {
    return self->getVariable("methods");
  };
  ClassMethods["methods:"] = [](Sol self, vector<Sol> arguments) {
    self->setVariable("methods", arguments[0]);
    return self;
  };
  ClassMethods["instanceVariables"] = [](Sol self, vector<Sol> arguments) {
    return self->getVariable("instanceVariables");
  };
  ClassMethods["instanceVariables:"] = [](Sol self, vector<Sol> arguments) {
    self->setVariable("instanceVariables", arguments[0]);
    return self;
  };
  ClassMethods["name"] = [](Sol self, vector<Sol> arguments) {
    return self->getVariable("name");
  };
  ClassMethods["name:"] = [](Sol self, vector<Sol> arguments) {
    self->setVariable("name", arguments[0]);
    return self;
  };
  ClassMethods["basicNew"] = [](Sol self, vector<Sol> arguments) {
    return SolCreate(self);
  };
  ClassMethods["new"] = [](Sol self, vector<Sol> arguments) {
    Sol object = SolCreate(self);
    Sol klass = self;
    list<Sol> classes;
    while(klass != nullptr) {
      classes.push_front(klass);
      klass = klass->getVariable("superclass");
    }
    for(Sol klass : classes) {
      vector<Sol> *instanceVariables =
        klass->getVariable("instanceVariables")->data<vector<Sol> >(Vector);
      for(Sol instanceVariable : *instanceVariables) {
	string variable = instanceVariable->getData<string>(Symbol);
	object->defineVariable(variable);
      }
      object->send(Message("initialize"), {}, klass);
    }
    return object;
  };
  ClassMethods["toString"] = [](Sol self, vector<Sol> arguments) {
    return self->getVariable("name");
  };
  //map<MethodName, StandardSolFunction> LambdaMethods;
  LambdaMethods["valueIn:with:"] = [](Sol self, vector<Sol> arguments) {
    LambdaStructure *lambda = self->data<LambdaStructure>();
    Sol selfObject = arguments[0];
    Sol argumentsObject = arguments[1];
    vector<Sol> actualArguments = argumentsObject->getData<vector<Sol> >(Vector);
    lambda->evalLambda(selfObject, actualArguments);
    return (Sol)nullptr;
  };
  LambdaMethods["valueIn:"] = [](Sol self, vector<Sol> arguments) {
    LambdaStructure *lambda = self->data<LambdaStructure>();
    Sol selfObject = arguments[0];
    lambda->evalLambda(selfObject);
    return (Sol)nullptr;
  };
  LambdaMethods["values:"] = [](Sol self, vector<Sol> arguments) {
    LambdaStructure *lambda = self->data<LambdaStructure>();
    Sol argumentsObject = arguments[0];
    vector<Sol> actualArguments = argumentsObject->getData<vector<Sol> >(Vector);
    lambda->evalLambda(actualArguments);
    return (Sol)nullptr;
  };
  LambdaMethods["value:"] = [](Sol self, vector<Sol> arguments) {
    LambdaStructure *lambda = self->data<LambdaStructure>();
    lambda->evalLambda((vector<Sol>){arguments[0]});
    return (Sol)nullptr;
  };
  LambdaMethods["value"] = [](Sol self, vector<Sol> arguments) {
    LambdaStructure *lambda = self->data<LambdaStructure>();
    lambda->evalLambda();
    return (Sol)nullptr;
  };
  LambdaMethods["delete"] = [](Sol self, vector<Sol> arguments) {
    LambdaStructure *lambda = self->data<LambdaStructure>();
    delete lambda;
    return (Sol)nullptr;
  };
  //map<MethodName, StandardSolFunction> BuiltinMethodMethods;
  BuiltinMethodMethods["valueIn:with:"] = [](Sol self, vector<Sol> arguments) {
    StandardSolFunction method = self->getData<StandardSolFunction>();
    vector<Sol> actualVector = arguments[1]->getData<vector<Sol> >(Vector);
    return method(arguments[0], actualVector);
  };
  BuiltinMethodMethods["valueIn:"] = [](Sol self, vector<Sol> arguments) {
    StandardSolFunction method = self->getData<StandardSolFunction>();
    return method(arguments[0], {});
  };
  BuiltinMethodMethods["values:"] = [](Sol self, vector<Sol> arguments) {
    StandardSolFunction method = self->getData<StandardSolFunction>();
    vector<Sol> actualVector = arguments[1]->getData<vector<Sol> >(Vector);
    return method(nullptr, actualVector);
  };
  BuiltinMethodMethods["value:"] = [](Sol self, vector<Sol> arguments) {
    StandardSolFunction method = self->getData<StandardSolFunction>();
    return method(nullptr, {arguments[0]});
  };
  BuiltinMethodMethods["value"] = [](Sol self, vector<Sol> arguments) {
    StandardSolFunction method = self->getData<StandardSolFunction>();
    return method(nullptr, {});
  };
  BuiltinMethodMethods["delete"] = [](Sol self, vector<Sol> arguments) {
    delete self->data<StandardSolFunction>();
    return (Sol)nullptr;
  };
  //map<MethodName, StandardSolFunction> SymbolDictionaryMethods;
  SymbolDictionaryMethods["initialize"] = [](Sol self, vector<Sol> arguments) {
    self->setData(new map<string, Sol>());
    return self;
  };
  SymbolDictionaryMethods["delete"] = [](Sol self, vector<Sol> arguments) {
    map<string, Sol> *dictionary = self->data<map<string,Sol> >();
    delete dictionary;
    return (Sol)nullptr;
  };
  SymbolDictionaryMethods["at:set:"] = [](Sol self, vector<Sol> arguments) {
    map<string, Sol> *dictionary = self->data<map<string,Sol> >();
    string symbol = arguments[0]->getData<string>(Symbol);
    dictionary->operator[](symbol) = arguments[1];
    return self;
  };
  SymbolDictionaryMethods["at:"] = [](Sol self, vector<Sol> arguments) {
    map<string, Sol> *dictionary = self->data<map<string,Sol> >();
    string name = arguments[0]->getData<string>(Symbol);
    if(dictionary->count(name) == 1)
      return dictionary->operator[](name);
    else
      throw RuntimeException("Could not find \"" + name + "\" in a SymbolDictionary.");
  };
  SymbolDictionaryMethods["remove:"] = [](Sol self, vector<Sol> arguments) {
    map<string, Sol> *dictionary = self->data<map<string,Sol> >();
    string key = arguments[0]->getData<string>(Symbol);
    if(dictionary->count(key) > 0)
      dictionary->erase(key);
    return self;
  };
  SymbolDictionaryMethods["count"] = [](Sol self, vector<Sol> arguments) {
    map<string, Sol> *dictionary = self->data<map<string,Sol> >();
    return SolCreate(Integer, new int(dictionary->size()));
  };
  SymbolDictionaryMethods["isEmpty"] = [](Sol self, vector<Sol> arguments) {
    map<string, Sol> *dictionary = self->data<map<string,Sol> >();
    return dictionary->empty() ? True : False;
  };
  SymbolDictionaryMethods["toString"] = [](Sol self, vector<Sol> arguments) {
    stringstream ss;
    ss<<"{";
    map<string, Sol> *dictionary = self->data<map<string,Sol> >();
    bool firstElement = true;
    for(pair<string, Sol> k : *dictionary) {
      Sol valueString = k.second->send("toString");
      ss<<(firstElement ? "" : ", ")<< k.first <<": "<<valueString->getData<string>(String);
      firstElement = false;
    }
    ss<<"}";
    return SolCreate(String, new string(ss.str()));
  };
  SymbolDictionaryMethods["do:"] = [](Sol self, vector<Sol> arguments) {
    map<string, Sol> *dictionary = self->data<map<string,Sol> >();
    for(pair<string, Sol> pair : *dictionary) {
      Sol keyObject = SolCreate(String, new string(pair.first));
      arguments[0]->send("values:", {SolCreate(Vector, new vector<Sol>({keyObject, pair.second}))});
    }
    return (Sol)nullptr;
  };
  //map<MethodName, StandardSolFunction> IntegerMethods; 
  IntegerMethods["initialize"] = [](Sol self, vector<Sol> arguments) {
    self->setData(new int());
    return self;
  };  
  IntegerMethods["delete"] = [](Sol self, vector<Sol> arguments) {
    delete self->data<int>();
    return (Sol)nullptr;
  };
  IntegerMethods["+:"] = [](Sol self, vector<Sol> arguments) {
    int a = self->getData<int>();
    if(arguments[0]->getClass() == Integer)
      return SolCreate(Integer, new int(a + arguments[0]->getData<int>(Integer)));
    else if(arguments[0]->getClass() == Double)
      return SolCreate(Double, new double(a + arguments[0]->getData<double>(Double)));
    else
      throw RuntimeTypeException("Type error sending + to an Integer instance");
  };
  IntegerMethods["-:"] = [](Sol self, vector<Sol> arguments) {
    int a = self->getData<int>();
    if(arguments[0]->getClass() == Integer)
      return SolCreate(Integer, new int(a - arguments[0]->getData<int>(Integer)));
    else if(arguments[0]->getClass() == Double)
      return SolCreate(Double, new double(a - arguments[0]->getData<double>(Double)));
    else
      throw RuntimeTypeException("Type error sending - to an Integer instance");
  };
  
  IntegerMethods["*:"] = [](Sol self, vector<Sol> arguments) {
    int a = self->getData<int>();
    if(arguments[0]->getClass() == Integer)
      return SolCreate(Integer, new int(a * arguments[0]->getData<int>(Integer)));
    else if(arguments[0]->getClass() == Double)
      return SolCreate(Double, new double(a * arguments[0]->getData<double>(Double)));
    else
      throw RuntimeTypeException("Type error sending * to an Integer instance");
  };
  IntegerMethods["/:"] = [](Sol self, vector<Sol> arguments) {
    int a = self->getData<int>();
    if(arguments[0]->getClass() == Integer)
      return SolCreate(Integer, new int(a / arguments[0]->getData<int>(Integer)));
    else if(arguments[0]->getClass() == Double)
      return SolCreate(Double, new double(a / arguments[0]->getData<double>(Double)));
    else
      throw RuntimeTypeException("Type error sending / to an Integer instance");
  };
  IntegerMethods["==:"] = [](Sol self, vector<Sol> arguments) {
    int a = self->getData<int>();
    if(arguments[0]->getClass() == Integer)
      return a == arguments[0]->getData<int>(Integer) ? True : False;
    else if(arguments[0]->getClass() == Double)
      return a == arguments[0]->getData<double>(Double) ? True : False;
    else
      throw RuntimeTypeException("Type error sending == to an Integer instance");
  };
  IntegerMethods["<:"] = [](Sol self, vector<Sol> arguments) {
    int a = self->getData<int>();
    if(arguments[0]->getClass() == Integer)
      return a < arguments[0]->getData<int>(Integer) ? True : False;
    else if(arguments[0]->getClass() == Double)
      return a < arguments[0]->getData<double>(Double) ? True : False;
    else
      throw RuntimeTypeException("Type error sending < to an Integer instance");
  };
  IntegerMethods["negate"] = [](Sol self, vector<Sol> arguments) {
    int a = self->getData<int>();
    return SolCreate(Integer, new int(-a));
  };
  IntegerMethods["toString"] = [](Sol self, vector<Sol> arguments) {
    stringstream ss; ss<<self->getData<int>();
    return SolCreate(String, new string(ss.str()));
  };
  IntegerMethods["next"] = [](Sol self, vector<Sol> arguments) {
    return SolCreate(Integer, new int(self->getData<int>()+1));
  };
  //map<MethodName, StandardSolFunction> DoubleMethods;
  DoubleMethods["initialize"] = [](Sol self, vector<Sol> arguments) {
    self->setData(new double());
    return self;
  };   
  DoubleMethods["delete"] = [](Sol self, vector<Sol> arguments) {
    delete self->data<double>();
    return (Sol)nullptr;
  };
  DoubleMethods["+:"] = [](Sol self, vector<Sol> arguments) {
    double a = self->getData<double>();
    if(arguments[0]->getClass() == Integer)
      return SolCreate(Double, new double(a + arguments[0]->getData<int>(Integer)));
    else if(arguments[0]->getClass() == Double)
      return SolCreate(Double, new double(a + arguments[0]->getData<double>(Double)));
    else
      throw RuntimeTypeException("Type error sending + to a Double instance");
  };
  DoubleMethods["-:"] = [](Sol self, vector<Sol> arguments) {
    double a = self->getData<double>();
    if(arguments[0]->getClass() == Integer)
      return SolCreate(Double, new double(a - arguments[0]->getData<int>(Integer)));
    else if(arguments[0]->getClass() == Double)
      return SolCreate(Double, new double(a - arguments[0]->getData<double>(Double)));
    else
      throw RuntimeTypeException("Type error sending + to a Double instance");
  };
  DoubleMethods["*:"] = [](Sol self, vector<Sol> arguments) {
    double a = self->getData<double>();
    if(arguments[0]->getClass() == Integer)
      return SolCreate(Double, new double(a * arguments[0]->getData<int>(Integer)));
    else if(arguments[0]->getClass() == Double)
      return SolCreate(Double, new double(a * arguments[0]->getData<double>(Double)));
    else
      throw RuntimeTypeException("Type error sending + to a Double instance");
  };
  DoubleMethods["/:"] = [](Sol self, vector<Sol> arguments) {
    double a = self->getData<double>();
    if(arguments[0]->getClass() == Integer)
      return SolCreate(Double, new double(a / arguments[0]->getData<int>(Integer)));
    else if(arguments[0]->getClass() == Double)
      return SolCreate(Double, new double(a / arguments[0]->getData<double>(Double)));
    else
      throw RuntimeTypeException("Type error sending + to a Double instance");
  };
  DoubleMethods["==:"] = [](Sol self, vector<Sol> arguments) {
    double a = self->getData<double>();
    if(arguments[0]->getClass() == Integer)
      return a == arguments[0]->getData<int>(Integer) ? True : False;
    else if(arguments[0]->getClass() == Double)
      return a == arguments[0]->getData<double>(Double) ? True : False;
    else
      throw RuntimeTypeException("Type error sending == to a Double instance");
  };
  DoubleMethods["<:"] = [](Sol self, vector<Sol> arguments) {
    double a = self->getData<double>();
    if(arguments[0]->getClass() == Integer)
      return a < arguments[0]->getData<int>(Integer) ? True : False;
    else if(arguments[0]->getClass() == Double)
      return a < arguments[0]->getData<double>(Double) ? True : False;
    else
      throw RuntimeTypeException("Type error sending < to an Double instance");
  };
  DoubleMethods["negate"] = [](Sol self, vector<Sol> arguments) {
    double a = self->getData<double>();
    return SolCreate(Double, new double(-a));
  };
  DoubleMethods["toString"] = [](Sol self, vector<Sol> arguments) {
    stringstream ss; ss<<self->getData<double>();
    return SolCreate(String, new string(ss.str()));
  };
  //map<MethodName, StandardSolFunction> CharacterMethods;
  CharacterMethods["initialize"] = [](Sol self, vector<Sol> arguments) {    
    self->setData(new char());
    return self;
  };
  CharacterMethods["delete"] = [](Sol self, vector<Sol> arguments) {    
    delete self->data<char>();
    return (Sol)nullptr;
  };
  CharacterMethods["ascii"] = [](Sol self, vector<Sol> arguments) {
    return SolCreate(Integer, new int((int)self->getData<char>()));
  };
  CharacterMethods["+:"] = [](Sol self, vector<Sol> arguments) {
    string stringResult(1,self->getData<char>());
    if(arguments[0]->getClass() == Character)
      stringResult += arguments[0]->getData<char>(Character);
    else if(arguments[0]->getClass() == String)
      stringResult += arguments[0]->getData<string>(String);
    else {
      Sol argumentString = arguments[0]->send("toString");
      stringResult += argumentString->getData<string>(String);
      argumentString;
    }
    return SolCreate(String, new string(stringResult));
  };
  CharacterMethods["next"] = [](Sol self, vector<Sol> arguments) {
    return SolCreate(Character, new char(self->getData<char>()+1));
  };
  CharacterMethods["toString"] = [](Sol self, vector<Sol> arguments) {
    stringstream ss; ss<<self->getData<char>();
    return SolCreate(String, new string("'"+ss.str()+"'"));
  };
  CharacterMethods["print"] = [](Sol self, vector<Sol> arguments) {
    return self->send("printString");
  };
  CharacterMethods["printString"] = [](Sol self, vector<Sol> arguments) {
    cout<<self->getData<char>();
    return self;
  };
  CharacterMethods["==:"] = [](Sol self, vector<Sol> arguments) {
    return self->getData<char>(Character) == arguments[0]->getData<char>() ? True : False;
  };
  CharacterMethods["<:"] = [](Sol self, vector<Sol> arguments) {
    return self->getData<char>(Character) < arguments[0]->getData<char>() ? True : False;
  };
  //map<MethodName, StandardSolFunction> StringMethods;
  StringMethods["initialize"] = [](Sol self, vector<Sol> arguments) {
    self->setData(new string());
    return self;
  };
  StringMethods["delete"] = [](Sol self, vector<Sol> arguments) {
    delete self->data<string>();
    return (Sol)nullptr;
  };
  StringMethods["+:"] = [](Sol self, vector<Sol> arguments) {
    string stringAppend;
    if(arguments[0]->getClass() == Character)
      stringAppend += arguments[0]->getData<char>(Character);
    else if(arguments[0]->getClass() == String)
      stringAppend += arguments[0]->getData<string>(String);
    else {
      Sol argumentString = arguments[0]->send("toString");
      stringAppend += argumentString->getData<string>(String);
      argumentString;
    }
    return SolCreate(String, new string(self->getData<string>() + stringAppend));
  };
  StringMethods["toString"] = [](Sol self, vector<Sol> arguments) {
    return SolCreate(String, new string("\""+self->getData<string>()+"\""));
  };
  StringMethods["length"] = [](Sol self, vector<Sol> arguments) {
    string s = self->getData<string>();
    return SolCreate(Integer, new int(s.size()));
  };
  StringMethods["at:"] = [](Sol self, vector<Sol> arguments) {
    string s = self->getData<string>();
    int index = arguments[0]->getData<int>(Integer);
    if(0 <= index and index < s.length()) 
      return SolCreate(Character, new char(s[index]));
    else
      throw RuntimeException("Out of boundary while indexing a String.");
  };
  StringMethods["==:"] = [](Sol self, vector<Sol> arguments) {
    string a = self->getData<string>();
    return a == arguments[0]->getData<string>(String) ? True : False;
  };
  StringMethods["<:"] = [](Sol self, vector<Sol> arguments) {
    string a = self->getData<string>();
    return a < arguments[0]->getData<string>(String) ? True : False;
  };
  StringMethods["print"] = [](Sol self, vector<Sol> arguments) {
    return self->send("printString");
  };
  StringMethods["printString"] = [](Sol self, vector<Sol> arguments) {
    cout<<self->getData<string>();
    return self;
  };
  //map<MethodName, StandardSolFunction> SymbolMethods;
  SymbolMethods["initialize"] = [](Sol self, vector<Sol> arguments) {
    self->setData(new string());
    return self;
  };
  SymbolMethods["delete"] = [](Sol self, vector<Sol> arguments) {
    delete self->data<string>();
    return (Sol)nullptr;
  };
  SymbolMethods["toString"] = [](Sol self, vector<Sol> arguments) {
    return SolCreate(String, new string("#"+self->getData<string>()));
  };
  SymbolMethods["+:"] = [](Sol self, vector<Sol> arguments) {
    string stringResult(self->getData<string>());
    if(arguments[0]->getClass() == Character)
      stringResult += arguments[0]->getData<char>(Character);
    else if(arguments[0]->getClass() == Symbol)
      stringResult += arguments[0]->getData<string>(Symbol);
    else
      throw RuntimeTypeException("Cannot add something to a symbol except for Characters and Symbols.");
    return SolCreate(Symbol, new string(stringResult));
  };
  SymbolMethods["==:"] = [](Sol self, vector<Sol> arguments) {
    string a = self->getData<string>();
    return a == arguments[0]->getData<string>(Symbol) ? True : False;
  };
  SymbolMethods["<:"] = [](Sol self, vector<Sol> arguments) {
    string a = self->getData<string>();
    return a < arguments[0]->getData<string>(Symbol) ? True : False;
  };
  SymbolMethods["asSetMethod"] = [](Sol self, vector<Sol> arguments) {
    string identifier = self->getData<string>();
    Message compoundSymbol(identifier);
    if(compoundSymbol.parts.size() == 0)
      return SolCreate(Lambda, new LambdaStructure(new Atom(new Atom(identifier),
							  new Atom(identifier)),
						 {identifier}, nullptr, nullptr));
    vector<Atom*> sequence;
    for(string identifier : compoundSymbol.parts)
      sequence.push_back(new Atom(new Atom(identifier),
				  new Atom(identifier)));
    return SolCreate(Lambda, new LambdaStructure(new Atom(sequence),
					       compoundSymbol.parts, nullptr, nullptr));
  };
  SymbolMethods["asGetMethod"] = [](Sol self, vector<Sol> arguments) {
    string identifier = self->getData<string>();
    return SolCreate(Lambda, new LambdaStructure(new Atom(identifier),
					       {}, nullptr, nullptr));
  };
  //map<MethodName, StandardSolFunction> VectorMethods;
  VectorMethods["initialize"] = [](Sol self, vector<Sol> arguments) {
    self->setData(new vector<Sol>());
    return self;
  };
  VectorMethods["delete"] = [](Sol self, vector<Sol> arguments) {
    vector<Sol> *v = self->data<vector<Sol> >();
    delete v;
    return (Sol)nullptr;
  };
  VectorMethods["size"] = [](Sol self, vector<Sol> arguments) {
    vector<Sol> *v = self->data<vector<Sol> >();
    return SolCreate(Integer, new int(v->size()));
  };
  VectorMethods["at:"] = [](Sol self, vector<Sol> arguments) {
    vector<Sol> *v = self->data<vector<Sol> >();
    int index = arguments[0]->getData<int>(Integer);
    if(0 <= index and index < v->size())
      return v->operator[](index);
    else
      throw RuntimeException("Out of boundary while indexing a Vector.");
  };
  VectorMethods["at:set:"] = [](Sol self, vector<Sol> arguments) {
    vector<Sol> *v = self->data<vector<Sol> >();
    int index = arguments[0]->getData<int>(Integer);
    if(0 <= index and index < v->size()) {
      return v->operator[](index) = arguments[1];
    } else
      throw RuntimeException("Out of boundary string indexing a Vector.");
  };
  VectorMethods["resize:"] = [](Sol self, vector<Sol> arguments) {
    vector<Sol> *v = self->data<vector<Sol> >();
    v->resize(arguments[0]->getData<int>(Integer), nullptr);
    return self;
  };
  VectorMethods["resize:with:"] = [](Sol self, vector<Sol> arguments) {
    vector<Sol> *v = self->data<vector<Sol> >();
    v->resize(arguments[0]->getData<int>(Integer), arguments[1]);
    return self;
  };
  VectorMethods["toString"] = [](Sol self, vector<Sol> arguments) {
    Sol result = SolCreate(String);
    vector<Sol> v = self->getData<vector<Sol> >();
    string s;
    if(v.size() == 0)
      s = "";
    else {
      for(int i=0; i<v.size()-1; i++) {
	Sol vs = v[i]->send("toString");
	s += vs->getData<string>(String) + ", ";
      }
      Sol vs = v.back()->send("toString");
      s += vs->getData<string>(String);
    }
    result->setData(new string("["+s+"]"));
    return result;
  };
  VectorMethods["push:"] = [](Sol self, vector<Sol> arguments) {
    vector<Sol> *v = self->data<vector<Sol> >();
    v->push_back(arguments[0]);
    return self;
  };
}

void classReification() {
  for(auto k : Classes)
    k.klass = SolCreate();
  for(auto k : Classes) {
    k.klass->setClass(Class);
    k.klass->defineVariable("superclass", Object);
    map<string, Sol> *methods = new map<string, Sol>();
    k.klass->defineVariable("methods",
			    SolCreate(SymbolDictionary, methods));
    k.klass->defineVariable("instanceVariables",
			    SolCreate(Vector, new vector<Sol>()));
    k.klass->defineVariable("name",
			    SolCreate(String, new string(k.name)));
    
    for(pair<string, StandardSolFunction> p : k.methods) {
      Sol method = SolCreate(BuiltinMethod, new StandardSolFunction(p.second));
      methods->operator[](p.first) = method;
    }
  }
  Object->setVariable("superclass", nullptr);
  Nothing->setVariable("superclass", nullptr);
  
  Class->getVariable("instanceVariables")->setData(new vector<Sol>({
	SolCreate(Symbol, new string("superclass")),
	  SolCreate(Symbol, new string("methods")),
	  SolCreate(Symbol, new string("instanceVariables")),
	  SolCreate(Symbol, new string("name"))}));
}

void initializeGlobalEnvironment(Environment globalEnvironment) {
  for(auto k : Classes)
    globalEnvironment->link(k.name, &k.klass); 
  for(auto k : ConstantObjects) 
    globalEnvironment->link(k.name, &k.klass);
}
