class Environment;
class EnvironmentException : public SolException {
public:
  EnvironmentException(Environment *env, string message) :
    SolException("Environment", message) {}
  EnvironmentException(Environment *env, Location location, string message) :
    SolException("Environment", location, message) {}
};
class Environment : public Collectable {
public:
private:
  Environment *outer;
  map<string, Sol*> env;
  map<string, Sol**> linkEnv;
  Sol *actualSelf;
public:
  Environment() : env(), linkEnv(), outer(nullptr), actualSelf(nullptr) {}
  Environment(Environment *outer) :
    env(), outer(outer), actualSelf(nullptr) {
    outer->reference();
  }
  Environment(Environment *outer, Sol *actualSelf) :
    env(), outer(outer), actualSelf(actualSelf) {
    outer->reference();
    actualSelf->reference();
  }
  void linkVariable(string variable, Sol **location) {
    (*location)->reference();
    linkEnv[variable] = location;
  }
  void defineVariable(string variable, Sol *value) {
    value->reference();
    env[variable] = value;
  }
  void setVariable(string variable, Sol *value) {
    value->reference();
    if(actualSelf != nullptr and actualSelf->hasVariable(variable))
      actualSelf->setVariable(variable, value);
    else if(linkEnv.count(variable) > 0) {
      if(linkEnv.count(variable) > 0)
	(*(linkEnv[variable]))->dereference();
      *(linkEnv[variable]) = value;
    } else if(outer == nullptr or !outer->hasVariable(variable)) {
      if(env.count(variable) > 0) 
	env[variable]->dereference();
      env[variable] = value;
    } else
      outer->setVariable(variable, value);
  }
  bool hasVariable(string variable) {
    if(actualSelf != nullptr and actualSelf->hasVariable(variable))
      return true;
    else if(env.count(variable) > 0 or linkEnv.count(variable) > 0)
      return true;
    else if(outer == nullptr)
      return false;
    else
      return outer->hasVariable(variable);
  }
  Sol *getVariable(Atom *a) {
    return getVariable(a->location, a->identifier);
  }
  Sol *getVariable(string s) {
    return getVariable(Location(), s);
  }
  Sol *getVariable(Location location, string s) {
    if(actualSelf != nullptr and actualSelf->hasVariable(s))
      return actualSelf->getVariable(s);
    else if(env.count(s) > 0)
      return env[s];
    else if(linkEnv.count(s) > 0)
      return *(linkEnv[s]);
    else if(outer == nullptr)
      throw EnvironmentException(this, location, "Undefined variable \"" + s + "\".");
    else
      return outer->getVariable(s);
  }
  Sol *getSelf() {
    if(actualSelf != nullptr)
      return actualSelf;
    else if(outer == nullptr)
      return nullptr;
    else
      return outer->getSelf();
  }
  string toString() {
    string s = "{";
    if(actualSelf != nullptr)
      s += "self";
    if(outer != nullptr)
      s += outer->toString();
    s += "|";
    for(pair<string, Sol*> p : env)
      s += " " + p.first;
    return s + "}";
  }
  void recursiveDereferenceStep() {
    if(actualSelf != nullptr)
      actualSelf->dereferenceStep();
    for(auto p : env)
      p.second->dereferenceStep();
    for(auto p : linkEnv)
      (*p.second)->dereferenceStep();
    if(outer != nullptr)
      outer->dereferenceStep();
  }
};
