class RuntimeException : public SolException {
public:
  RuntimeException(string message) :
    SolException("Runtime", message) {}
  RuntimeException(Atom *atom, string message) :
    SolException("Runtime", atom->location, message) {}
};

class RuntimeTypeException : public SolException {
public:
  RuntimeTypeException(string message);
  RuntimeTypeException(Sol *object, Sol *klass);
};

string UTILITY_getClassName(Sol *klass) {
  Sol *nameKlass = klass->getVariable("name");
  if(nameKlass->klass != String)
    throw RuntimeTypeException("Expected String type for the class name during exception.");
  return *(string*)nameKlass->data;
}
string UTILITY_getObjectTypeName(Sol *object) {
  if(object == nullptr)
    return UTILITY_getClassName(Nothing);
  else
    return UTILITY_getClassName(object->klass);
}
void typeCheck(Sol *a, Sol *klass) {
  if(a->klass == klass)
    return;
  else
    throw RuntimeTypeException(a, klass);
}
Sol *UTILITY_createSymbol(string s) {
  Sol *result = Symbol->send(Message("basicNew"),{});
  result->data = new string(s);
  return result;
}
Sol *UTILITY_createString(string s) {
  Sol *result = String->send(Message("basicNew"),{});
  result->data = new string(s);
  return result;
}

RuntimeTypeException::RuntimeTypeException(string message) :
  SolException("RuntimeType", message) {}
RuntimeTypeException::RuntimeTypeException(Sol *object, Sol *klass) :
  SolException("RuntimeType", "Unexpected type \"" + UTILITY_getObjectTypeName(object) +
	       "\", expected \"" + UTILITY_getClassName(klass) + "\".") {}  

class ObjectVariableException : public SolException {
public:
  ObjectVariableException(Sol *object, string variable) :
    SolException("ObjectVariable", "Undefined variable " + variable + " in an instance of " + UTILITY_getObjectTypeName(object->klass) + ".") {}
  ObjectVariableException(Sol *object, Location location, string variable) :
    SolException("ObjectVariable", location,
		 "Undefined variable " + variable + " in an instance of " +
		 UTILITY_getObjectTypeName(object->klass) + ".") {}
};
