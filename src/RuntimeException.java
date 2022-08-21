public class RuntimeException extends SolException {
    RuntimeException(String message) {
        super("Runtime", message);
    }
    RuntimeException(Atom atom, String message) {
        super("Runtime", atom.getLocation(), message);
    }
}
