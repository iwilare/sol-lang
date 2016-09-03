bool charCheck(char c, string s) {
  for(char k : s)
    if(c == k)
      return true;
  return false;
}
int charToInt(char c) { return c - '0'; }
bool isWhitespace(char c) { return c==' ' or c=='\n' or c=='\t'; }
bool isAlpha(char c) { return 'a'<=c and c<='z' or 'A'<=c and c<='Z'; }
bool isNumeric(char c) { return '0'<=c and c<='9'; }
bool isHexadecimal(char c) { return isNumeric(c) or 'a'<=c and c<='f' or 'A'<=c and c<='F'; }
bool isAlphanumeric(char c) { return isAlpha(c) or isNumeric(c); }
bool isIdentifier(char c) { return isAlphanumeric(c); }
bool isParenthesis(char c) { return charCheck(c, "()[]{}"); }

pair<char, Token::Type> blockingCharacters[] = {
  {'[', Token::Type::vectorStartT},
  {']', Token::Type::vectorEndT},
  {'{', Token::Type::lambdaStartT},
  {'}', Token::Type::lambdaEndT},
  {'(', Token::Type::parenthesisOpenT},
  {')', Token::Type::parenthesisCloseT},
  {',', Token::Type::vectorSeparatorT},
  {';', Token::Type::pipeT},
};
bool isBlockingCharacter(char c) {
  for(auto p : blockingCharacters)
    if(c == p.first)
      return true;
  return false;
}

pair<string, Token::Type> nonblockingCharacters[] = {
  {"^", Token::Type::returnTokenT},
  {".", Token::Type::sequenceSeparatorT},
  {"@", Token::Type::methodMarkT},
  {"|", Token::Type::lambdaSeparatorT},
};
bool isBinaryOperator(char c) {
  return c != EOF and !isWhitespace(c) and !isIdentifier(c) and
    !isBlockingCharacter(c) and !charCheck(c, "\'\"");
}
bool isSymbol(char c) {
  return isBinaryOperator(c) or isAlphanumeric(c) or c==':';
}
int charToHex(char a) {
  if('0'<=a and a<='9') return a-'0';
  else if('a'<=a and a<='f') return a-'a'+10;
  else if('A'<=a and a<='F') return a-'A'+10;
}
char bytesToByte(char a, char b) {
  return charToHex(a) * 16 + charToHex(b);
}
char escapeCharacter(char c) {
  switch(c) {
  case 'n': return '\n';
  case '\\': return '\\';
  case '\'': return '\'';
  case '\"': return '\"';    
  case 't': return '\t';
  case 'r': return '\r';
  default: return c;
  }
}


class Tokenizer;
class TokenizerException : public SolException {
public:
  TokenizerException(Tokenizer *tokenizer, string message);
  TokenizerException(Token token, string message);
};
class TokenizerState {
public:
  StreamState *streamState;
  Token token;
  TokenizerState(StreamState *streamState, Token token) :
    streamState(streamState), token(token) {}
};
class Tokenizer {
public:
  CharacterStream *stream;
  Token token;
  Tokenizer(CharacterStream *stream) : stream(stream) { nextToken(); }
  void close() { stream->close(); }
  bool hasTokens() { return token.type != Token::Type::eofT; }
  TokenizerState *saveState() { return new TokenizerState(stream->saveState(), token); }
  void restoreState(TokenizerState *state) {
    stream->restoreState(state->streamState);
    token = state->token;
  }
  Token getToken() { return token; }
  void nextToken() {
    
    while(skipCharacters());
    
    token.location = stream->currentLocation();
    if(stream->isEOF()) {
      token.type = Token::Type::eofT;
      return;
    }
    
    if(getNumber()) return;
    if(getString()) return;
    if(getCharacter()) return;
    if(getSymbol()) return;
    if(getIdentifierKeyword()) return;
    if(getComment()) return;
    if(getLineComment()) return;
    
    
    for(auto p : blockingCharacters) 
      if(getSingleCharacterToken(p.first, p.second))
	return;

    if(getBinaryOperator()) {
      for(auto p : nonblockingCharacters)
	if(token.binaryOperator == p.first) {
	  token.type = p.second;
	  return;
	}
      return;
    }
    throw TokenizerException(this, "Unrecognized character.");
  }
  bool skipCharacters() {
    if(getWhitespace())     return true;
    if(getLineComment())    return true;
    if(getComment())        return true;
    if(getReverseComment()) return true;
    return false;
  }
  bool getWhitespace() {
    if(isWhitespace(stream->getCharacter())) {
      stream->nextCharacter();
      return true;
    } else
      return false;
  }
  bool checkConstantString(string s) {
    StreamState *state = stream->saveState();
    for(char c : s)
      if(stream->getCharacter() == c)
	stream->nextCharacter();
      else {
	stream->restoreState(state);
	return false;
      }
    return true;
  }
  bool getReverseComment() {
    return checkConstantString("-}");
  }
  bool getComment() {
    if(checkConstantString("{-")) {
      while(!stream->isEOF() and !checkConstantString("-}"))
	stream->nextCharacter();
      return true;
    } else return false;
  }
  bool getLineComment() {
    if(checkConstantString("--")) {
      while(!stream->isEOF() and stream->getCharacter() != '\n')
	stream->nextCharacter();
      stream->nextCharacter();
      return true;
    } else return false;
  }
  bool getNumber() {
    if(!isNumeric(stream->getCharacter()))
      return false;
    int integerPart = 0;
    while(isNumeric(stream->getCharacter())) {
      integerPart *= 10;
      integerPart += charToInt(stream->getCharacter());
      stream->nextCharacter();
    }
    StreamState *state = stream->saveState();
    if(stream->getCharacter() == '.') {
      stream->nextCharacter();
      if(!isNumeric(stream->getCharacter())) {
	stream->restoreState(state);
	token.type = Token::Type::integerT;
	token.integerValue = integerPart;
	return true;
      }
      token.type = Token::Type::doubleT;
      token.doubleValue = integerPart;
      double doublePart = 0.0;
      int exp = -1;
      while(isNumeric(stream->getCharacter())) {
	token.doubleValue += charToInt(stream->getCharacter()) * pow(10,exp);
	exp--;
	stream->nextCharacter();
      }
    } else {
      token.type = Token::Type::integerT;
      token.integerValue = integerPart;
    }
    return true;
  }
  bool getString() {
    if(stream->getCharacter() != '"')
      return false;
    token.type = Token::Type::stringT;
    token.stringValue = "";
    while(stream->getCharacter() == '"') {
      stream->nextCharacter();
      while(stream->getCharacter() != '"') {
	if(stream->getCharacter() == EOF)
	  throw TokenizerException(this, "Unexpected EOF in string.");
	else if(stream->getCharacter() == '\n') {
	  stream->nextCharacter();
	  skipCharacters();
	} else if(stream->getCharacter() == '\\') {
	  token.stringValue += parseEscapeSequence();
	  stream->nextCharacter();
	} else {	
	  token.stringValue += stream->getCharacter();
	  stream->nextCharacter();
	}
      }
      stream->nextCharacter();
      skipCharacters();
    }
    return true;      
  }
  char parseEscapeSequence() {
    stream->nextCharacter();
    char a = stream->getCharacter();
    if(a == EOF) throw TokenizerException(this, "Expected escape character value but got EOF.");
    if(a == '\n') throw TokenizerException(this, "Expected escape character value but got newline.");
    if(isHexadecimal(a)) {
      stream->nextCharacter();
      char b = stream->getCharacter(); 
      if(b == EOF) throw TokenizerException(this, "Expected hexadecimal character value but got EOF.");
      if(b == '\n') throw TokenizerException(this, "Expected hexadecimal character value but got newline.");
      if(!isHexadecimal(b))
	throw TokenizerException(this, "Expected valid hexadecimal character value.");
      return bytesToByte(a, b);
    } else
      return escapeCharacter(stream->getCharacter());
  }
  bool getCharacter() {
    if(stream->getCharacter() != '\'')
      return false;
    stream->nextCharacter();
    token.type = Token::Type::characterT;
    if(stream->getCharacter() == '\n')
      throw TokenizerException(this, "Expected character but got newline.");
    else if(stream->getCharacter() == EOF)
      throw TokenizerException(this, "Expected character but got EOF.");
    else if(stream->getCharacter() == '\\')
      token.characterValue = parseEscapeSequence();
    else
      token.characterValue = stream->getCharacter();	 
    stream->nextCharacter();
    if(stream->getCharacter() != '\'')
      throw TokenizerException(this, "Expected closing character quote.");
    stream->nextCharacter();
    return true;      
  }
  bool getSymbol() {
    if(stream->getCharacter() != '#')
      return false;
    stream->nextCharacter();
    token.type = Token::Type::symbolT;
    token.symbol = "";
    if(!isSymbol(stream->getCharacter()))
      throw TokenizerException(this, "Expected valid identifier character in symbol.");
    while(isSymbol(stream->getCharacter())) {
      token.symbol += stream->getCharacter();
      stream->nextCharacter();
    }
    return true;
  }
  bool getSingleCharacterToken(char character, Token::Type type) {
    if(stream->getCharacter() != character)
      return false;
    token.type = type;
    stream->nextCharacter();
    return true;
  }
  bool getIdentifierKeyword() { // Either Identifier or Keyword
    if(!isIdentifier(stream->getCharacter()))
      return false;
    string symbol;
    while(isIdentifier(stream->getCharacter())) {
      symbol += stream->getCharacter();
      stream->nextCharacter();
    }
    if(stream->getCharacter() == ':') {
      stream->nextCharacter();
      token.type = Token::Type::keywordT;
      token.keyword = symbol;
    }
    else {
      token.type = Token::Type::identifierT;
      token.identifier = symbol;
    }
    return true;
  }
  bool getBinaryOperator() {
    if(!isBinaryOperator(stream->getCharacter()))
      return false;
    token.type = Token::Type::binaryOperatorT;
    token.binaryOperator = "";
    while(isBinaryOperator(stream->getCharacter())) {
      token.binaryOperator += stream->getCharacter();
      stream->nextCharacter();
    }
    return true;
  }
  Tokenizer *debugPrint() {
    TokenizerState *state = saveState();
    while(hasTokens()) {
      cout<<getToken().toString()<<endl;
      nextToken();
    }
    restoreState(state);
    return this;
  }
};

TokenizerException::TokenizerException(Tokenizer *tokenizer, string message) :
  SolException("Tokenizer", tokenizer->getToken().location, message) {}
TokenizerException::TokenizerException(Token token, string message) :
  SolException("Tokenizer", token.location, message) {}
