class EnvironmentException : public SolException {
public:
  EnvironmentException(Environment env, string message) :
    SolException("Environment", message) {}
  EnvironmentException(Environment env, Location location, string variable) :
    SolException("Environment", location, "Undefined variable \"" + variable + "\".") {}
};
class EnvironmentS : public Collectable {
public:
private:
  Environment outer;
  map<string, Sol> env;
  map<string, Sol*> linkEnv;
public:
  EnvironmentS() : env(), linkEnv(), outer(nullptr) {}
  EnvironmentS(Environment outer) :
    env(), outer(outer) {
  }
  void link(string variable, Sol *location) {
    linkEnv[variable] = location;
  }
  void define(string variable, Sol value) {
    env[variable] = value;
  }
  void set(string variable, Sol value) {
    if(linkEnv.count(variable) > 0)
      *(linkEnv[variable]) = value;
    else if(outer == nullptr or !outer->has(variable))
      env[variable] = value;
    else
      outer->set(variable, value);
  }
  bool has(string variable) {
    if(env.count(variable) > 0 or linkEnv.count(variable) > 0)
      return true;
    else if(outer == nullptr)
      return false;
    else
      return outer->has(variable);
  }
  Sol get(Atom *a) {
    return get(a->location, a->identifier);
  }
  Sol get(string s) {
    return get(Location(), s);
  }
  Sol get(Location location, string variable) {
    if(env.count(variable) > 0)
      return env[variable];
    else if(linkEnv.count(variable) > 0)
      return *(linkEnv[variable]);
    else if(outer == nullptr)
      throw EnvironmentException(Environment(this), location, variable);
    else
      return outer->get(variable);
  }
  Sol getSelf() {
    if(outer == nullptr)
      return nullptr;
    else
      return outer->getSelf();
  }
  string toString() {
    string s;
    if(outer != nullptr)
      s += outer->toString() + "+";
    s += "{";
    for(pair<string, Sol> p : env)
      s += " " + p.first;
    return s + " }";
  }
};
Environment EnvironmentCreate() {
  return Environment(new EnvironmentS());
}
Environment EnvironmentCreate(Environment outer) {
  return Environment(new EnvironmentS(outer));
}
