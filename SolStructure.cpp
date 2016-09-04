inline Sol SolS::sendWER(Message message, vector<Sol> arguments) {
  return sendWER(message, arguments, getClass());
}
Sol SolS::sendWER(Message message, vector<Sol> arguments, Sol startingClass) {
#ifdef Log
  LogMessages<<this<<" "<<message.toString()<<" [ ";
  for(Sol a : arguments)
    LogMessages<<a.get()<<" ";
  LogMessages<<"]"<<LogEnd;
#endif
  
  Sol klass;
  if(this == nullptr)
    klass = Nothing;
  else if(startingClass == nullptr)
    throw RuntimeException("Attempting to send " + message.toString() + " to a classless Object.");
  klass = startingClass;
  while(klass != nullptr) {
    Sol methodsObject = klass->getVariable("methods");
    map<string, Sol> *methodMap = methodsObject->data<map<string, Sol> >(SymbolDictionary);
    if(methodMap->count(message.toString()) == 1) {
      Sol method = methodMap->operator[](message.toString());
      if(method == nullptr)
	break;
      else if(method->getClass() == BuiltinMethod) {
	if(message.parametersNumber != arguments.size())
	  throw RuntimeException("Attempting to send " + message.toString() + " to an instance of " +
				 UTILITY_getObjectTypeName(Sol(this)) + " with " +
				 intToString(arguments.size()) + " arguments, expected " +
				 intToString(message.parametersNumber) + ".");
	StandardSolFunction actualMethod = method->getData<StandardSolFunction>(BuiltinMethod);
	return actualMethod(Sol(this), arguments);
      } else if(method->getClass() == Lambda) {
	LambdaStructure *lambda = method->data<LambdaStructure>(Lambda);
	lambda->evalLambda(Sol(this), arguments, klass);
      } else {
	Sol argumentsObject = SolCreate(Vector, new vector<Sol>(arguments));
	return method->sendWER(Message("valueIn:with:"), {Sol(this), argumentsObject});
      }
    }
    klass = klass->getVariable("superclass");
  }
  if(this->getClass() == startingClass)
    return Sol(this)->sendWER(Message("doesNotUnderstand:with:"), {
	  SolCreate(Symbol, new string(message.toString())),
	  SolCreate(Vector, new vector<Sol>(arguments))});
  else
    return Sol(this)->sendWER(Message("doesNotUnderstand:with:as:"), {
	  SolCreate(Symbol, new string(message.toString())),
	  SolCreate(Vector, new vector<Sol>(arguments)),
	  startingClass});
}
inline Sol SolS::send(Message message, vector<Sol> arguments, Sol startingClass) {
  try {
    return sendWER(message, arguments, startingClass);
  } catch(EvaluationRequest request) {
    return eval(request.atom,
		request.self,
		request.contextClass,
		request.environment);
  }
}
inline Sol SolS::send(Message message, vector<Sol> arguments) {
  return send(message, arguments, false);
}
inline Sol SolS::superSend(Message message, vector<Sol> arguments) {
  return send(message, arguments, true);
}
inline Sol SolS::send(Message message, vector<Sol> arguments, bool superMessage) {
  if(this == nullptr)
    return send(message, arguments, Nothing);
  else if(klass == nullptr)
    throw RuntimeException("Attempting to send " + message.toString() + " to a classless Object.");
  else if(superMessage)
    return send(message, arguments, klass->getVariable("superclass"));
  else
    return send(message, arguments, klass);
}

inline Sol SolS::send(string message) {
  return send(message,{});
}
inline Sol SolS::send(string message, vector<Sol> arguments) {
  return send(Message(message), arguments);
}
inline Sol SolS::superSend(string message) {
  return superSend(message,{});
}
inline Sol SolS::superSend(string message, vector<Sol> arguments) {
  return superSend(Message(message), arguments);
}
Sol SolS::getClass() {
  return this == nullptr ? Nothing : klass;
}
void SolS::setClass(Sol klass) {
  if(Sol(this) == nullptr)
    throw RuntimeException("Trying to change nothing class.");
  this->klass = klass;
}
bool SolS::hasVariable(string variable) { return variables.count(variable) > 0; }
Sol SolS::getVariable(string variable) {
  if(variables.count(variable) == 0)
    throw ObjectVariableException(Sol(this), variable);
  else
    return variables[variable];
}
void SolS::defineVariable(string variable) {
  variables[variable] = nullptr;
}
void SolS::defineVariable(string variable, Sol value) {
  defineVariable(variable);
  setVariable(variable, value);
}
void SolS::setVariable(string variable, Sol value) {
  if(!hasVariable(variable))
    throw ObjectVariableException(Sol(this), variable);
  variables[variable] = value;
}
template<typename T> T *SolS::data(Sol klass) {
  if(getClass() == klass) return (T*)internalData;
  else throw RuntimeTypeException(Sol(this), klass);
}
void SolS::printeronos() {
  cout<<"----- "<<this<<" ------"<<endl;
  cout<<"Class: ";
  cout<<klass.get()<<endl;
  for(auto k : variables)
    cout<<k.first<<": "<<k.second.get()<<endl;
  cout<<"-----------"<<endl;
}
SolS::~SolS() {
  if(this == nullptr)
    return;
  if(klass == Integer)               delete (int*)internalData;
  else if(klass == Double)           delete (double*)internalData;
  else if(klass == Character)        delete (char*)internalData;
  else if(klass == Lambda)           delete (LambdaStructure*)internalData;
  else if(klass == String)           delete (string*)internalData;
  else if(klass == Symbol)           delete (string*)internalData;
  else if(klass == SymbolDictionary) delete (map<string,Sol>*)internalData;
  else if(klass == BuiltinMethod)    delete (StandardSolFunction*)internalData;    
}
