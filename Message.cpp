class Message {
public:
  vector<string> parts;
  int parametersNumber;
  Message() {}
  Message(string s) {
    string part;
    parametersNumber = 0;
    for(char c : s)
      if(c == ':') {
	parametersNumber++;
	parts.push_back(part);
	part = "";
      } else
	part += c;
    if(part != "")
      parts.push_back(part);
  }
  Message(vector<string> parts, int parametersNumber) :
    parts(parts), parametersNumber(parametersNumber) {}
  string toString() {
    if(parametersNumber == 0)
      return parts[0];
    string name;
    for(string s : parts)
      name += s + ":";
    return name;
  }
};
