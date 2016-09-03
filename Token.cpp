class Token {
public:
  enum Type { integerT, doubleT, stringT, characterT,
	      symbolT, identifierT, keywordT, binaryOperatorT,
	      returnTokenT, sequenceSeparatorT, pipeT, methodMarkT,
	      vectorStartT, vectorEndT, vectorSeparatorT,
	      lambdaStartT, lambdaSeparatorT, lambdaEndT,
	      parenthesisOpenT, parenthesisCloseT,
	      eofT
  } type;
  Location location;
  //union {
  string identifier;
  string symbol;
  string keyword;
  string binaryOperator;
  int integerValue;
  double doubleValue;
  string stringValue;
  char characterValue;
  Token() {}
  Token(Location location) : location(location) {}
  Token(Location location, Type type) : location(location), type(type) {}
  //};
  bool isObjectStart() {
    return
      (type == Type::binaryOperatorT and binaryOperator == "-") or
      type == Type::identifierT or
      type == Type::integerT or
      type == Type::doubleT or
      type == Type::stringT or
      type == Type::characterT or
      type == Type::symbolT or
      type == Type::lambdaStartT or
      type == Type::methodMarkT or
      type == Type::returnTokenT or
      type == Type::vectorStartT or
      type == Type::parenthesisOpenT;
  }
  string toString() {
    switch(type) {
    case Type::identifierT:
      return "Identifier " + identifier;
    case Type::integerT:
      return "Integer " + to_string(integerValue);
    case Type::doubleT:
      return "Double " + to_string(doubleValue);
    case Type::stringT:
      return "String \"" + stringValue + "\"";
    case Type::characterT:
      return "Character '" + std::string(1,characterValue) + "'";
    case Type::symbolT:
      return "Symbol #" + symbol;
    case Type::keywordT:
      return "Keyword " + keyword;
    case Type::binaryOperatorT:
      return "BinaryOperator " + binaryOperator;
    case Type::returnTokenT:
      return "^";
    case Type::sequenceSeparatorT:
      return ".";
    case Type::pipeT:
      return ";";
    case Type::methodMarkT:
      return "@";
    case Type::vectorStartT:
      return "[";
    case Type::vectorSeparatorT:
      return ",";      
    case Type::vectorEndT:
      return "]";
    case Type::lambdaStartT:
      return "{";
    case Type::lambdaSeparatorT:
      return "|";
    case Type::lambdaEndT:
      return "}";
    case Type::parenthesisOpenT:
      return "(";
    case Type::parenthesisCloseT:
      return ")";
    case Type::eofT:
      return "<EOF>";
    default:
      return "<unrecognized token>";
    }
  }
};
