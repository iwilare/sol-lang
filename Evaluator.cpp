Sol eval(Atom *a, Sol self, Sol contextClass, Environment environment) {
 tailRecursionStart:
#ifdef Log
  evaluationStackIndent();
  LogEvaluation<<a->toString()<<LogEnd;
#endif
  if(a == nullptr) {
    return nullptr;
  }
  Sol returnValue;
  switch(a->type) {
  case Atom::Type::message: {
    GlobalEvaluationStackCount++;
    Sol receiver, klass;
    vector<Sol> arguments;
    if(a->message.receiver->type == Atom::Type::identifier and
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
  case Atom::Type::identifier:
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
  case Atom::Type::assignment: {
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
  case Atom::Type::sequence: {
    int i;
    GlobalEvaluationStackCount++;
    for(i=0; i<a->sequence.size()-1; i++)
      eval(a->sequence[i], self, contextClass, environment);
    GlobalEvaluationStackCount--;
    a = a->sequence[i];
    goto tailRecursionStart;
  }
  case Atom::Type::lambda: {
    return SolCreate(Lambda, new LambdaStructure(a->lambda.body,
						 a->lambda.parameters,
						 self,
						 environment));
  }
  case Atom::Type::vector: {
    vector<Sol> elements;
    GlobalEvaluationStackCount++;
    for(Atom *element : a->vectorElements) {
      Sol elementObject = eval(element, self, contextClass, environment);
      elements.push_back(elementObject);
    }
    GlobalEvaluationStackCount--;
    return SolCreate(Vector, new vector<Sol>(elements));
  }
  case Atom::Type::symbol:
    return SolCreate(Symbol, new string(a->symbol)); 
  case Atom::Type::integer:
    return SolCreate(Integer, new int(a->integerValue)); 
  case Atom::Type::double:
    return SolCreate(Double, new double(a->doubleValue)); 
  case Atom::Type::character:
    return SolCreate(Character, new char(a->characterValue)); 
  case Atom::Type::string:
    return SolCreate(String, new string(a->stringValue)); 
  default:
    cerr<<"Fatal error, unrecognized Atom."<<endl;
  }
}
inline Sol eval(Atom *atom, Environment environment) {
  return eval(atom, nullptr, nullptr, environment);
}
