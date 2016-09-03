class Atom;
string atomVectorToString(vector<Atom*> v, string mid);

class Atom {
public:
  Location location;
  enum Type { messageT,
	      identifierT,
	      assignmentT,
	      lambdaT, returnT,
	      sequenceT,
	      vectorT,
	      symbolT, integerT, doubleT, characterT, stringT } type;
  //union {
  string identifier;
  string symbol;
  int integerValue;
  double doubleValue;
  char characterValue;
  string stringValue;
  Atom *returnExpression;
  vector<Atom*> sequence;
  vector<Atom*> vectorElements;
  struct {
    Atom *receiver;
    Message message;
    vector<Atom*> arguments;
  } message;
  struct {
    string variable;
    Atom *value;
  } assignment;
  struct {
    vector<string> parameters;
    Atom *body;
  } lambda;
  Atom(Type type) : type(type) {}
  Atom(Location location) : location(location) {}
  Atom(string identifier) :
    type(Type::identifierT), identifier(identifier) {}
  Atom(Atom *variable, Atom *value) :
    type(Type::assignmentT), location(variable->location),
    assignment({variable->identifier, value}) {}
  Atom(vector<Atom*> sequence) :
    type(Type::sequenceT), location(sequence[0]->location),
    sequence(sequence) {}
  Atom(Atom *receiver, Message messageName, vector<Atom*> arguments) :
    type(Type::messageT), location(receiver->location),
    message({receiver, messageName, arguments}) {}
  /*struct {
    Message message;
    vector<string> parameters;
    Atom *body;
    } method;*/
  //};
  string toString() {
    if(this == nullptr)
      return "<nothing>";
    string s;
    switch(type) {
    case Type::messageT:
      return "((" + message.receiver->toString() + ") " + message.message.toString() + " [" +
	atomVectorToString(message.arguments, ", ") + "])";
    case Type::identifierT:
      return identifier;
    case Type::assignmentT:
      return assignment.variable + " := " + assignment.value->toString();
    case Type::lambdaT:
      if(lambda.parameters.size() == 0)
	return "{ " + lambda.body->toString() + " }";
      for(string p : lambda.parameters)
	s += p + " ";
      return "{ " + s + "| "  + lambda.body->toString() + " }";
    case Type::returnT:
      return "(Return " + returnExpression->toString() + ")";
    case Type::sequenceT:
      for(Atom *a : sequence)
	s += " " + a->toString() + ".";
      return "(Sequence" + s + ")";
    case Type::vectorT:
      return "[" + atomVectorToString(vectorElements, ", ") + "]";     
    case Type::symbolT:
      return "#" + symbol;
    case Type::integerT:
      return to_string(integerValue);
    case Type::doubleT:
      return to_string(doubleValue);
    case Type::characterT:
      return "'" + string(1,characterValue) + "'";
    case Type::stringT:
      return "\"" + stringValue + "\"";
    default:
      return "<unrecognized atom>";
    }
  }
};

string atomVectorToString(vector<Atom*> v, string mid) {
  string s;
  if(v.size() == 0)
    return "";
  for(int i=0; i<v.size()-1; i++)
    s += v[i]->toString() + mid;
  s += v.back()->toString();
  return s;
}
