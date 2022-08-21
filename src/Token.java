abstract class Token {
    Location location;
    public void setLocation(Location location) { this.location = location; }
    public Location getLocation() { return location; }
    public String toCompleteString() {
        return location.toString() + "    " + toString();
    }
    public boolean isObjectStart() {
        return
            this instanceof BinaryOperatorToken && ((BinaryOperatorToken)this).getBinaryOperator().equals("-")
            || this instanceof IdentifierToken
            || this instanceof IntegerToken
            || this instanceof DoubleToken
            || this instanceof CharacterToken
            || this instanceof StringToken
            || this instanceof MessageToken
            || this instanceof LambdaStartToken
            || this instanceof MethodMarkToken
            || this instanceof VectorStartToken
            || this instanceof ParenthesisOpenToken;
    }
}
class IntegerToken extends Token {
    int value;
    IntegerToken(int value) { this.value = value; }
    public int getInteger() { return this.value; }
    public String toString() { return "Integer " + Integer.toString(value); }
}
class DoubleToken extends Token {
    double value;
    DoubleToken(double value) { this.value = value; }
    public double getDouble() { return this.value; }
    public String toString() { return "Double " + Double.toString(value); }
}
class CharacterToken extends Token {
    char value;
    CharacterToken(char value) { this.value = value; }
    public char getCharacter() { return this.value; }
    public String toString() { return "Character '" + Character.toString(value) + "'"; }
}
class StringToken extends Token {
    String value;
    StringToken(String value) { this.value = value; }
    public String getString() { return this.value; }
    public String toString() { return "String \"" + value + "\""; }
}
class MessageToken extends Token {
    Message value;
    MessageToken(Message value) { this.value = value; }
    public Message getMessage() { return this.value; }
    public String toString() { return "Message #" + value; }
}
class KeywordToken extends Token {
    String value;
    KeywordToken(String value) { this.value = value; }
    public String getKeyword() { return this.value; }
    public String toString() { return "Keyword " + value + ":"; }
}
class IdentifierToken extends Token {
    String value;
    IdentifierToken(String value) { this.value = value; }
    public String getIdentifier() { return this.value; }
    public String toString() { return "Identifier " + value; }
}
class BinaryOperatorToken extends Token {
    String value;
    BinaryOperatorToken(String value) { this.value = value; }
    public String getBinaryOperator() { return this.value; }
    public String toString() { return "BinaryOperator " + value; }
}
class SequenceSeparatorToken extends Token {
    public String toString() { return "."; }
}
class PipeToken extends Token {
    public String toString() { return ";"; }
}
class MethodMarkToken extends Token {
    public String toString() { return "@"; }
}
class VectorStartToken extends Token {
    public String toString() { return "["; }
}
class VectorEndToken extends Token {
    public String toString() { return "]"; }
}
class VectorSeparatorToken extends Token {
    public String toString() { return ","; }
}
class LambdaStartToken extends Token {
    public String toString() { return "{"; }
}
class LambdaSeparatorToken extends Token {
    public String toString() { return "|"; }
}
class LambdaEndToken extends Token {
    public String toString() { return "}"; }
}
class ParenthesisOpenToken extends Token {
    public String toString() { return "("; }
}
class ParenthesisCloseToken extends Token {
    public String toString() { return ")"; }
}
class EOFToken extends Token {
    public String toString() { return "<eof>"; }
}
