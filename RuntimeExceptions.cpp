int GlobalEvaluationStackCount = -1;
void evaluationStackIndent() {
  for(int i=0; i<GlobalEvaluationStackCount; i++)
    LogEvaluation<<"|> ";
}
/*vector<Atom*> GlobalEvaluationStack;
  Atom *LastExpression;*/

class RuntimeException : public SolException {
public:
  RuntimeException(string source, Location location, string message) :
    SolException(source, location, message) { printStackTrace(); }
  RuntimeException(string source, string message) :
    SolException(source, message) { printStackTrace(); }
  RuntimeException(string message) :
    RuntimeException("Runtime", message) {}
  static void printStackTrace() {
    /*cout<<"-------------- Stack trace -----------"<<endl;
    cout<<LastExpression->toString()<<endl;
        for(int i=1; i<GlobalEvaluationStack.size(); i++)
	  cout<<GlobalEvaluationStack[i]->location.toString()<<"\t"<<GlobalEvaluationStack[i]->toString()<<endl;
    cout<<"--------------------------------------"<<endl;*/
  }
};

class RuntimeTypeException : public RuntimeException {
public:
  RuntimeTypeException(string message);
  RuntimeTypeException(Sol object, Sol klass);
};

string UTILITY_getClassName(Sol klass) {
  if(klass == nullptr)
    return "<nothing>";
  if(!klass->hasVariable("name"))
    return "<Anonymous class>";
  Sol nameKlass = klass->getVariable("name");
  if(nameKlass->getClass() != String)
    return "<Badly named>";
  return nameKlass->getData<string>();
}
string UTILITY_getObjectTypeName(Sol object) {
  if(object == nullptr)
    return UTILITY_getClassName(Nothing);
  else
    return UTILITY_getClassName(object->getClass());
}

RuntimeTypeException::RuntimeypeException(string message) :
  RuntimeException("RuntimeType", message) {}
RuntimeTypeException::RuntimeypeException(Sol object, Sol klass) :
  RuntimeException("RuntimeType", "Unexpected type " + UTILITY_getObjectTypeName(object) +
		   ", expected " + UTILITY_getClassName(klass) + ".") {}  

class ObjectVariableException : public RuntimeException {
public:
  ObjectVariableException(Sol object, string variable) :
    RuntimeException("ObjectVariable", "Undefined variable " + variable + " in an instance of " +
		     UTILITY_getObjectTypeName(object->getClass()) + ".") {}
  ObjectVariableException(Sol object, Location location, string variable) :
    RuntimeException("ObjectVariable", location,
		     "Undefined variable " + variable + " in an instance of " +
		     UTILITY_getObjectTypeName(object->getClass()) + ".") {}
};
