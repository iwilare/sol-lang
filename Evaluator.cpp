Sol eval(Atom *a, Sol self, Sol contextClass, Environment environment) {
 tailRecursionStart:
#ifdef Log
  LogEvaluation<<a->location.toString()<<" ";
  evaluationStackIndent();
  LogEvaluation<<a->toString()<<LogEnd;
#endif
  if(a == nullptr) {
    return nullptr;
  }
  Sol returnValue;
  switch(a->type) {
  case Atom::Type::messageT: {
    GlobalEvaluationStackCount++;
    Sol receiver, klass;
    vector<Sol> arguments;
    if(a->message.receiver->type == Atom::Type::identifierT and
       a->message.receiver->identifier == "super") {
      if(contextClass == nullptr)
	throw RuntimeException(/*a, */"Cannot refer to super while not in a valid context.");
      receiver = self;
      for(Atom *argument : a->message.arguments)
	arguments.push_back(eval(argument, self, contextClass, environment));
      klass = contextClass->getVariable("superclass");
    } else {
      receiver = eval(a->message.receiver, self, contextClass, environment);
      for(Atom *argument : a->message.arguments)
	arguments.push_back(eval(argument, self, contextClass, environment));
      klass = receiver->getClass();
    }
    try {
      GlobalEvaluationStackCount--;
      return receiver->sendWER(a->message.message, arguments, klass);	
    } catch(EvaluationRequest request) {
      a            = std::move(request.atom);
      self         = std::move(request.self);
      contextClass = std::move(request.contextClass);
      environment  = std::move(request.environment);
      
      LogEvaluation<<"Catching "<<LogEnd;
      goto tailRecursionStart;
    }
  }
  case Atom::Type::identifierT:
    if(a->identifier == "self")
      if(self == nullptr)
	throw RuntimeException(/*a, */"Cannot refer to self while not in a valid context.");
      else
	return self;
    else if(a->identifier == "super")
      throw RuntimeException(/*a, */"Cannot refer to super as a standalone variable.");
    else if(a->identifier == "nothing")
      return (Sol)nullptr;
    else if(environment->has(a->identifier))
      return environment->get(a->identifier);
    else if(self != nullptr and self->hasVariable(a->identifier))
      return self->getVariable(a->identifier);
    else
      throw EnvironmentException(environment, a->location, a->identifier);
  case Atom::Type::assignmentT: {
    GlobalEvaluationStackCount++;
    Sol value = eval(a->assignment.value, self, contextClass, environment);
    GlobalEvaluationStackCount--;
    string variable = a->assignment.variable;
    if(self != nullptr and self->hasVariable(variable))
      self->setVariable(variable, value);
    else if(environment->has(variable))
      environment->set(variable, value);
    else
      environment->define(variable, value);
    return self;
  }
  case Atom::Type::sequenceT: {
    int i;
    GlobalEvaluationStackCount++;
    for(i=0; i<a->sequence.size()-1; i++)
      eval(a->sequence[i], self, contextClass, environment);
    GlobalEvaluationStackCount--;
    a = a->sequence[i];
    goto tailRecursionStart;
  }
  case Atom::Type::lambdaT: {
    return SolCreate(Lambda, new LambdaStructure(a->lambda.body,
						 a->lambda.parameters,
						 self,
						 environment));
  }
  case Atom::Type::vectorT: {
    vector<Sol> elements;
    GlobalEvaluationStackCount++;
    for(Atom *element : a->vectorElements) {
      Sol elementObject = eval(element, self, contextClass, environment);
      elements.push_back(elementObject);
    }
    GlobalEvaluationStackCount--;
    return SolCreate(Vector, new vector<Sol>(elements));
  }
  case Atom::Type::symbolT:
    return SolCreate(Symbol, new string(a->symbol)); 
  case Atom::Type::integerT:
    return SolCreate(Integer, new int(a->integerValue)); 
  case Atom::Type::doubleT:
    return SolCreate(Double, new double(a->doubleValue)); 
  case Atom::Type::characterT:
    return SolCreate(Character, new char(a->characterValue)); 
  case Atom::Type::stringT:
    return SolCreate(String, new string(a->stringValue)); 
  default:
    cerr<<"Fatal error, unrecognized Atom."<<endl;
  }
}
inline Sol eval(Atom *atom, Environment environment) {
  return eval(atom, nullptr, nullptr, environment);
}
