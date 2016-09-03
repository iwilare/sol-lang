typedef Sol *(*StandardSolFunction)(Sol *self, vector<Sol*> arguments);

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
