import java.io.*;
import java.util.*;

class ParserException extends SolException {
    ParserException(Parser parser, String message) throws TokenizerException {
	super("Parser", parser.getTokenizer().getToken().getLocation(), message);
    }
    ParserException(Parser parser, Atom atom, String message) {
	super("Parser", atom.getLocation(), message);
    }
    ParserException(Parser parser, Token token, String message) {
	super("Parser", token.getLocation(), message);
    }
    ParserException(Parser parser, Location location, String message) {
	super("Parser", location, message);
    }
}
public class Parser {
    Tokenizer tokenizer;

    class State {
	public Tokenizer.State state;
	State(Tokenizer.State state) {
	    this.state = state;
	}
    }
    
    Parser(Tokenizer tokenizer) {
	this.tokenizer = tokenizer;
    }
    public State saveState() {
	return new State(tokenizer.saveState());
    }
    public void restoreState(State state) {
	tokenizer.restoreState(state.state);
    }
    public Tokenizer getTokenizer() {
	return tokenizer;
    }
    public Atom parse() throws ParserException, TokenizerException {
	if(!tokenizer.hasTokens()) {
	    tokenizer.close();
	    return null;
	}
	Atom atom = parseAtom();
	if(tokenizer.hasTokens())
	    throw new ParserException(this, "Expected EOF.");
	tokenizer.close();
	return atom;
    }
    public Atom parseAtom() throws ParserException, TokenizerException { return parseSequence(); }
    public Atom parseUnaryMessage() throws ParserException, TokenizerException { return parseUnaryMessage(parseObject()); }
    public Atom parseUnaryMessage(Atom atom) throws ParserException, TokenizerException {
	while(tokenizer.getToken() instanceof IdentifierToken) {
	    String identifier = ((IdentifierToken)tokenizer.getToken()).getIdentifier();
	    atom = new UnaryMessageAtom(atom, new UnaryMessage(identifier));
	    tokenizer.nextToken();
	}
	return atom;
    }
    public Atom parseBinaryMessage() throws ParserException, TokenizerException { return parseBinaryMessage(parseUnaryMessage()); }
    public Atom parseBinaryMessage(Atom atom) throws ParserException, TokenizerException {
	Token token = tokenizer.getToken();
	if(token instanceof BinaryOperatorToken &&
	   ((BinaryOperatorToken)token).getBinaryOperator().equals("="))
	    if(!(atom instanceof IdentifierAtom))
		throw new ParserException(this, atom, "Expected identifier in assignment.");
	    else if(((IdentifierAtom)atom).getIdentifier().equals("super"))
		throw new ParserException(this, atom, "Super cannot be used in assignment.");
	    else if(((IdentifierAtom)atom).getIdentifier().equals("self"))
		throw new ParserException(this, atom, "Self cannot be used in assignment.");
	    else if(((IdentifierAtom)atom).getIdentifier().equals("nothing"))
		throw new ParserException(this, atom, "Nothing cannot be used in assignment.");
	    else {
		tokenizer.nextToken();
		return new AssignmentAtom(((IdentifierAtom)atom).getIdentifier(), parseKeywordMessage()); 
	    }     
	while(tokenizer.getToken() instanceof BinaryOperatorToken) {
	    String op = ((BinaryOperatorToken)tokenizer.getToken()).getBinaryOperator();
	    tokenizer.nextToken();
	    atom = new BinaryMessageAtom(atom, new BinaryMessage(op), parseUnaryMessage());
	}
	return atom;
    }
    public Atom parseKeywordMessage() throws ParserException, TokenizerException { return parseKeywordMessage(parseBinaryMessage()); }
    public Atom parseKeywordMessage(Atom atom) throws ParserException, TokenizerException {
	ArrayList<String> messageParts = new ArrayList<>();
	ArrayList<Atom> arguments = new ArrayList<>();
	while(tokenizer.getToken() instanceof KeywordToken) {
	    messageParts.add(((KeywordToken)tokenizer.getToken()).getKeyword());
	    tokenizer.nextToken();
	    arguments.add(parseBinaryMessage());
	}
	String[] messagePartsArray = messageParts.toArray(new String[messageParts.size()]);
	Atom[] argumentsArray = arguments.toArray(new Atom[arguments.size()]);
	
	if(arguments.size() > 0)
	    atom = new KeywordMessageAtom(atom, new KeywordMessage(messagePartsArray), argumentsArray);
	while(tokenizer.getToken() instanceof PipeToken) {
	    tokenizer.nextToken();
	    atom = parseKeywordMessage(parseBinaryMessage(parseUnaryMessage(atom)));
	}
	return atom;
    }
    public Atom parseSequence() throws ParserException, TokenizerException {
	ArrayList<Atom> sequence = new ArrayList<>();
	do {
	    if(tokenizer.getToken() instanceof SequenceSeparatorToken)
		tokenizer.nextToken();
	    if(!tokenizer.getToken().isObjectStart())
		break;
	    sequence.add(parseKeywordMessage());
	} while(tokenizer.getToken() instanceof SequenceSeparatorToken);
	if(sequence.size() == 0)
	    throw new ParserException(this, "Expected expression.");
	else if(sequence.size() == 1)
	    return sequence.get(0);
	else
	    return new SequenceAtom(sequence.toArray(new Atom[sequence.size()]));
    }
    public Atom parseObject() throws ParserException, TokenizerException { return parseObject(tokenizer.getToken()); }
    public Atom parseObject(Token token) throws ParserException, TokenizerException {
	if(!token.isObjectStart())
	    throw new ParserException(this, token, "Unexpected token \"" + token.toString() + "\", expected a valid object.");
	tokenizer.nextToken();
	Atom atom = null;
	if(token instanceof BinaryOperatorToken) {
	    atom = parseObject();
	    if(atom instanceof IntegerAtom)
		((IntegerAtom)atom).negateValue();
	    else if(atom instanceof DoubleAtom)
		((DoubleAtom)atom).negateValue();
	    else
		atom = new UnaryMessageAtom(atom, new UnaryMessage("negate"));
	}
	else if(token instanceof IdentifierToken) atom = new IdentifierAtom(((IdentifierToken)token).getIdentifier());
	else if(token instanceof IntegerToken)    atom = new IntegerAtom(((IntegerToken)token).getInteger());
	else if(token instanceof DoubleToken)     atom = new DoubleAtom(((DoubleToken)token).getDouble());
	else if(token instanceof CharacterToken)  atom = new CharacterAtom(((CharacterToken)token).getCharacter());
	else if(token instanceof StringToken)     atom = new StringAtom(((StringToken)token).getString());
	else if(token instanceof MessageToken)    atom = new MessageSymbolAtom(((MessageToken)token).getMessage());
	else if(token instanceof LambdaStartToken) {
	    Tokenizer.State state = tokenizer.saveState();
	    ArrayList<String> parameters = new ArrayList<>();
	    while(tokenizer.getToken() instanceof IdentifierToken) {
		parameters.add(((IdentifierToken)tokenizer.getToken()).getIdentifier());
		tokenizer.nextToken();
	    }
	    String[] parametersArray;
	    if(tokenizer.getToken() instanceof LambdaSeparatorToken) {
		tokenizer.nextToken();
		parametersArray = parameters.toArray(new String[parameters.size()]);
	    } else {
		parametersArray = new String[]{};
		if(parameters.size() == 0)
		    ;
		else
		    tokenizer.restoreState(state);
	    }
	    Atom body = null;
	    if(!(tokenizer.getToken() instanceof LambdaEndToken))
		body = parseAtom();
	    else
		body = null;
	    if(!(tokenizer.getToken() instanceof LambdaEndToken))
		throw new ParserException(this, "Expected lambda end.");
	    tokenizer.nextToken();
	    atom = new LambdaAtom(body, parametersArray);
	}
	else if(token instanceof MethodMarkToken) {
	    if(!tokenizer.getToken().isObjectStart())
		throw new ParserException(this, "Expected valid object as method destination.");

	    Location messageLocation = tokenizer.getToken().getLocation();

	    Atom methodDefinition = parseKeywordMessage();
	    
	    if(!(methodDefinition instanceof MessageAtom))
		throw new ParserException(this, methodDefinition, "Expected valid method definition.");

	    Atom receiver    = ((MessageAtom)methodDefinition).getReceiver();
	    Message message  = ((MessageAtom)methodDefinition).getMessage();
	    Atom[] arguments = ((MessageAtom)methodDefinition).getArguments();
	    ArrayList<String> parameters = new ArrayList<>();
	    for(Atom argument : arguments)
		if(argument instanceof IdentifierAtom)
		    parameters.add(((IdentifierAtom)argument).getIdentifier());
		else
		    throw new ParserException(this, argument, "Expected valid parameter identifier.");
	    
	    String[] parametersArray = parameters.toArray(new String[parameters.size()]);

	    if(!(tokenizer.getToken() instanceof MethodMarkToken))
		throw new ParserException(this, "Expected method mark token.");
	    tokenizer.nextToken();
	    
	    Atom messageSymbol = new MessageSymbolAtom(message);
	    messageSymbol.setLocation(messageLocation);
	    if(tokenizer.getToken() instanceof LambdaStartToken) {
		tokenizer.nextToken();
		Atom body = null;
		if(!(tokenizer.getToken() instanceof LambdaEndToken))
		    body = parseAtom();
		if(!(tokenizer.getToken() instanceof LambdaEndToken))
		    throw new ParserException(this, "Expected method end.");
		tokenizer.nextToken();

		Atom lambda = new LambdaAtom(body, parametersArray);
		atom = new KeywordMessageAtom(receiver, new KeywordMessage(new String[]{"method","set"}),
					      new Atom[]{messageSymbol, lambda});
	    } else if(tokenizer.getToken() instanceof BinaryOperatorToken &&
		      ((BinaryOperatorToken)tokenizer.getToken()).getBinaryOperator().equals("=")) {
		tokenizer.nextToken();
		atom = new KeywordMessageAtom(receiver, new KeywordMessage(new String[]{"method","set"}),
					      new Atom[]{messageSymbol, parseKeywordMessage()});
	    } else
		atom = new KeywordMessageAtom(receiver, new KeywordMessage(new String[]{"method"}),
					      new Atom[]{messageSymbol});
	} else if(token instanceof VectorStartToken) {
	    ArrayList<Atom> elements = new ArrayList<>();
	    while(!(tokenizer.getToken() instanceof VectorEndToken)) {
		elements.add(parseAtom());
		if(tokenizer.getToken() instanceof VectorSeparatorToken)
		    tokenizer.nextToken();
		else if(!(tokenizer.getToken() instanceof VectorEndToken))
		    throw new ParserException(this, "Expected vector separator or vector end.");
	    }
	    tokenizer.nextToken();
	    Atom[] elementsArray = elements.toArray(new Atom[elements.size()]);
	    atom = new VectorAtom(elementsArray);
	} else if(token instanceof ParenthesisOpenToken) {
	    atom = parseAtom();
	    if(!(tokenizer.getToken() instanceof ParenthesisCloseToken))
		throw new ParserException(this, "Expected closed parenthesis.");
	    tokenizer.nextToken();
	} else
	    throw new ParserException(this, "Unexpected token.");
	atom.setLocation(token.getLocation());
	return atom;
    }
}
