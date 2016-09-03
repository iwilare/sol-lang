Sol *Sol::genericSend(Message message, vector<Sol*> arguments, bool superMessage) {
  Sol *currentClass;
  if(this == nullptr)
    currentClass = Nothing;
  else if(superMessage)
    currentClass = klass->getVariable("superclass");
  else
    currentClass = klass;
  while(currentClass != nullptr) {
    Sol *methodDictionary = currentClass->getVariable("methodDictionary");
    typeCheck(methodDictionary, SymbolDictionary);
    map<string, Sol*> methodMap = *(map<string, Sol*>*)methodDictionary->data;
    if(methodMap.count(message.toString()) == 1) {
      Sol *method = methodMap[message.toString()];
      if(method->klass == BuiltinMethod) {
	StandardSolFunction actualMethod = *(StandardSolFunction*)method->data;
	//cout<<"@@@@@@@@@@@@@@@ START "<<message.toString()<<" @@@@@@@@@@"<<endl;
	this->reference();
	for(Sol *argument : arguments) 
	  argument->reference();
	Sol *result = actualMethod(this, arguments);
	result->reference();
	this->dereference();
	for(Sol *argument : arguments) 
	  argument->dereference();
	result->dereferenceProtect();
	return result;
      } else if(method->klass == Lambda) {
	LambdaStructure *lambda = (LambdaStructure*)method->data;
	return evalLambda(lambda, this, arguments, true);
      } else {
	Sol *argumentsObject = new Sol(Vector);
	argumentsObject->data = new vector<Sol*>(arguments);
	return method->send("valueIn:with:", {this, argumentsObject});
      }
    }
    currentClass = currentClass->getVariable("superclass");
  }
  Sol *messageObject = UTILITY_createSymbol(message.toString());
  return this->send("doesNotUnderstand:", {messageObject});
}
inline Sol *Sol::send(string message) {
  return send(message,{});
}
inline Sol *Sol::send(string message, vector<Sol*> arguments) {
  return send(Message(message), arguments);
}
inline Sol *Sol::send(Message message, vector<Sol*> arguments) {
  return genericSend(message, arguments, false);
}
inline Sol *Sol::superSend(string message) {
  return superSend(message,{});
}
inline Sol *Sol::superSend(string message, vector<Sol*> arguments) {
  return superSend(Message(message), arguments);
}
inline Sol *Sol::superSend(Message message, vector<Sol*> arguments) {
  return genericSend(Message(message), arguments, true);
}
bool Sol::hasVariable(string variable) { return variables.count(variable) > 0; }
Sol *Sol::getVariable(string variable) {
  if(variables.count(variable) == 0)
    throw ObjectVariableException(this, variable);
  else
    return variables[variable];
}
Sol *Sol::getVariable(Location location, string variable) {
  if(variables.count(variable) == 0)
    throw ObjectVariableException(this, location, variable);
  else
    return variables[variable];
}
void Sol::setVariable(string variable, Sol *value) {
  value->reference();
  if(hasVariable(variable))
    variables[variable]->dereference();
  variables[variable] = value;
}
void Sol::printeronos() {
  cout<<"----- "<<this<<" ------"<<endl;
  cout<<"KLASS::::";
  cout<<klass<<endl;
  for(auto k : variables)
    cout<<k.first<<": "<<k.second<<endl;
  cout<<"-----------"<<endl;}
void Sol::recursiveDereferenceStep() {
  klass->dereferenceStep();
  for(auto p : variables)
    if(p.second != nullptr)
      p.second->dereferenceStep();
  if(klass == SymbolDictionary) {
    map<string, Sol*> dictionary = *(map<string, Sol*>*)data;
    for(auto p : dictionary)
      p.second->dereferenceStep(); 
  } else if(klass == Lambda) {
    LambdaStructure *lambda = (LambdaStructure*)data;
    lambda->environment->dereferenceStep();
  } else if(klass == Vector) {
    vector<Sol*> v = *(vector<Sol*>*)data;
    for(Sol *element : v)
      element->dereferenceStep();
  }
}
