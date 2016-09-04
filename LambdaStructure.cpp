class LambdaStructure {
public:
  Atom *body;
  vector<string> parameters;
  Sol lexicalSelf;
  Environment environment;
  LambdaStructure(Atom *body, vector<string> parameters, Sol lexicalSelf, Environment environment) :
    body(body), parameters(parameters), lexicalSelf(lexicalSelf), environment(environment) {}
  void evalLambda(Sol self, vector<Sol> arguments, Sol contextClass) {
    Environment lambdaEnvironment = EnvironmentCreate(environment);
    LogEvaluation<<"Creating new environment at "<<lambdaEnvironment.get()<<LogEnd;
    if(arguments.size() != parameters.size())
      throw RuntimeException("Wrong number of arguments passed to lambda. (Expected " +
			     intToString(parameters.size()) + " but got " +
			     intToString(arguments.size()) + ".)");
    for(int i=0; i<parameters.size(); i++) 
      lambdaEnvironment->set(parameters[i], arguments[i]);
    
    throw EvaluationRequest(body, self, contextClass, lambdaEnvironment);
  }
  inline void evalLambda(Sol self, vector<Sol> arguments) {
    evalLambda(self, arguments, self->getClass());
  }
  inline void evalLambda(vector<Sol> arguments) {
    evalLambda(lexicalSelf, arguments);
  }
  inline void evalLambda(Sol self) {
    evalLambda(self, {});
  }
  inline void evalLambda() {
    evalLambda(lexicalSelf);
  }
};
