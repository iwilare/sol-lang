int GlobalLLID = 0;
Sol *eval(int LLID, Atom *a, Environment *e) {
 tailRecursionStart:
  if(a == nullptr)
    return nullptr;
  switch(a->type) {
  case Atom::Type::messageT: {
    Sol *receiver;
    if(a->message.receiver->type == Atom::Type::identifierT and
       a->message.receiver->identifier == "super") {
      if((receiver = e->getSelf()) == nullptr)
	throw EnvironmentException(e, a->location,
				   "Cannot refer to super while not in a valid context.");
      vector<Sol*> evaluatedArguments;      
      for(Atom *argument : a->message.arguments)
        evaluatedArguments.push_back(eval(LLID, argument, e));
      return receiver->superSend(a->message.message, evaluatedArguments);
    } else {
      receiver = eval(LLID, a->message.receiver, e);
      vector<Sol*> evaluatedArguments;
      for(Atom *argument : a->message.arguments)
        evaluatedArguments.push_back(eval(LLID, argument, e));
      return receiver->send(a->message.message, evaluatedArguments);
    }
  }
  case Atom::Type::identifierT:
    if(a->identifier == "self") {
      Sol *self = e->getSelf();
      if(self == nullptr)
	throw EnvironmentException(e, a->location,
				   "Cannot refer to self while not in a valid context.");
      else
	return self;
    } else if(a->identifier == "super")
      throw EnvironmentException(e, a->location,
				 "Cannot refer to super as a standalone variable.");
    else if(a->identifier == "nothing")
      return (Sol*)nullptr;
    else
      return e->getVariable(a);
  case Atom::Type::returnT: {
    Sol *value = eval(LLID, a->returnExpression, e);
    if(LLID == 0)
      return value;
    else
      throw SolContinuation(value, LLID);
  }
  case Atom::Type::assignmentT: {
    Sol *value = eval(LLID, a->assignment.value, e);
    e->setVariable(a->assignment.variable, value);
    return value;
  }
  case Atom::Type::sequenceT: {
    int i;
    for(i=0; i<a->sequence.size()-1; i++)
      eval(LLID, a->sequence[i], e)->finalize();
    a = a->sequence[i];
    goto tailRecursionStart;
  }
  case Atom::Type::lambdaT: {
    Sol *lambdaObject = new Sol(Lambda);
    if(LLID == 0)
      LLID = GlobalLLID++;
    lambdaObject->data = new LambdaStructure(LLID, a->lambda.body,
					     a->lambda.parameters, e);
    return lambdaObject;
  }
  case Atom::Type::vectorT: {
    Sol *vectorObject = new Sol(Vector);
    vector<Sol*> evaluatedElements;
    for(Atom *element : a->vectorElements) {
      Sol *elementObject = eval(LLID, element, e);
      elementObject->reference();
      evaluatedElements.push_back(elementObject);
    }
    vectorObject->data = new vector<Sol*>(evaluatedElements);
    return vectorObject;
  }
  case Atom::Type::symbolT: {
    Sol *symbolObject = new Sol(Symbol);
    symbolObject->data = new string(a->symbol);
    return symbolObject;
  }
  case Atom::Type::integerT: {
    Sol *integerObject = new Sol(Integer);
    integerObject->data = new int(a->integerValue);
    return integerObject;
  }
  case Atom::Type::doubleT: {
    Sol *doubleObject = new Sol(Double);
    doubleObject->data = new double(a->doubleValue);
    return doubleObject;
  }
  case Atom::Type::characterT: {
    Sol *characterObject = new Sol(Character);
    characterObject->data = new char(a->characterValue);
    return characterObject;
  }
  case Atom::Type::stringT: {
    Sol *stringObject = new Sol(String);
    stringObject->data = new string(a->stringValue);
    return stringObject;
  }
  default:
    cerr<<"Fatal error, unrecognized Atom."<<endl;
  }
}
