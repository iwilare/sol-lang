class Atom;
string atomVectorToString(vector<Atom*> v, string mid);

class Atom {
public:
  Location location;
  enum Type : unsigned char { MessageT,
	      Identifier,
	      Assignment,
	      Lambda,
	      Sequence,
	      Vector,
	      Symbol, Integer, Double, Character, String } type;
  //union {
  string identifier;
  string symbol;
  int integerValue;
  double doubleValue;
  char characterValue;
  string stringValue;
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
    type(Type::Identifier), identifier(identifier) {}
  Atom(Atom *variable, Atom *value) :
    type(Type::Assignment), location(variable->location),
    assignment({variable->identifier, value}) {}
  Atom(vector<Atom*> sequence) :
    type(Type::Sequence), location(sequence[0]->location),
    sequence(sequence) {}
  Atom(Atom *receiver, Message messageName, vector<Atom*> arguments) :
    type(Type::MessageT), location(receiver->location),
    message({receiver, messageName, arguments}) {}
  string toString() {
    if(this == nullptr)
      return "<nothing>";
    string s;
    switch(type) {
    case Type::MessageT:
      return "<" + message.receiver->toString() + " " + message.message.toString() + " | " +
	atomVectorToString(message.arguments, " | ") + ">";
    case Type::Identifier:
      return identifier;
    case Type::Assignment:
      return assignment.variable + " := " + assignment.value->toString();
    case Type::Lambda:
      if(lambda.parameters.size() == 0)
	return "{ " + lambda.body->toString() + " }";
      for(string p : lambda.parameters)
	s += p + " ";
      return "{ " + s + "| "  + lambda.body->toString() + " }";
    case Type::Sequence:
      for(Atom *a : sequence)
	s += " " + a->toString() + ".";
      return "(Sequence" + s + ")";
    case Type::Vector:
      return "[" + atomVectorToString(vectorElements, ", ") + "]";     
    case Type::Symbol:
      return "#" + symbol;
    case Type::Integer:
      return intToString(integerValue);
    case Type::Double:
      return doubleToString(doubleValue);
    case Type::Character:
      return "'" + string(1,characterValue) + "'";
    case Type::String:
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
