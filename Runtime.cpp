void initializeClasses() {
  //map<MethodName, StandardSolFunction> ObjectMethods;
  ObjectMethods["send:with:"] = [](Sol *self, vector<Sol*> arguments) {
    typeCheck(arguments[0], Symbol);
    typeCheck(arguments[1], Vector);
    string name = *(string*)arguments[0]->data;
    vector<Sol*> actualArguments = *(vector<Sol*>*)arguments[1]->data;
    return self->send(name, actualArguments);
  };  
  ObjectMethods["superSend:with:"] = [](Sol *self, vector<Sol*> arguments) {
    typeCheck(arguments[0], Symbol);
    typeCheck(arguments[1], Vector);
    string name = *(string*)arguments[0]->data;
    vector<Sol*> actualArguments = *(vector<Sol*>*)arguments[1]->data;
    return self->superSend(name, actualArguments);
  };  
  ObjectMethods["initialize"] = [](Sol *self, vector<Sol*> arguments) {
    return self;
  };
  ObjectMethods["class"] = [](Sol *self, vector<Sol*> arguments) {
    return self->klass;
  };
  ObjectMethods["class:"] = [](Sol *self, vector<Sol*> arguments) {
    self->klass = arguments[0];
    return self;
  };
  ObjectMethods["id"] = [](Sol *self, vector<Sol*> arguments) {
    Sol *id = new Sol(Integer);
    id->data = new int(*((int*)(&self)));
    return id;
  };
  ObjectMethods["toString"] = [](Sol *self, vector<Sol*> arguments) {
    stringstream ss; ss<<(void*)self;
    string address = ss.str();
    return
    UTILITY_createString("<")
    ->send("+:", {self->klass->send("toString")})
    ->send("+:", {UTILITY_createString(" at "+address+">")});
  };
  ObjectMethods["value"] = [](Sol *self, vector<Sol*> arguments) {
    return self;
  };
  ObjectMethods["isObject"] = [](Sol *self, vector<Sol*> arguments) {
    return True;
  };
  ObjectMethods["isBoolean"] = [](Sol *self, vector<Sol*> arguments) {
    return False;
  };
  ObjectMethods["isNumber"] = [](Sol *self, vector<Sol*> arguments) {
    return False;
  };
  ObjectMethods["isSymbol"] = [](Sol *self, vector<Sol*> arguments) {
    return False;
  };
  ObjectMethods["==:"] = [](Sol *self, vector<Sol*> arguments) {
    return self == arguments[0] ? True : False;
  };
  ObjectMethods["!=:"] = [](Sol *self, vector<Sol*> arguments) {
    return self->send("==:",{arguments[0]})->send("not",{});
  };
  ObjectMethods["print"] = [](Sol *self, vector<Sol*> arguments) {
    self->send("toString")->send("printString")->finalize();
    return self;
  };
  ObjectMethods["printLine"] = [](Sol *self, vector<Sol*> arguments) {
    self->send("print")->finalize();
    cout<<endl;
    return self;
  };
  ObjectMethods["deallocate"] = [](Sol *self, vector<Sol*> arguments) {
    return self;
  };
  ObjectMethods["doesNotUnderstand:"] = [](Sol *self, vector<Sol*> arguments) {
    string className = UTILITY_getObjectTypeName(self);
    typeCheck(arguments[0], Symbol);
    string method = *(string*)arguments[0]->data;
    throw RuntimeException("An instance of " + className + " could not understand the method " + method + ".");
    return (Sol*)nullptr;
  };
  ObjectMethods["printeronos"] = [](Sol *self, vector<Sol*> arguments) {
    self->printeronos();
    return self;
  };
    
  //map<MethodName, StandardSolFunction> ClassMethods;
  ClassMethods["initialize"] = [](Sol *self, vector<Sol*> arguments) {
    self->setVariable("superclass", Object);
    self->setVariable("methodDictionary", SymbolDictionary->send("new"));
    self->setVariable("instanceVariables", Vector->send("new"));
    self->setVariable("instances", Vector->send("new"));
    self->setVariable("name", String->send("new"));
    self->getVariable("name")->data = new string("<Anonymous class>");
    return self;
  };
  ClassMethods["superclass"] = [](Sol *self, vector<Sol*> arguments) {
    return self->getVariable("superclass");
  };
  ClassMethods["superclass:"] = [](Sol *self, vector<Sol*> arguments) {
    self->setVariable("superclass", arguments[0]);
    return self;
  };
  ClassMethods["method:"] = [](Sol *self, vector<Sol*> arguments) {
    return self->getVariable("methodDictionary")->send("at:", {arguments[0]});
  };
  ClassMethods["method:set:"] = [](Sol *self, vector<Sol*> arguments) {
    return self->getVariable("methodDictionary")->send("at:set:", {arguments[0], arguments[1]});
  };
  ClassMethods["methodDictionary"] = [](Sol *self, vector<Sol*> arguments) {
    return self->getVariable("methodDictionary");
  };
  ClassMethods["methodDictionary:"] = [](Sol *self, vector<Sol*> arguments) {
    self->setVariable("methodDictionary", arguments[0]);
    return self;
  };
  ClassMethods["instanceVariables"] = [](Sol *self, vector<Sol*> arguments) {
    return self->getVariable("instanceVariables");
  };
  ClassMethods["instanceVariables:"] = [](Sol *self, vector<Sol*> arguments) {
    self->setVariable("instanceVariables", arguments[0]);
    return self;
  };
  ClassMethods["name"] = [](Sol *self, vector<Sol*> arguments) {
    return self->getVariable("name");
  };
  ClassMethods["name:"] = [](Sol *self, vector<Sol*> arguments) {
    typeCheck(arguments[0], String);
    self->setVariable("name", arguments[0]);
    return self;
  };
  ClassMethods["basicNew"] = [](Sol *self, vector<Sol*> arguments) {
    return new Sol(self);
  };
  ClassMethods["new"] = [](Sol *self, vector<Sol*> arguments) {
    Sol *object = new Sol();
    object->klass = self;
    object->superSend("initialize");
    object->send("initialize");
    Sol *klass = self;
    while(klass != nullptr) {
      typeCheck(klass->getVariable("instanceVariables"), Vector);
      vector<Sol*> *instanceVariables =
	(vector<Sol*>*)klass->getVariable("instanceVariables")->data;
      for(Sol *instanceVariable : *instanceVariables) {
	typeCheck(instanceVariable, Symbol);
	string variable = *(string*)instanceVariable->data;
	object->setVariable(variable, (Sol*)nullptr);
      }
      klass = klass->getVariable("superclass");
    }
    return object;
  };
  ClassMethods["toString"] = [](Sol *self, vector<Sol*> arguments) {
    return self->getVariable("name");
  };
  //map<MethodName, StandardSolFunction> LambdaMethods;
  LambdaMethods["valueIn:with:"] = [](Sol *self, vector<Sol*> arguments) {
    LambdaStructure *lambda = (LambdaStructure*)self->data;    
    Sol *selfObject = arguments[0];
    Sol *argumentsObject = arguments[1];
    typeCheck(argumentsObject, Vector);
    vector<Sol*> actualArguments = *(vector<Sol*>*)argumentsObject->data;
    return evalLambda(lambda, selfObject, actualArguments);
  };
  LambdaMethods["valueIn:"] = [](Sol *self, vector<Sol*> arguments) {
    LambdaStructure *lambda = (LambdaStructure*)self->data;    
    Sol *selfObject = arguments[0];
    return evalLambda(lambda, selfObject, {});
  };
  LambdaMethods["values:"] = [](Sol *self, vector<Sol*> arguments) {
    LambdaStructure *lambda = (LambdaStructure*)self->data;
    Sol *argumentsObject = arguments[0];
    typeCheck(argumentsObject, Vector);
    vector<Sol*> actualArguments = *(vector<Sol*>*)argumentsObject->data;
    return evalLambda(lambda, actualArguments);
  };
  LambdaMethods["value"] = [](Sol *self, vector<Sol*> arguments) {
    LambdaStructure *lambda = (LambdaStructure*)self->data;
    return evalLambda(lambda);
  };
  LambdaMethods["deallocate"] = [](Sol *self, vector<Sol*> arguments) {
    delete (LambdaStructure*)self->data;
    return self;
  };
  //map<MethodName, StandardSolFunction> BuiltinMethodMethods;
  BuiltinMethodMethods["valueIn:with:"] = [](Sol *self, vector<Sol*> arguments) {
    StandardSolFunction method = *(StandardSolFunction*)self->data;
    typeCheck(arguments[1], Vector);
    vector<Sol*> actualVector = *(vector<Sol*>*)arguments[1]->data;
    return method(arguments[0], actualVector);
  };
  BuiltinMethodMethods["deallocate"] = [](Sol *self, vector<Sol*> arguments) {
    delete (StandardSolFunction*)self->data;
    return self;
  };
  //map<MethodName, StandardSolFunction> SymbolDictionaryMethods;
  SymbolDictionaryMethods["initialize"] = [](Sol *self, vector<Sol*> arguments) {
    self->data = new map<string, Sol*>();
    return self;
  };
  SymbolDictionaryMethods["deallocate"] = [](Sol *self, vector<Sol*> arguments) {
    delete (map<string,Sol*>*)self->data;
    return self;
  };
  SymbolDictionaryMethods["at:set:"] = [](Sol *self, vector<Sol*> arguments) {
    typeCheck(arguments[0], Symbol);
    map<string, Sol*> *dictionary = (map<string,Sol*>*)self->data;
    string symbol = *(string*)arguments[0]->data;
    arguments[1]->reference();
    if(dictionary->count(symbol) > 0)
      dictionary->operator[](symbol)->dereference();
    dictionary->operator[](symbol) = arguments[1];
    return self;
  };
  SymbolDictionaryMethods["at:"] = [](Sol *self, vector<Sol*> arguments) {
    typeCheck(arguments[0], Symbol);
    map<string, Sol*> *dictionary = (map<string,Sol*>*)self->data;
    string name = *(string*)arguments[0]->data;
    if(dictionary->count(name) == 1)
      return dictionary->operator[](name);
    else
      throw RuntimeException("Could not find \"" + name + "\" in a SymbolDictionary.");
  };
  SymbolDictionaryMethods["count"] = [](Sol *self, vector<Sol*> arguments) {
    map<string, Sol*> *dictionary = (map<string,Sol*>*)self->data;
    return new Sol(Integer, new int(dictionary->size()));
  };
  SymbolDictionaryMethods["isEmpty"] = [](Sol *self, vector<Sol*> arguments) {
    map<string, Sol*> *dictionary = (map<string,Sol*>*)self->data;
    return dictionary->empty() ? True : False;
  };
  SymbolDictionaryMethods["toString"] = [](Sol *self, vector<Sol*> arguments) {
    stringstream ss;
    ss<<"{";
    map<string, Sol*> *dictionary = (map<string,Sol*>*)self->data;
    bool firstElement = true;
    for(pair<string, Sol*> k : *dictionary) {
      Sol *valueString = k.second->send("toString");
      typeCheck(valueString, String);
      ss<< (firstElement ? "" : ", ") << k.first <<": "<<*(string*)valueString->data;
      valueString->finalize();
      firstElement = false;
    }
    ss<<"}";
    return new Sol(String, new string(ss.str()));
  };
  //map<MethodName, StandardSolFunction> IntegerMethods; 
  IntegerMethods["initialize"] = [](Sol *self, vector<Sol*> arguments) {
    self->data = new int();
    return self;
  };  
  IntegerMethods["deallocate"] = [](Sol *self, vector<Sol*> arguments) {
    delete (int*)self->data;
    return self;
  };
  IntegerMethods["+:"] = [](Sol *self, vector<Sol*> arguments) {
    int a = *(int*)self->data;
    if(arguments[0]->klass == Integer)
      return new Sol(Integer, new int(a + *(int*)arguments[0]->data));
    else if(arguments[0]->klass == Double)
      return new Sol(Double, new double(a + *(double*)arguments[0]->data));
    else
      throw RuntimeTypeException("Type error sending + to an Integer instance");
  };
  IntegerMethods["-:"] = [](Sol *self, vector<Sol*> arguments) {
    int a = *(int*)self->data;
    if(arguments[0]->klass == Integer)
      return new Sol(Integer, new int(a - *(int*)arguments[0]->data));
    else if(arguments[0]->klass == Double)
      return new Sol(Double, new double(a - *(double*)arguments[0]->data));
    else
      throw RuntimeTypeException("Type error sending - to an Integer instance");
  };
  
  IntegerMethods["*:"] = [](Sol *self, vector<Sol*> arguments) {
    int a = *(int*)self->data;
    if(arguments[0]->klass == Integer)
      return new Sol(Integer, new int(a * *(int*)arguments[0]->data));
    else if(arguments[0]->klass == Double)
      return new Sol(Double, new double(a * *(double*)arguments[0]->data));
    else
      throw RuntimeTypeException("Type error sending * to an Integer instance");
  };
  IntegerMethods["/:"] = [](Sol *self, vector<Sol*> arguments) {
    int a = *(int*)self->data;
    if(arguments[0]->klass == Integer)
      return new Sol(Integer, new int(a / *(int*)arguments[0]->data));
    else if(arguments[0]->klass == Double)
      return new Sol(Double, new double(a / *(double*)arguments[0]->data));
    else
      throw RuntimeTypeException("Type error sending / to an Integer instance");
  };
  IntegerMethods["==:"] = [](Sol *self, vector<Sol*> arguments) {
    int a = *(int*)self->data;
    if(arguments[0]->klass == Integer)
      return a == *(int*)arguments[0]->data ? True : False;
    else if(arguments[0]->klass == Double)
      return a == *(double*)arguments[0]->data ? True : False;
    else
      throw RuntimeTypeException("Type error sending == to an Integer instance");
  };
  IntegerMethods["<:"] = [](Sol *self, vector<Sol*> arguments) {
    int a = *(int*)self->data;
    if(arguments[0]->klass == Integer)
      return a < *(int*)arguments[0]->data ? True : False;
    else if(arguments[0]->klass == Double)
      return a < *(double*)arguments[0]->data ? True : False;
    else
      throw RuntimeTypeException("Type error sending < to an Integer instance");
  };
  IntegerMethods["negate"] = [](Sol *self, vector<Sol*> arguments) {
    int a = *(int*)self->data;
    return new Sol(Integer, new int(-a));
  };
  IntegerMethods["toString"] = [](Sol *self, vector<Sol*> arguments) {
    stringstream ss; ss<<*(int*)self->data;
    return new Sol(String, new string(ss.str()));
  };
  IntegerMethods["next"] = [](Sol *self, vector<Sol*> arguments) {
    return new Sol(Integer, new int(1));
  };
  //map<MethodName, StandardSolFunction> DoubleMethods;
  DoubleMethods["initialize"] = [](Sol *self, vector<Sol*> arguments) {
    self->data = new double();
    return self;
  };   
  DoubleMethods["deallocate"] = [](Sol *self, vector<Sol*> arguments) {
    delete (double*)self->data;
    return self;
  };
  DoubleMethods["+:"] = [](Sol *self, vector<Sol*> arguments) {
    double a = *(double*)self->data;
    if(arguments[0]->klass == Integer)
      return new Sol(Double, new double(a + *(int*)arguments[0]->data));
    else if(arguments[0]->klass == Double)
      return new Sol(Double, new double(a + *(double*)arguments[0]->data));
    else
      throw RuntimeTypeException("Type error sending + to a Double instance");
  };
  DoubleMethods["-:"] = [](Sol *self, vector<Sol*> arguments) {
    double a = *(double*)self->data;
    if(arguments[0]->klass == Integer)
      return new Sol(Double, new double(a - *(int*)arguments[0]->data));
    else if(arguments[0]->klass == Double)
      return new Sol(Double, new double(a - *(double*)arguments[0]->data));
    else
      throw RuntimeTypeException("Type error sending + to a Double instance");
  };
  DoubleMethods["*:"] = [](Sol *self, vector<Sol*> arguments) {
    double a = *(double*)self->data;
    if(arguments[0]->klass == Integer)
      return new Sol(Double, new double(a * *(int*)arguments[0]->data));
    else if(arguments[0]->klass == Double)
      return new Sol(Double, new double(a * *(double*)arguments[0]->data));
    else
      throw RuntimeTypeException("Type error sending + to a Double instance");
  };
  DoubleMethods["/:"] = [](Sol *self, vector<Sol*> arguments) {
    double a = *(double*)self->data;
    if(arguments[0]->klass == Integer)
      return new Sol(Double, new double(a / *(int*)arguments[0]->data));
    else if(arguments[0]->klass == Double)
      return new Sol(Double, new double(a / *(double*)arguments[0]->data));
    else
      throw RuntimeTypeException("Type error sending + to a Double instance");
  };
  DoubleMethods["==:"] = [](Sol *self, vector<Sol*> arguments) {
    double a = *(double*)self->data;
    if(arguments[0]->klass == Integer)
      return a == *(int*)arguments[0]->data ? True : False;
    else if(arguments[0]->klass == Double)
      return a == *(double*)arguments[0]->data ? True : False;
    else
      throw RuntimeTypeException("Type error sending == to a Double instance");
  };
  DoubleMethods["<:"] = [](Sol *self, vector<Sol*> arguments) {
    double a = *(double*)self->data;
    if(arguments[0]->klass == Integer)
      return a < *(int*)arguments[0]->data ? True : False;
    else if(arguments[0]->klass == Double)
      return a < *(double*)arguments[0]->data ? True : False;
    else
      throw RuntimeTypeException("Type error sending < to an Double instance");
  };
  DoubleMethods["negate"] = [](Sol *self, vector<Sol*> arguments) {
    double a = *(double*)self->data;
    return new Sol(Double, new double(-a));
  };
  DoubleMethods["toString"] = [](Sol *self, vector<Sol*> arguments) {
    stringstream ss; ss<<*(double*)self->data;
    return new Sol(String, new string(ss.str()));
  };
  //map<MethodName, StandardSolFunction> CharacterMethods;
  CharacterMethods["initialize"] = [](Sol *self, vector<Sol*> arguments) {
    self->data = new char();
    return self;
  };   
  CharacterMethods["ascii"] = [](Sol *self, vector<Sol*> arguments) {
    return new Sol(Integer, new int(*(char*)self->data));
  };
  CharacterMethods["+:"] = [](Sol *self, vector<Sol*> arguments) {
    string stringResult(1,*(char*)self->data);
    if(arguments[0]->klass == Character)
      stringResult += *(char*)arguments[0]->data;
    else if(arguments[0]->klass == String)
      stringResult += *(string*)arguments[0]->data;
    else {
      Sol *argumentString = self->send("toString");
      typeCheck(argumentString, String);
      stringResult += *(string*)argumentString->data;
      argumentString->finalize();
    }
    return new Sol(String, new string(stringResult));
  };
  CharacterMethods["next"] = [](Sol *self, vector<Sol*> arguments) {
    return new Sol(Character, new char(*(char*)self->data + 1));
  };
  CharacterMethods["toString"] = [](Sol *self, vector<Sol*> arguments) {
    stringstream ss; ss<<*(char*)self->data;
    return new Sol(String, new string("'"+ss.str()+"'"));
  };
  CharacterMethods["==:"] = [](Sol *self, vector<Sol*> arguments) {
    typeCheck(arguments[0], Character);
    return *(char*)self->data == *(char*)arguments[0]->data ? True : False;
  };
  CharacterMethods["<:"] = [](Sol *self, vector<Sol*> arguments) {
    typeCheck(arguments[0], Character);
    return *(char*)self->data < *(char*)arguments[0]->data ? True : False;
  };
  //map<MethodName, StandardSolFunction> StringMethods;
  StringMethods["initialize"] = [](Sol *self, vector<Sol*> arguments) {
    self->data = new string();
    return self;
  };
  StringMethods["deallocate"] = [](Sol *self, vector<Sol*> arguments) {
    delete (string*)self->data;
    return self;
  };
  StringMethods["+:"] = [](Sol *self, vector<Sol*> arguments) {
    string stringAppend;
    if(arguments[0]->klass == Character)
      stringAppend += *(char*)arguments[0]->data;
    else if(arguments[0]->klass == String)
      stringAppend += *(string*)arguments[0]->data;
    else {
      Sol *argumentString = arguments[0]->send("toString");
      typeCheck(argumentString, String);
      stringAppend += *(string*)argumentString->data;
      argumentString->finalize();
    }
    return new Sol(String, new string(*(string*)self->data + stringAppend));
  };
  StringMethods["toString"] = [](Sol *self, vector<Sol*> arguments) {
    return new Sol(String, new string("\""+*(string*)self->data+"\""));
  };
  StringMethods["length"] = [](Sol *self, vector<Sol*> arguments) {
    string s = *(string*)self->data;
    return new Sol(Integer, new int(s.size()));
  };
  StringMethods["at:"] = [](Sol *self, vector<Sol*> arguments) {
    typeCheck(arguments[0], Integer);
    string s = *(string*)self->data;
    int index = *(int*)arguments[0]->data;
    if(0 <= index and index < s.length()) 
      return new Sol(Character, new char(s[index]));
    else
      throw RuntimeException("Out of boundary while indexing a String.");
  };
  StringMethods["==:"] = [](Sol *self, vector<Sol*> arguments) {
    string a = *(string*)self->data;
    typeCheck(arguments[0], String);
    return a == *(string*)arguments[0]->data ? True : False;
  };
  StringMethods["<:"] = [](Sol *self, vector<Sol*> arguments) {
    string a = *(string*)self->data;
    typeCheck(arguments[0], String);
    return a < *(string*)arguments[0]->data ? True : False;
  };
  StringMethods["print"] = [](Sol *self, vector<Sol*> arguments) {
    return self->send("printString");
  };
  StringMethods["printString"] = [](Sol *self, vector<Sol*> arguments) {
    cout<<*(string*)self->data;
    return self;
  };
  //map<MethodName, StandardSolFunction> SymbolMethods;
  SymbolMethods["initialize"] = [](Sol *self, vector<Sol*> arguments) {
    self->data = new string();
    return self;
  };
  SymbolMethods["deallocate"] = [](Sol *self, vector<Sol*> arguments) {
    delete (string*)self->data;
    return self;
  };
  SymbolMethods["toString"] = [](Sol *self, vector<Sol*> arguments) {
    return new Sol(String, new string("#"+*(string*)self->data));
  };
  SymbolMethods["+:"] = [](Sol *self, vector<Sol*> arguments) {
    string stringResult(*(string*)self->data);
    if(arguments[0]->klass == Character)
      stringResult += *(char*)arguments[0]->data;
    else if(arguments[0]->klass == Symbol)
      stringResult += *(string*)arguments[0]->data;
    else
      throw RuntimeTypeException("Cannot add something to a symbol except for Characters and Symbols.");
    return new Sol(String, new string(stringResult));
  };
  SymbolMethods["==:"] = [](Sol *self, vector<Sol*> arguments) {
    string a = *(string*)self->data;
    typeCheck(arguments[0], Symbol);
    return a == *(string*)arguments[0]->data ? True : False;
  };
  SymbolMethods["<:"] = [](Sol *self, vector<Sol*> arguments) {
    string a = *(string*)self->data;
    typeCheck(arguments[0], Symbol);
    return a < *(string*)arguments[0]->data ? True : False;
  };
  //map<MethodName, StandardSolFunction> VectorMethods;
  VectorMethods["initialize"] = [](Sol *self, vector<Sol*> arguments) {
    self->data = new vector<Sol*>();
    return self;
  };
  VectorMethods["deallocate"] = [](Sol *self, vector<Sol*> arguments) {
    delete (vector<Sol*>*)self->data;
    return self;
  };
  VectorMethods["size"] = [](Sol *self, vector<Sol*> arguments) {
    vector<Sol*> v = *(vector<Sol*>*)self->data;
    return new Sol(Integer, new int(v.size()));
  };
  VectorMethods["at:"] = [](Sol *self, vector<Sol*> arguments) {
    typeCheck(arguments[0], Integer);
    vector<Sol*> v = *(vector<Sol*>*)self->data;
    int index = *(int*)arguments[0]->data;
    if(0 <= index and index < v.size())
      return v[index];
    else
      throw RuntimeException("Out of boundary while indexing a Vector.");
  };
  VectorMethods["at:set:"] = [](Sol *self, vector<Sol*> arguments) {
    typeCheck(arguments[0], Integer);
    vector<Sol*> v = *(vector<Sol*>*)self->data;
    int index = *(int*)arguments[0]->data;
    if(0 <= index and index < v.size()) {
      arguments[1]->reference();
      v[index]->dereference();
      return v[index] = arguments[1];
    } else
      throw RuntimeException("Out of boundary string indexing a Vector.");
  };
  VectorMethods["toString"] = [](Sol *self, vector<Sol*> arguments) {
    Sol *result = new Sol(String);
    vector<Sol*> v = *(vector<Sol*>*)self->data;
    string s;
    if(v.size() == 0)
      s = "";
    else {
      for(int i=0; i<v.size()-1; i++) {
	Sol *vs = v[i]->send("toString");
	typeCheck(vs, String);
	s += *(string*)vs->data + ", ";
	vs->finalize();
      }
      Sol *vs = v.back()->send("toString");
      typeCheck(vs, String);
      s += *(string*)vs->data;
      vs->finalize();
    }
    result->data = new string("["+s+"]");
    return result;
  };
  VectorMethods["push:"] = [](Sol *self, vector<Sol*> arguments) {
    vector<Sol*> *v = (vector<Sol*>*)self->data;
    arguments[0]->reference();
    v->push_back(arguments[0]);
    return self;
  };
}

void classReification() {
  for(auto k : Classes)
    k.klass = new Sol();
  for(auto k : Classes) {
    *(k.klass) = *(new Sol(Class));
    k.klass->setVariable("superclass", Object);
    Sol *methodDictionary = new Sol(SymbolDictionary);
    methodDictionary->data = new map<string, Sol*>();
    k.klass->setVariable("methodDictionary", methodDictionary);
    Sol *instanceVariables = new Sol(Vector);
    instanceVariables->data = new vector<Sol*>();
    k.klass->setVariable("instanceVariables", instanceVariables);
    Sol *name = new Sol(String);
    name->data = new string(k.name);
    k.klass->setVariable("name", name);
    
    for(pair<string, StandardSolFunction> p : k.methods) {
      Sol *method = new Sol(BuiltinMethod);
      method->data = new StandardSolFunction(p.second);
      method->reference();
      ((map<string,Sol*>*)methodDictionary->data)->operator[](p.first) = method;
    }
  }
  Object->setVariable("superclass", nullptr);
  /*Class->getVariable("instanceVariables")->send("push:",{UTILITY_createSymbol("superclass")});
    Class->getVariable("instanceVariables")->send("push:",{UTILITY_createSymbol("methodDictionary")});
    Class->getVariable("instanceVariables")->send("push:",{UTILITY_createSymbol("instanceVariables")});
    Class->getVariable("instanceVariables")->send("push:",{UTILITY_createSymbol("superclass")});*/
}

void initializeGlobalEnvironment(Environment *globalEnvironment) {
  for(auto k : Classes)
    globalEnvironment->linkVariable(k.name, &k.klass); 
  for(auto k : SelfDescriptiveClasses) 
    globalEnvironment->linkVariable(k.name, &k.klass);
}
