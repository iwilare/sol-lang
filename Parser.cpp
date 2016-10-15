class Parser;
class ParserException : public SolException {
public:
  ParserException(Parser *parser, string message);
  ParserException(Atom *atom, string message);
  ParserException(Token token, string message);
};
class Parser {
public:
  Tokenizer *tokenizer;
  Parser(Tokenizer *tokenizer) : tokenizer(tokenizer) {}
  Atom *parse() {
    Atom *atom = parseAtom();
    if(tokenizer->hasTokens())
      throw ParserException(this, "Expected EOF.");
    tokenizer->close();				   
    return atom;
  }
  Atom *parseAtom() {
    return parseSequence();
  }
  Atom *parseUnaryMessage() {
    return parseUnaryMessage(parseObject());
  }
  Atom *parseUnaryMessage(Atom *atom) {
    while(tokenizer->getToken().type == Token::Type::Identifier) {
      atom = new Atom(atom, Message({tokenizer->getToken().identifier}, 0), {});
      tokenizer->nextToken();
    }
    return atom;
  }
  Atom *parseBinaryMessage() {
    return parseBinaryMessage(parseUnaryMessage());
  }
  Atom *parseBinaryMessage(Atom *atom) {
    if(tokenizer->getToken().type == Token::Type::BinaryOperator and
       tokenizer->getToken().binaryOperator == "=")
      if(atom->type != Atom::Type::Identifier)
	throw ParserException(this, "Expected identifier in assignment.");
      else if(atom->identifier == "super")
	throw ParserException(this, "Super cannot be used in assignment.");
      else if(atom->identifier == "self")
	throw ParserException(this, "Self cannot be used in assignment.");
      else if(atom->identifier == "nothing")
	throw ParserException(this, "Nothing cannot be used in assignment.");
      else {
	tokenizer->nextToken();
	return new Atom(atom, parseKeywordMessage()); 
      }     
    while(tokenizer->getToken().type == Token::Type::BinaryOperator) {
      string op = tokenizer->getToken().binaryOperator;
      tokenizer->nextToken();
      atom = new Atom(atom, Message({op},1), {parseUnaryMessage()});
    }
    return atom;
  }
  Atom *parseKeywordMessage() {
    return parseKeywordMessage(parseBinaryMessage());
  }
  Atom *parseKeywordMessage(Atom *atom) {
    vector<string> messageParts;
    vector<Atom*> arguments;
    while(tokenizer->getToken().type == Token::Type::Keyword) {
      messageParts.push_back(tokenizer->getToken().keyword);
      tokenizer->nextToken();
      arguments.push_back(parseBinaryMessage());
    }
    if(arguments.size() > 0)
      atom = new Atom(atom, Message(messageParts, messageParts.size()), arguments);
    while(tokenizer->getToken().type == Token::Type::Pipe) {
      tokenizer->nextToken();
      atom = parseKeywordMessage(parseBinaryMessage(parseUnaryMessage(atom)));
    }
    return atom;
  }
  Atom *parseSequence() {
    vector<Atom*> sequence;
    do {
      if(tokenizer->getToken().type == Token::Type::SequenceSeparator)
	tokenizer->nextToken();
      if(!tokenizer->getToken().isObjectStart())
	break;
      sequence.push_back(parseKeywordMessage());
    } while(tokenizer->getToken().type == Token::Type::SequenceSeparator);
    if(sequence.size() == 0)
      throw ParserException(this, "Expected expression.");
    else if(sequence.size() == 1)
      return sequence[0];
    else
      return new Atom(sequence);
  }
  Atom *parseObject() {
    return parseObject(tokenizer->getToken());
  }
  Atom *parseObject(Token token) {
    if(!token.isObjectStart())
      throw ParserException(token, "Unexpected token \"" + token.toString() + "\", expected a valid object.");
    Atom *atom = new Atom(token.location);
    tokenizer->nextToken();
    switch(token.type) {
    case Token::Type::BinaryOperator: {
      Atom *toNegate = parseObject();
      if(toNegate->type == Atom::Type::Integer)
	toNegate->integerValue *= -1;
      else if(toNegate->type == Atom::Type::Double)
	toNegate->doubleValue *= -1;
      else
	return new Atom(toNegate, Message("negate"), {});
      return toNegate;
    }
    case Token::Type::Identifier:
      atom->type = Atom::Type::Identifier;
      atom->identifier = token.identifier;
      return atom;
    case Token::Type::Integer:
      atom->type = Atom::Type::Integer;
      atom->integerValue = token.integerValue;
      return atom;
    case Token::Type::Double:
      atom->type = Atom::Type::Double;
      atom->doubleValue = token.doubleValue;
      return atom;
    case Token::Type::String:
      atom->type = Atom::Type::String;
      atom->stringValue = token.stringValue;
      return atom;
    case Token::Type::Character:
      atom->type = Atom::Type::Character;
      atom->characterValue = token.characterValue;
      return atom;
    case Token::Type::Symbol:
      atom->type = Atom::Type::Symbol;
      atom->symbol = token.symbol;
      return atom;
    case Token::Type::LambdaStart: {
      TokenizerState *state = tokenizer->saveState();
      vector<Token> parameters;
      while(tokenizer->getToken().type == Token::Type::Identifier) {
	parameters.push_back(tokenizer->getToken());
	tokenizer->nextToken();
      }
      if(parameters.size() == 0) 
	;
      else if(tokenizer->getToken().type == Token::Type::LambdaSeparator) {
	tokenizer->nextToken();
	for(Token t : parameters)
	  atom->lambda.parameters.push_back(t.identifier);
      } else 
	tokenizer->restoreState(state);
      atom->type = Atom::Type::Lambda;
      if(tokenizer->getToken().type == Token::Type::LambdaEnd)
	atom->lambda.body = nullptr;
      else
	atom->lambda.body = parseAtom();
      if(tokenizer->getToken().type != Token::Type::LambdaEnd)
	throw ParserException(this, "Expected lambda end.");
      tokenizer->nextToken();
      return atom;
    }
    case Token::Type::MethodMark: {
      if(!tokenizer->getToken().isObjectStart())
	throw ParserException(this, "Expected valid object as method destination.");

      Location lambdaLocation, messageLocation;
      messageLocation = tokenizer->getToken().location;
      
      Atom *methodDefinition = parseKeywordMessage();
      if(methodDefinition->type != Atom::Type::MessageT)
	throw ParserException(methodDefinition, "Expected valid method definition.");

      Atom *receiver = methodDefinition->message.receiver;
      Message message = methodDefinition->message.message;
      vector<string> parameters;
      for(Atom *argument : methodDefinition->message.arguments)
	if(argument->type == Atom::Type::Identifier)
	  parameters.push_back(argument->identifier);
	else
	  throw ParserException(argument, "Expected valid parameter identifier.");
      
      if(tokenizer->getToken().type != Token::Type::MethodMark)
	throw ParserException(this, "Expected token mark.");
      tokenizer->nextToken();
      
      Atom *messageSymbol = new Atom(Atom::Type::Symbol);
      messageSymbol->location = messageLocation;
      messageSymbol->symbol = message.toString();
      
      if(tokenizer->getToken().type == Token::Type::LambdaStart) {
	lambdaLocation = tokenizer->getToken().location;
	tokenizer->nextToken();
	Atom *body;
	if(tokenizer->getToken().type == Token::Type::LambdaEnd)
	  body = nullptr;
	else
	  body = parseAtom();
	if(tokenizer->getToken().type != Token::Type::LambdaEnd)
	  throw ParserException(this, "Expected method end.");
	tokenizer->nextToken();

	Atom *lambda = new Atom(Atom::Type::Lambda);
	lambda->location = lambdaLocation;
	lambda->lambda.body = body;
	lambda->lambda.parameters = parameters; 
	return new Atom(receiver, Message({"method","set"},2),
			{messageSymbol, lambda});
      } else if(Token::Type::BinaryOperator and
		tokenizer->getToken().binaryOperator == "=") {
	tokenizer->nextToken();
	return new Atom(receiver, Message({"method","set"},2),
			{messageSymbol, parseKeywordMessage()});
      } else
	return new Atom(receiver, Message({"method"},1), {messageSymbol});
    }
    case Token::Type::VectorStart:
      atom->type = Atom::Type::Vector;
      atom->vectorElements = {};
      while(tokenizer->getToken().type != Token::Type::VectorEnd) {
      	Atom *element = parseAtom();
	atom->vectorElements.push_back(element);
	if(tokenizer->getToken().type == Token::Type::VectorSeparator)
	  tokenizer->nextToken();	  
	else if(tokenizer->getToken().type != Token::Type::VectorEnd)
	  throw ParserException(this, "Expected vector separator or vector end.");
      }
      tokenizer->nextToken();
      return atom;
    case Token::Type::ParenthesisOpen: {
      Atom *content = parseAtom();
      if(tokenizer->getToken().type != Token::Type::ParenthesisClose)
	throw ParserException(this, "Expected closed parenthesis.");
      tokenizer->nextToken();
      return content;
    }
    default:
      throw ParserException(this, "Unexpected token.");
    }
  }
};


ParserException::ParserException(Parser *parser, string message) :
  SolException("Parser", parser->tokenizer->getToken().location, message) {}
ParserException::ParserException(Atom *atom, string message) :
  SolException("Parser", atom->location, message) {}
ParserException::ParserException(Token token, string message) :
  SolException("Parser", token.location, message) {}
