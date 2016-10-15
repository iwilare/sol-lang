class Instruction {
public:
  enum Type : unsigned char { MessageT,
	      Push,
	      Set,
	      PushSelf,
	      PushNull,
	      SuperMessage,
	      Pop,
	      Return,
	      PushLambda,
	      PushVector,
	      PushSymbol,
	      PushInteger,
	      PushDouble,
	      PushCharacter,
	      PushString,
	      DeleteStackFrame,
  } type;
  //union {
  Message message;
  Message superMessage;
  string pushVariable;
  string setVariable;
  
  struct {
    int skipSize;
    vector<string> parameters;
  } lambda;
  int vectorCollectionSize;
  
  string symbol;
  int integerValue;
  double doubleValue;
  char characterValue;
  string stringValue;
//};
  Instruction() {} 
  Instruction(Type type, string value) : type(type) {
    switch(type) {
    case Type::Push:         pushVariable   = value; break;
    case Type::Set:          setVariable    = value; break;

    case Type::PushSymbol: symbol = value; break;
    case Type::PushString: stringValue = value; break;
    }
  }
  Instruction(Type type, int value) : type(type) {
    switch(type) {
    case Type::PushVector:  vectorCollectionSize = value; break;
    case Type::PushInteger: integerValue         = value; break;
    }
  }
  Instruction(Type type, double value) : type(type) {
    switch(type) {
    case Type::PushDouble: doubleValue = value;
    }
  }
  Instruction(Type type, char value) : type(type) {
    switch(type) {
    case Type::PushCharacter: characterValue = value;
    }
  }
  Instruction(int skipSize, vector<string> parameters) :
    type(Type::PushLambda), lambda({skipSize, parameters}) {}
  Instruction(Message message) :
    type(Type::MessageT), message(message) {}  
  Instruction(Message message, bool superMessage) :
    type(Type::SuperMessage), superMessage(message) {}  
  Instruction(Type type) : type(type) {}
  string toString() {
    switch(type) {
    case MessageT: return "message " + message.toString();
    case Push:     return "push " + pushVariable;
    case Set:      return "set " + setVariable;
    case PushSelf: return "pushSelf";
    case PushNull: return "pushNull";
    case SuperMessage: return "superMessage " + superMessage.toString();
    case Pop:      return "pop";
    case Return:   return "return";
    case PushLambda: {
      string parameters;
      for(string parameter : lambda.parameters)
	parameters += parameter + " ";
      return "pushLambda +" + to_string(lambda.skipSize) + " | " + parameters;
    }
    case PushVector:    return "pushVector " + to_string(vectorCollectionSize);
    case PushSymbol:    return "pushSymbol #" + symbol;
    case PushInteger:   return "pushInteger " + to_string(integerValue);
    case PushDouble:    return "pushDouble " + to_string(doubleValue);
    case PushCharacter: return "pushCharacter '" + to_string(characterValue) + "'";
    case PushString:    return "pushString \"" + stringValue + "\"";
    case DeleteStackFrame: return "deleteStackFrame";
    default:
      return "<unrecognized instruction>";
    }
  }
};
