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

typedef unsigned char byte;

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

#include "Instruction.cpp"
#include "Bytecode.cpp"
#include "Compiler.cpp"

#include "Collectable.cpp"

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
#include "Classes.cpp"

#include "RuntimeExceptions.cpp"
#include "SolStructure.cpp"
#include "Address.cpp"
#include "VirtualMachine.cpp"
#include "Runtime.cpp"

#include "Program.cpp"
