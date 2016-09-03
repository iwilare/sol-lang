#include <iostream>
#include <cmath>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <functional>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

#define VERSION "0"

#include "Location.cpp"

class SolException : public runtime_error {
public:
  SolException(string source, string message) :
    runtime_error(source + "@" + ""                  + ": " + message) {}
  SolException(string source, Location location, string message) :
    runtime_error(source + "@" + location.toString() + ": " + message) {}
};

#include "CharacterStream.cpp"
#include "Token.cpp"
#include "Tokenizer.cpp"
#include "Message.cpp"
#include "Atom.cpp"
#include "Parser.cpp"
#include "Collectable.cpp"

class Sol : public Collectable {
public:
  Sol *klass;
  map<string, Sol*> variables;
  void *data;
  Sol() : klass(nullptr), variables(), data(nullptr) {}
  Sol(Sol *klass) : klass(klass), variables(), data(nullptr) {
    klass->reference();
  }
  Sol(Sol *klass, void *data) : klass(klass), variables(), data(data) {
    klass->reference();
  }
  ~Sol() { send("deallocate"); }
  Sol *genericSend(Message message, vector<Sol*> arguments, bool superMessage);
  Sol *send(string message);
  Sol *send(string message, vector<Sol*> arguments);
  Sol *send(Message message, vector<Sol*> arguments);
  Sol *superSend(string message);
  Sol *superSend(string message, vector<Sol*> arguments);
  Sol *superSend(Message message, vector<Sol*> arguments);
  bool hasVariable(string variable);
  Sol *getVariable(Atom *variable);
  Sol *getVariable(string variable);
  Sol *getVariable(Location location, string variable);
  void setVariable(string variable, Sol *value);
  void recursiveDereferenceStep();
  void printeronos();
};

#include "Environment.cpp"

typedef Sol *(*StandardSolFunction)(Sol *self, vector<Sol*> arguments);
class SolContinuation {
public:
  Sol *returnValue;
  int LLID;
  SolContinuation(Sol *returnValue, int LLID) :
    returnValue(returnValue), LLID(LLID) {}
};
Sol *eval(int LLID, Atom *a, Environment *e);

#include "Classes.cpp"
#include "RuntimeExceptions.cpp"
#include "LambdaStructure.cpp"
#include "Evaluator.cpp"
#include "Runtime.cpp"
#include "Semantics.cpp"
#include "Program.cpp"

