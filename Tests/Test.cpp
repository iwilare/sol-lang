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

#include "../Utility.cpp"
#include "../Location.cpp"

class SolException : public runtime_error {
public:
  SolException(string source, string message) :
    runtime_error(source + "@" + ""                  + ": " + message) {}
  SolException(string source, Location location, string message) :
    runtime_error(source + "@" + location.toString() + ": " + message) {}
};

#include "../CharacterStream.cpp"
#include "../Token.cpp"
#include "../Tokenizer.cpp"
#include "../Message.cpp"
#include "../Atom.cpp"
#include "../Parser.cpp"
#include "../Instruction.cpp"
#include "../Bytecode.cpp"
#include "../Compiler.cpp"

int main() {
  while(true) {
    string s;
    cout<<"--------------------------------------------------"<<endl;
    cout<<"> "; getline(cin,s);
    cout<<"String length: "<<s.length()<<endl;
    CharacterStream *stream = new StringStream("<input>", s);
    Tokenizer *tokenizer = new Tokenizer(stream);
    cout<<"----- Tokenizer"<<endl;
    TokenizerState *state = tokenizer->saveState();
    while(tokenizer->hasTokens()) {
      cout<<tokenizer->getToken().toString()<<endl;
      tokenizer->nextToken();
    }
    tokenizer->restoreState(state);
    cout<<"----- Parser"<<endl;
    Parser parser(tokenizer);
    Atom *atom = parser.parse();
    cout<<atom->toString()<<endl;
    cout<<"----- Compiler"<<endl;
    Compiler compiler;
    compiler.compile(atom);
    Bytecode bytecode = compiler.getCode();
    cout<<"Bytecode size: "<<bytecode.size()<<endl;
    cout<<bytecode.toString()<<endl;
    cout<<"----- Decoding"<<endl;
    int i = 0;
    while(i < bytecode.size()) {
      cout<<i<<"   ";
      cout<<bytecode.decode(i).toString()<<endl;
    }
  }
}
