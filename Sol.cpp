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

#define Version "1"
#define Log

string intToString(int a) {
  stringstream ss; ss<<a; return ss.str();
}
string doubleToString(double a) {
  stringstream ss; ss<<a; return ss.str();
}

#include "Location.cpp"

class SolException : public runtime_error {
public:
  SolException(string source, string message) :
    runtime_error(source + "@" + ""                  + ": " + message) {}
  SolException(string source, Location location, string message) :
    runtime_error(source + "@" + location.toString() + ": " + message) {}
};

#include "Log.cpp"
#include "CharacterStream.cpp"
#include "Token.cpp"
#include "Tokenizer.cpp"
#include "Message.cpp"
#include "Atom.cpp"
#include "Parser.cpp"

#include "Collectable.cpp"

class SolS;
typedef Reference<SolS> Sol;
class EnvironmentS;
typedef Reference<EnvironmentS> Environment;

class SolS : public Collectable {
private:
  Sol klass;
  map<string, Sol> variables;
  void *internalData;
public:
  SolS() : klass(nullptr), variables(), internalData(nullptr) {}
  SolS(Sol klass) : klass(klass), variables(), internalData(nullptr) {}
  SolS(Sol klass, void *data) : klass(klass), variables(), internalData(data) {}
  Sol sendWER(Message message, vector<Sol> arguments);
  ~SolS();
  inline Sol sendWER(Message message, vector<Sol> arguments, Sol klass);
  inline Sol send(Message message, vector<Sol> arguments, Sol klass);
  inline Sol send(Message message, vector<Sol> arguments);
  inline Sol superSend(Message message, vector<Sol> arguments);
  inline Sol send(Message message, vector<Sol> arguments, bool superMessage);
  
  inline Sol send(string message);
  inline Sol send(string message, vector<Sol> arguments);
  inline Sol superSend(string message);
  inline Sol superSend(string message, vector<Sol> arguments);
  Sol getClass();
  void setClass(Sol klass);
  bool hasVariable(string variable);
  Sol getVariable(string variable);
  void setVariable(string variable, Sol value);
  void defineVariable(string variable);
  void defineVariable(string variable, Sol value);
  template<typename T> T *data(Sol klass);
  template<typename T> T *data() { return (T*)internalData; }
  template<typename T> T getData(Sol klass) { return *data<T>(klass); }
  template<typename T> T getData() { return *data<T>(); }
  void setData(void *data) {
    internalData = data;
  }
  void printeronos();
};
Sol SolCreate() { return Reference<SolS>(new SolS()); }
Sol SolCreate(Sol klass) { return Reference<SolS>(new SolS(klass)); }
Sol SolCreate(Sol klass, void *data) { return Reference<SolS>(new SolS(klass, data)); }

#include "Environment.cpp"

Sol eval(Atom *a, Sol self, Sol contextClass, int LLID, Environment environment);
class EvaluationRequest {
public:
  Atom *atom;
  Sol self;
  Sol contextClass;
  Environment environment;

  Sol value;
  EvaluationRequest(Atom *atom, Sol self, Sol contextClass, Environment environment) :
    atom(atom), self(self), contextClass(contextClass), environment(environment) {}
  string toString() {
    return atom->toString();
  }
};

typedef Sol (*StandardSolFunction)(Sol self, vector<Sol> arguments);

#include "Classes.cpp"
#include "RuntimeExceptions.cpp"
#include "LambdaStructure.cpp"
#include "Evaluator.cpp"
#include "Runtime.cpp"
#include "SolStructure.cpp"
#include "Program.cpp"

// TODO

/*
Integer miao @ = nothing
3 miao
*/

/*
:e FloydWarshall.sol
A = Matrice newSize: 10 with: Infinito.
A reduce.
*/
