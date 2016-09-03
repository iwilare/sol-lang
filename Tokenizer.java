import java.io.*;
import java.util.*;

class TokenizerException extends SolException {
    TokenizerException(Tokenizer tokenizer, String message) throws TokenizerException {
	super("Tokenizer", tokenizer.getToken().getLocation(), message);
    }
    TokenizerException(Tokenizer tokenizer, Token token, String message) {
	super("Tokenizer", token.getLocation(), message);
    }
    TokenizerException(Tokenizer tokenizer, Location location, String message) {
	super("Tokenizer", location, message);
    }
}
public class Tokenizer {
    CharacterStream stream;
    Token token;

    class State {
	public CharacterStream.State state;
	public Token token;
	State(CharacterStream.State state, Token token) {
	    this.state = state;
	    this.token = token;
	}
    }
    Tokenizer(CharacterStream stream) throws TokenizerException {
	this.stream = stream;
    }
    public State saveState() {
	return new State(stream.saveState(), token);
    }
    public void restoreState(State state) {
	stream.restoreState(state.state);
	token = state.token;
    }
    public static boolean isIdentifier(char c) {
	return Character.isLetterOrDigit(c);
    }
    public static boolean isMessageBlockingCharacter(char c) {
	return "[]{}(),;".indexOf(c) != -1;
    }
    public static boolean isBinaryOperator(char c) {
	return c != 0 && !Character.isWhitespace(c) && !isIdentifier(c) &&
	    !isMessageBlockingCharacter(c) && c != '\'' && c != '"';
    }
    public void close() { stream.close(); }
    public boolean hasTokens() throws TokenizerException { return !(getToken() instanceof EOFToken); }
    public Token getToken() throws TokenizerException {
	if(token == null)
	    nextToken();
	return token;
    }
    void nextToken() throws TokenizerException {
	skipCharacters();
	Location loc = new Location(stream.getLocation());
	if(stream.hasCharacters())
	    nextTokenNonlocalized();
	else
	    token = new EOFToken();	     
	token.setLocation(loc);
    }
    void nextTokenNonlocalized() throws TokenizerException {
	Location loc = new Location(stream.getLocation());
		
	if((token = getNumber())            != null) return;
	if((token = getString())            != null) return;
	if((token = getCharacter())         != null) return;
	if((token = getMessage())            != null) return;
	if((token = getIdentifierOrKeyword()) != null) return;
	
	if(getSingleCharacterToken('[')) { token = new VectorStartToken();      return; }
	if(getSingleCharacterToken(']')) { token = new VectorEndToken();        return; }
	if(getSingleCharacterToken('{')) { token = new LambdaStartToken();      return; }
	if(getSingleCharacterToken('}')) { token = new LambdaEndToken();        return; }
	if(getSingleCharacterToken('(')) { token = new ParenthesisOpenToken();  return; }
	if(getSingleCharacterToken(')')) { token = new ParenthesisCloseToken(); return; }
	if(getSingleCharacterToken(',')) { token = new VectorSeparatorToken();  return; }
	if(getSingleCharacterToken(';')) { token = new PipeToken();             return; }
	
	if((token = getBinaryOperator()) != null) {
	    BinaryOperatorToken binaryOperator = (BinaryOperatorToken)token;
	    if(binaryOperator.getBinaryOperator().equals("."))
		token = new SequenceSeparatorToken();
	    if(binaryOperator.getBinaryOperator().equals("@"))
		token = new MethodMarkToken();
	    if(binaryOperator.getBinaryOperator().equals("|"))
		token = new LambdaSeparatorToken();
	    return;
	}
	throw new TokenizerException(this, loc, "Unrecognized character.");	
    }
    public boolean getSingleCharacterToken(char character) {
	if(stream.getCharacter() != character)
	    return false;
	stream.nextCharacter();
	return true;
    }
    public void skipCharacters() {
	while(skipWhitespace()
	      || skipLineComment()
	      || skipComment()
	      || skipReverseComment());
    }
    public boolean skipWhitespace() {
	if(Character.isWhitespace(stream.getCharacter())) {
	    stream.nextCharacter();
	    return true;
	} else
	    return false;
    }
    public boolean skipConstantString(String s) {
	CharacterStream.State state = stream.saveState();
	for(char c : s.toCharArray())
	    if(stream.getCharacter() == c)
		stream.nextCharacter();
	    else {
		stream.restoreState(state);
		return false;
	    }
	return true;
    }
    public boolean skipReverseComment() {
	return skipConstantString("-}");
    }
    public boolean skipComment() {
	if(skipConstantString("{-")) {
	    while(stream.hasCharacters() && !skipConstantString("-}"))
		stream.nextCharacter();
	    return true;
	} else return false;
    }
    public boolean skipLineComment() {
	if(skipConstantString("--")) {
	    while(stream.hasCharacters() && stream.getCharacter() != '\n')
		stream.nextCharacter();
	    stream.nextCharacter();
	    return true;
	} else return false;
    }
    ///////////////////////////////////////////////////////////////
    public Token getNumber() {
	if(!Character.isDigit(stream.getCharacter()))
	    return null;
	int integerValue = 0;
	while(Character.isDigit(stream.getCharacter())) {
	    integerValue *= 10;
	    integerValue += Character.getNumericValue(stream.getCharacter());
	    stream.nextCharacter();
	}
	CharacterStream.State state = stream.saveState();
	if(stream.getCharacter() == '.') {
	    stream.nextCharacter();
	    if(!Character.isDigit(stream.getCharacter())) {
		stream.restoreState(state);
		return new IntegerToken(integerValue);
	    }
	    double doubleValue = 0.0;
	    int exp = -1;
	    while(Character.isDigit(stream.getCharacter())) {
		doubleValue +=
		    Character.getNumericValue(stream.getCharacter()) * Math.pow(10,exp);
		exp--;
		stream.nextCharacter();
	    }
	    return new DoubleToken(integerValue + doubleValue);
	} else
	    return new IntegerToken(integerValue);
    }
    public Token getString() throws TokenizerException {
	if(stream.getCharacter() != '"')
	    return null;
	String stringValue = "";
	while(stream.getCharacter() == '"') {
	    stream.nextCharacter();
	    while(stream.getCharacter() != '"') {
		if(!stream.hasCharacters())
		    throw new TokenizerException(this, "Unexpected EOF in string.");
		if(stream.getCharacter() == '\n') {
		    stream.nextCharacter();
		    skipCharacters();
		} else if(stream.getCharacter() == '\\') {
		    stringValue += parseEscapeSequence();
		    stream.nextCharacter();
		} else {	
		    stringValue += stream.getCharacter();
		    stream.nextCharacter();
		}
	    }
	    stream.nextCharacter();
	    skipCharacters();
	}
	return new StringToken(stringValue);
    }
    public char parseEscapeSequence() throws TokenizerException {
	stream.nextCharacter();
	char a = stream.getCharacter();
	if(!stream.hasCharacters()) throw new TokenizerException(this, "Expected escape character value but got EOF.");
	if(a == '\n')      throw new TokenizerException(this, "Expected escape character value but got newline.");
	if(Character.digit(a, 16) != -1) {
	    stream.nextCharacter();

	    char b = stream.getCharacter(); 
	    if(!stream.hasCharacters()) throw new TokenizerException(this, "Expected hexadecimal character value but got EOF.");
	    if(b == '\n')      throw new TokenizerException(this, "Expected hexadecimal character value but got newline.");
	    if(Character.digit(a, 16) != -1)
		return (char)(Character.digit(a, 16) * 16 + Character.digit(b, 16));
	    else
		throw new TokenizerException(this, "Expected valid hexadecimal character value.");
	} else
	    return escapeCharacter(stream.getCharacter());
    }
    public static char escapeCharacter(char c) {
	switch(c) {
	case 'n':  return '\n';
	case '\\': return '\\';
	case '\'': return '\'';
	case '\"': return '\"';    
	case 't':  return '\t';
	case 'r':  return '\r';
	default:   return c;
	}
    }
    public Token getCharacter() throws TokenizerException {
	if(stream.getCharacter() != '\'')
	    return null;
	stream.nextCharacter();
	char character;
	if(!stream.hasCharacters())
	    throw new TokenizerException(this, "Expected character but got EOF.");
	else if(stream.getCharacter() == '\n')
	    throw new TokenizerException(this, "Expected character but got newline.");
	else if(stream.getCharacter() == '\\')
	    character = parseEscapeSequence();
	else
	    character = stream.getCharacter();	 
	stream.nextCharacter();
	if(stream.getCharacter() != '\'')
	    throw new TokenizerException(this, "Expected closing character quote.");
	stream.nextCharacter();
	return new CharacterToken(character);
    }
    public Token getMessage() throws TokenizerException {
	if(stream.getCharacter() != '#')
	    return null;
	CharacterStream.State state = stream.saveState();
	stream.nextCharacter();
	if(isIdentifier(stream.getCharacter())) {
	    ArrayList<String> parts = new ArrayList<>();
	    String part = "";
	    while(isIdentifier(stream.getCharacter()) || stream.getCharacter() == ':') {
		if(stream.getCharacter() == ':') {
		    if(part.equals(""))
			throw new TokenizerException(this, "Invalid empty keyword while parsing Message.");
		    parts.add(part);
		    part = "";
		} else
		    part += stream.getCharacter();
		stream.nextCharacter();
	    }
	    if(!part.equals(""))
		if(parts.size() == 0)
		    return new MessageToken(new UnaryMessage(part));
		else
		    throw new TokenizerException(this, "Expected keyword end.");
	    else
		return new MessageToken(new KeywordMessage(parts.toArray(new String[parts.size()])));
	} else if(isBinaryOperator(stream.getCharacter())) {
	    String message = "";
	    while(isBinaryOperator(stream.getCharacter())) {
		message += stream.getCharacter();
		stream.nextCharacter();
	    }
	    return new MessageToken(new BinaryMessage(message));
	} else {
	    stream.restoreState(state);
	    return null;
	}
    }
    public Token getIdentifierOrKeyword() {
	if(!isIdentifier(stream.getCharacter()))
	    return null;
	String symbol = "";
	while(isIdentifier(stream.getCharacter())) {
	    symbol += stream.getCharacter();
	    stream.nextCharacter();
	}
	if(stream.getCharacter() == ':') {
	    stream.nextCharacter();
	    return new KeywordToken(symbol);
	} else
	    return new IdentifierToken(symbol);
    }
    public Token getBinaryOperator() {
	if(!isBinaryOperator(stream.getCharacter()))
	    return null;
	String binaryOperator = "";
	while(isBinaryOperator(stream.getCharacter())) {
	    binaryOperator += stream.getCharacter();
	    stream.nextCharacter();
	}
	return new BinaryOperatorToken(binaryOperator);
    }
}
