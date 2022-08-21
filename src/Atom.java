import java.util.*;

abstract class Atom {
    Location location;

    public void setLocation(Location location) { this.location = location; }
    public Location getLocation() { return location; }
    public String completeToString() {
        return location.toString() + "    " + toString();
    }
    public static String vectorToString(Atom[] vector, String sep) {
        if(vector.length == 0)
            return "";
        String s = "";
        for(int i=0; i<vector.length-1; i++)
            s += vector[i].toString() + sep;
        s += vector[vector.length-1];
        return s;
    }
}
abstract class MessageAtom extends Atom {
    Atom receiver;
    public Atom getReceiver() { return this.receiver; }
    public abstract Message getMessage();
    public abstract Atom[] getArguments();
}
class UnaryMessageAtom extends MessageAtom {
    UnaryMessage message;
    UnaryMessageAtom(Atom receiver, UnaryMessage message) {
        this.receiver = receiver;
        this.message = message;
    }
    public Message getMessage() { return this.message; }
    public Atom[] getArguments() { return new Atom[]{}; }

    public String toString() { return "(" + receiver + ") " + message; }
}
class BinaryMessageAtom extends MessageAtom {
    BinaryMessage message;
    Atom argument;
    BinaryMessageAtom(Atom receiver, BinaryMessage message, Atom argument) {
        this.receiver = receiver;
        this.message = message;
        this.argument = argument;
    }
    public Message getMessage() { return this.message; }
    public Atom[] getArguments() { return new Atom[]{argument}; }
    public String toString() { return "(" + receiver + ") " + message + " (" + argument + ")"; }
}
class KeywordMessageAtom extends MessageAtom {
    KeywordMessage message;
    Atom[] arguments;
    KeywordMessageAtom(Atom receiver, KeywordMessage message, Atom arguments[] ) {
        this.receiver = receiver;
        this.message = message;
        this.arguments = arguments;
    }
    public Message getMessage() { return this.message; }
    public Atom[] getArguments() { return this.arguments; }
    public String toString() {
        String s = "(" + receiver + ")";
        String[] keywords = message.getKeywords();
        for(int i=0; i<keywords.length; i++)
            s += " " + keywords[i] + ": (" + arguments[i] + ")";
        return s;
    }
}
class IdentifierAtom extends Atom {
    String identifier;
    IdentifierAtom(String identifier) { this.identifier = identifier; }
    public String getIdentifier() { return this.identifier; }
    public String toString() { return identifier; }
}
class LambdaAtom extends Atom {
    Atom body;
    String[] parameters;
    LambdaAtom(Atom body, String[] parameters) {
        this.body = body;
        this.parameters = parameters;
    }
    public Atom getBody() { return this.body; }
    public String[] getParameters() { return this.parameters; }
    public String toString() {
        return "{" + (parameters.length > 0 ? Arrays.toString(parameters) + " | " : "") +
            (body == null ? "" : body)+ "}"; }
}
class SequenceAtom extends Atom {
    Atom[] sequence;
    SequenceAtom(Atom[] sequence) {
        this.sequence = sequence;
    }
    public Atom[] getSequence() { return this.sequence; }
    public String toString() { return "(" + Atom.vectorToString(sequence, ". ") + ")"; }
}
class AssignmentAtom extends Atom {
    String variable;
    Atom expression;
    AssignmentAtom(String variable, Atom expression) {
        this.variable = variable;
        this.expression = expression;
    }
    public String getVariable() { return this.variable; }
    public Atom getExpression() { return this.expression; }
    public String toString() { return variable + " := " + expression; }
}
class VectorAtom extends Atom {
    Atom[] vector;
    VectorAtom(Atom[] vector) {
        this.vector = vector;
    }
    public Atom[] getVector() { return this.vector; }
    public String toString() { return "[" + Atom.vectorToString(vector, ",") + "]"; }
}
class IntegerAtom extends Atom {
    int value;
    IntegerAtom(int value) { this.value = value; }
    public int getInteger() { return this.value; }
    public String toString() { return Integer.toString(value); }
    public void negateValue() { this.value *= -1; }
}
class DoubleAtom extends Atom {
    double value;
    DoubleAtom(double value) { this.value = value; }
    public double getDouble() { return this.value; }
    public String toString() { return Double.toString(value); }
    public void negateValue() { this.value *= -1; }
}
class CharacterAtom extends Atom {
    char value;
    CharacterAtom(char value) { this.value = value; }
    public char getCharacter() { return this.value; }
    public String toString() { return "'" + Character.toString(value) + "'"; }
}
class StringAtom extends Atom {
    String value;
    StringAtom(String value) { this.value = value; }
    public String getString() { return this.value; }
    public String toString() { return "\"" + value + "\""; }
}
class MessageSymbolAtom extends Atom {
    Message message;
    MessageSymbolAtom(Message message) { this.message = message; }
    public Message getMessage() { return this.message; }
    public String toString() { return "#" + message; }
}
