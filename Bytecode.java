import java.io.*;
import java.nio.*;

/*class Bytecode {
    ByteArrayOutputStream bytes;
    int index = 0;
    Bytecode() { bytes = new ByteArrayOutputStream(); }
    Bytecode(ArrayList<Instruction> instructions) {
	for(Instruction instruction : instructions)
	    serialize(instruction);
    }
    byte[] array() { return bytes.array(); }
    byte get(int i) { return bytes.get(i); }
    int getIndex() { return index; }
    //void serialize() { serialize((byte)0); }
    void serialize(byte x) { bytes.write(x); }
    void serialize(int x) { bytes.write(ByteBuffer.allocate(4).putInt(x)); }
    void serializeCount(int x) { bytes.write((byte)x); }
    void serializeType(byte x) { serialize(x); }
    void serialize(string s) {
	for(byte b : s)
	    serialize(b);
	serialize();
    }
  void serialize(ArrayList<string> v) {
    for(string s : v)
      serialize(s);
  }
  void serialize(Message m) {
    serializeCount(m.parametersNumber);
    serialize(m.parts);
  }
  template<typename T> void serialize(T value) {
    byte p = (byte)&value;
    for(int i=0; i<sizeof(T); i++)
      serialize(p++);
  }
  int serialize(Instruction i) {
    int index = size();
    serializeType(i.type);
    switch(i.type) {
    case Instruction::Type::MessageT:
      serialize(i.message); break;      
    case Instruction::Type::Push:
      serialize(i.pushVariable); break;
    case Instruction::Type::Set:
      serialize(i.pushVariable); break;
    case Instruction::Type::PushSelf:
    case Instruction::Type::PushNull:
    case Instruction::Type::Pop:
    case Instruction::Type::Return:
    case Instruction::Type::DeleteStackFrame:
      break;
    case Instruction::Type::SuperMessage:
      serialize(i.superMessage); break;
    case Instruction::Type::PushLambda:
      serialize(i.lambda.skipSize);    
      serializeCount(i.lambda.parameters.size());
      serialize(i.lambda.parameters); break;
    case Instruction::Type::PushArrayList:
      serialize(i.ArrayListCollectionSize); break;
    case Instruction::Type::PushSymbol:
      serialize(i.symbol); break;
    case Instruction::Type::PushInteger:
      serialize(i.integerValue); break;
    case Instruction::Type::PushDouble:
      serialize(i.doubleValue); break;
    case Instruction::Type::PushCharacter:
      serialize(i.characterValue); break;
    case Instruction::Type::PushString:
      serialize(i.stringValue); break;
    }
    return index;
  }
  int serialize(int index, Instruction i) {
    
  }
  void decode(int &i, byte &b) {
    b = bytes[i++];
  }
  void decodeCount(int &i, int &c) {
    decode(i, (byte&)c);
  }
  void decodeType(int &i, Instruction::Type &x) {
    decode(i, (byte&)x);
  }
  void decode(int &i, string &s) {
    s = "";
    byte b;
    while((b = bytes[i++]) != 0)
      s += (char)b;
  }
  void decode(int &i, int count, ArrayList<string> &v) {
    string s;
    for(int n=0; n<count; n++) {
      decode(i, s);
      v.push_back(s);
    }
  }
  void decode(int &i, Message &m) {
    decodeCount(i, m.parametersNumber);
    decode(i, m.parametersNumber == 0 ? 1 : m.parametersNumber, m.parts);
  }
  template<typename T> void decode(int &i, T &value) {
    byte b = (byte)&value;
    for(int j=0; j<sizeof(T); j++, i++)
      (b++) = bytes[i];
  }
  Instruction decode(int &i) {
    Instruction instr;
    decodeType(i,instr.type);
    switch(instr.type) {
    case Instruction::Type::MessageT:
      decode(i,instr.message); break;      
    case Instruction::Type::Push:
      decode(i,instr.pushVariable); break;
    case Instruction::Type::Set:
      decode(i,instr.pushVariable); break;
    case Instruction::Type::PushSelf:
    case Instruction::Type::PushNull:
    case Instruction::Type::Pop:
    case Instruction::Type::Return:
    case Instruction::Type::DeleteStackFrame:
      break;
    case Instruction::Type::SuperMessage:
      decode(i,instr.superMessage); break;
    case Instruction::Type::PushLambda:
      decode(i,instr.lambda.skipSize);
      int parametersNumber;
      decodeCount(i,parametersNumber);
      decode(i,parametersNumber,instr.lambda.parameters); break;
    case Instruction::Type::PushArrayList:
      decode(i,instr.ArrayListCollectionSize); break;
    case Instruction::Type::PushSymbol:
      decode(i,instr.symbol); break;
    case Instruction::Type::PushInteger:
      decode(i,instr.integerValue); break;
    case Instruction::Type::PushDouble:
      decode(i,instr.doubleValue); break;
    case Instruction::Type::PushCharacter:
      decode(i,instr.characterValue); break;
    case Instruction::Type::PushString:
      decode(i,instr.stringValue); break;
    default:
      throw BytecodeException("Unrecognized instruction type.");
    }
    return instr;
  }
  ArrayList<Instruction> decodeAll() {
    int i;
    ArrayList<Instruction> v;
    while(i < size())
      v.push_back(decode(i));
  }
  string toString() {
    stringstream ss;
    for(int i=0; i<size(); i++)
      ss<<(int)bytes[i]<<" ";
    return ss.str();
  }
};
*/
