public class Lambda {
    Atom body;
    String[] parameters;
    Sol lexicalSelf;
    Environment environment;
    Lambda(Atom body, String[] parameters, Sol lexicalSelf, Environment environment) {
	this.body = body;
	this.parameters = parameters;
	this.lexicalSelf = lexicalSelf;
	this.environment = environment;
    }
    public void eval(Sol self, Sol[] arguments, Sol contextClass) throws EvaluationRequest, RuntimeException {
	Environment lambdaEnvironment = new Environment(environment);
	if(arguments.length != parameters.length)
	    throw new RuntimeException("Wrong number of arguments passed to lambda. (Expected " +
				       Integer.toString(parameters.length) + " but got " +
				       Integer.toString(arguments.length) + ".)");
	for(int i=0; i<parameters.length; i++)
	    lambdaEnvironment.define(parameters[i], arguments[i]);

	throw new EvaluationRequest(body, self, contextClass, lambdaEnvironment);
    }
    public void eval(Sol self, Sol[] arguments) throws EvaluationRequest, RuntimeException {
	eval(self, arguments, Sol.getClass(self));
    }
    public void eval(Sol[] arguments) throws EvaluationRequest, RuntimeException {
	eval(lexicalSelf, arguments);
    }
    public void eval(Sol self) throws EvaluationRequest, RuntimeException {
	eval(self, new Sol[]{});
    }
    public void eval() throws EvaluationRequest, RuntimeException {
	eval(lexicalSelf);
    } 
}
