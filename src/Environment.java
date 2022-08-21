import java.util.*;

class EnvironmentException extends SolException {
    EnvironmentException(Environment env, String message) {
        super("Environment", message);
    }
    EnvironmentException(Environment env, Location location, String message) {
        super("Environment", location, message); //"Undefined variable \"" + variable + "\".");
    }
};
public class Environment {
    Environment outer;
    HashMap<String,Sol> environment;
    HashMap<String,Reference<Sol>> linkEnvironment;

    Environment() {
        this.environment = new HashMap<String,Sol>();
        this.linkEnvironment = new HashMap<String,Reference<Sol>>();
    }
    Environment(Environment outer) {
        this.outer = outer;
        this.environment = new HashMap<String,Sol>();
        this.linkEnvironment = new HashMap<String,Reference<Sol>>();
    }

    public void link(String variable, Reference<Sol> value) { linkEnvironment.put(variable, value); }
    public void define(String variable, Sol value) { environment.put(variable, value); }
    public void set(String variable, Sol value) {
        if(outer == null || !outer.has(variable))
            if(linkEnvironment.containsKey(variable))
                linkEnvironment.get(variable).set(value);
            else
                environment.put(variable, value);
        else
            outer.set(variable, value);
    }
    public boolean has(String variable) {
        if(environment.containsKey(variable))
            return true;
        else if(linkEnvironment.containsKey(variable))
            return true;
        else if(outer == null)
            return false;
        else
            return outer.has(variable);
    }
    public Sol get(String variable) { return get(new Location(), variable); }
    public Sol get(Atom atom) { return get(atom.getLocation(), ((IdentifierAtom)atom).getIdentifier()); }
    public Sol get(Location location, String variable) {
        if(environment.containsKey(variable))
            return environment.get(variable);
        if(linkEnvironment.containsKey(variable))
            return linkEnvironment.get(variable).get();
        else if(outer == null)
            return null;
        else
            return outer.get(location, variable);
    }
}
