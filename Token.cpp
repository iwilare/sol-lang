class Token {
public:
  enum Type : unsigned char { Integer, Double, String, Character,
	      Symbol, Identifier, Keyword, BinaryOperator,
	      SequenceSeparator, Pipe, MethodMark,
	      VectorStart, VectorEnd, VectorSeparator,
	      LambdaStart, LambdaSeparator, LambdaEnd,
	      ParenthesisOpen, ParenthesisClose,
	      Eof
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
      (type == Type::BinaryOperator and binaryOperator == "-") or
      type == Type::Identifier or
      type == Type::Integer or
      type == Type::Double or
      type == Type::String or
      type == Type::Character or
      type == Type::Symbol or
      type == Type::LambdaStart or
      type == Type::MethodMark or
      type == Type::VectorStart or
      type == Type::ParenthesisOpen;
  }
  string toString() {
    switch(type) {
    case Type::Identifier:
      return "Identifier " + identifier;
    case Type::Integer:
      return "Integer " + intToString(integerValue);
    case Type::Double:
      return "Double " + doubleToString(doubleValue);
    case Type::String:
      return "String \"" + stringValue + "\"";
    case Type::Character:
      return "Character '" + std::string(1,characterValue) + "'";
    case Type::Symbol:
      return "Symbol #" + symbol;
    case Type::Keyword:
      return "Keyword " + keyword;
    case Type::BinaryOperator:
      return "BinaryOperator " + binaryOperator;
    case Type::SequenceSeparator:
      return ".";
    case Type::Pipe:
      return ";";
    case Type::MethodMark:
      return "@";
    case Type::VectorStart:
      return "[";
    case Type::VectorSeparator:
      return ",";      
    case Type::VectorEnd:
      return "]";
    case Type::LambdaStart:
      return "{";
    case Type::LambdaSeparator:
      return "|";
    case Type::LambdaEnd:
      return "}";
    case Type::ParenthesisOpen:
      return "(";
    case Type::ParenthesisClose:
      return ")";
    case Type::Eof:
      return "<EOF>";
    default:
      return "<unrecognized token>";
    }
  }
};
