class VirtualMachine;
class Address {
public:
  enum Type { virtualT, builtinT, internalT } type;
  //union {
  CodeAddress codeAddress;
  StandardSolFunction function;
  //};
  Address() :
    type(virtualT), codeAddress() {}
  Address(int address) :
    type(virtualT), codeAddress(address) {}
  Address(CodeAddress codeAddress) :
    type(virtualT), codeAddress(codeAddress) {}
  Address(StandardSolFunction function) :
    type(builtinT), function(function) {}
  Address(bool internal) :
    type(internalT) {}
  string toString() {
    switch(type) {
    case virtualT:
      return "@Virtual:" + codeAddress.toString();
    case builtinT: {
      stringstream ss;
      ss<<function;
      return "@Builtin:" + ss.str();
    }
    case internalT:
      return "@Internal";
    }
  }
};
