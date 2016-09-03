public class EvaluationRequest extends Exception {
    Atom atom;
    Sol self;
    Sol contextClass;
    Environment environment;
    EvaluationRequest(Atom atom, Sol self, Sol contextClass, Environment environment) {
	this.atom = atom;
	this.self = self;
	this.contextClass = contextClass;
	this.environment = environment;
    }
    public Sol eval() throws Exception {
	return Evaluator.eval(atom, self, contextClass, environment);
    }
}
