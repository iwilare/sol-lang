public class SolException extends Exception {
    SolException(String name, Location location, String message) {
        super(name + "@" + location + ":" + message);
    }
    SolException(String name, String message) {
        super(name + ":" + message);
    }
}
