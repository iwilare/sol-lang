class LambdaStructure {
public:
  Atom *body;
  vector<string> parameters;
  Environment *environment;
  int LLID;
  LambdaStructure(int LLID, Atom *body, vector<string> parameters, Environment *environment) :
    LLID(LLID), body(body), parameters(parameters), environment(environment) {
    environment->reference();
  }
};
Sol *evalLambda(LambdaStructure *lambda, Sol *self, vector<Sol*> arguments, bool catchReturn) {
  Environment *lambdaEnvironment = new Environment(lambda->environment, self);
  // Check possibly memory leak in environment
  if(arguments.size() != lambda->parameters.size())
    throw RuntimeException("Wrong number of arguments passed to lambda. (Expected " +
			   to_string(lambda->parameters.size()) + " but got " +
			   to_string(arguments.size()) + ".)");
  for(int i=0; i<lambda->parameters.size(); i++) 
    lambdaEnvironment->setVariable(lambda->parameters[i], arguments[i]);
  self->reference();
  lambdaEnvironment->reference();
  for(Sol *argument : arguments)
    argument->reference();
  Sol *result;
  if(!catchReturn)
    result = eval(lambda->LLID, lambda->body, lambdaEnvironment);
  else
    try {
      result = eval(lambda->LLID, lambda->body, lambdaEnvironment);
    } catch(SolContinuation c) {
      if(c.LLID == lambda->LLID)
	return c.returnValue;
      else
	throw c;
    }
  result->reference();
  self->dereference();
  lambdaEnvironment->dereference();
  for(Sol *argument : arguments)
    argument->dereference();
  result->dereferenceProtect();
  return result;
}
inline Sol *evalLambda(LambdaStructure *lambda, Sol *self, vector<Sol*> arguments) {
  return evalLambda(lambda, self, arguments, false);
}
inline Sol *evalLambda(LambdaStructure *lambda) {
  return evalLambda(lambda, nullptr, {});
}
inline Sol *evalLambda(LambdaStructure *lambda, Sol *arguments) {
  return evalLambda(lambda, {arguments});
}
inline Sol *evalLambda(LambdaStructure *lambda, vector<Sol*> arguments) {
  return evalLambda(lambda, nullptr, arguments);
}
