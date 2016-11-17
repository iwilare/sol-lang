import java.io.*;
import java.nio.*;
import java.util.*;

abstract class Instruction {
    public static enum Type {
	UnaryMessageInstruction((byte)0x00),
	BinaryMessageInstruction((byte)0x01),
	KeywordMessageInstruction((byte)0x02),
	SuperUnaryMessageInstruction((byte)0x03),
	SuperBinaryMessageInstruction((byte)0x04),
	SuperKeywordMessageInstruction((byte)0x05),
	PushInstruction((byte)0x06),
	PushSelfInstruction((byte)0x07),
	PushNothingInstruction((byte)0x08),
	PopInstruction((byte)0x09),
	ReturnInstruction((byte)0x0A),
	AssignmentInstruction((byte)0x0B),
	PushLambdaInstruction((byte)0x0C),
	PushVectorInstruction((byte)0x0D),
	PushIntegerInstruction((byte)0x0E),
	PushDoubleInstruction((byte)0x0F),
	PushCharacterInstruction((byte)0x10),
	PushStringInstruction((byte)0x011),
	PushMessageInstruction((byte)0x012);

	byte value;
	public byte id() { return value; }
	Type(byte value) {
	    this.value = value;
	}
    };
    public abstract byte[] serialize();
    public static String deserializeString(ByteBuffer bytes) {
	StringBuilder sb = new StringBuilder();
	while(bytes.remaining() > 0) {
	    char c = bytes.getChar();
	    if(c == 0)
		break;
	    sb.append(c);
	}
	return sb.toString();
    }
    public static Instruction deserialize(byte[] byteArray, int index) {
	ByteBuffer bytes = ByteBuffer.wrap(byteArray, index, byteArray.length - index);
	Type type = new Type(bytes.get());
	switch(type) {
	case UnaryMessageInstruction:
	    UnaryMessage message = UnaryMessage.deserialize(bytes);
	    return new UnaryMessageInstruction(message);
	case BinaryMessageInstruction:
	    BinaryMessage message = BinaryMessage.deserialize(bytes);
	    return new BinaryMessageInstruction(message);
	case KeywordMessageInstruction:
	    KeywordMessage message = KeywordMessage.deserialize(bytes);
	    return new KeywordMessageInstruction(message);
	case SuperUnaryMessageInstruction:
	    SuperUnaryMessage message = UnaryMessage.deserialize(bytes);
	    return new SuperUnaryMessageInstruction(message);
	case SuperBinaryMessageInstruction:
	    SuperBinaryMessage message = BinaryMessage.deserialize(bytes);
	    return new SuperBinaryMessageInstruction(message);
	case SuperKeywordMessageInstruction:
	    SuperKeywordMessage message = KeywordMessage.deserialize(bytes);
	    return new SuperKeywordMessageInstruction(message);
	case PushInstruction:
	    String variable = Instruction.deserializeString(bytes);
	    return new PushInstruction(variable);
	case PushSelfInstruction:
	    return new PushSelfInstruction();
	case PushNothingInstruction:
	    return new PushNothingInstruction();
	case PopInstruction:
	    return new PopInstruction();
	case ReturnInstruction:
	    return new ReturnInstruction();
	case AssignmentInstruction:
	    String variable = Instruction.deserializeString(bytes);
	    return new AssignmentInstruction(variable);
        case PushLambdaInstruction:
	    int lambdaSize = bytes.getInt();
	    String[] parameters = KeywordMessage.deserialize(bytes).getKeywords();
	    return new PushLambdaInstruction(lambdaSize, parameters);
	case PushVectorInstruction:
	    int vectorSize = bytes.getInt();
	    return new PushVectorInstruction(vectorSize);
	case PushIntegerInstruction:
	    int value = bytes.getInt();
	    return new PushIntegerInstruction(value);
	case PushDoubleInstruction:
	    Double value = bytes.getDouble();
	    return new PushDoubleInstruction(value);
	case PushCharacterInstruction:
	    Character value = bytes.getCharacter();
	    return new PushCharacterInstruction(value);
	case PushStringInstruction:
	    String value = bytes.getString();
	    return new PushStringInstruction(value);
	case PushMessageInstruction:
	    byte[] messageBytes = message.serialize();
	ByteBuffer bytes = ByteBuffer.allocate(1 + messageBytes.length);
	bytes.put(Instruction.Type.PushMessageInstruction.id());
	if(message instanceof UnaryMessage)
	    bytes.put(Instruction.Type.UnaryMessageInstruction.id());
	else if(message instanceof BinaryMessage)
	    bytes.put(Instruction.Type.BinaryMessageInstruction.id());
	else if(message instanceof KeywordMessage)
	    bytes.put(Instruction.Type.KeywordMessageInstruction.id());
	bytes.put(messageBytes);
	return bytes.array();

	Message value = bytes.getMessage();
	    return new PushMessageInstruction(value);
	}
	return null;
    }
}
abstract class MessageInstruction extends Instruction {
    public abstract Message getMessage();
}
class UnaryMessageInstruction extends MessageInstruction {
    UnaryMessage message;
    UnaryMessageInstruction(UnaryMessage message) {
	this.message = message;
    }
    public Message getMessage() { return this.message; }
    public String toString() { return "message " + message; }
    public byte[] serialize() {
	byte[] messageBytes = message.serialize();
	ByteBuffer bytes = ByteBuffer.allocate(1 + messageBytes.length);
	bytes.put(Instruction.Type.UnaryMessageInstruction.id());
	bytes.put(messageBytes);
	return bytes.array();
    }
}
class BinaryMessageInstruction extends MessageInstruction {
    BinaryMessage message;
    BinaryMessageInstruction(BinaryMessage message) {
	this.message = message;
    }
    public Message getMessage() { return this.message; }
    public String toString() { return "message " + message; }
    public byte[] serialize() {
	byte[] messageBytes = message.serialize();
	ByteBuffer bytes = ByteBuffer.allocate(1 + messageBytes.length);
	bytes.put(Instruction.Type.BinaryMessageInstruction.id());
	bytes.put(messageBytes);
	return bytes.array();
    }
}
class KeywordMessageInstruction extends MessageInstruction {
    KeywordMessage message;
    KeywordMessageInstruction(KeywordMessage message ) {
	this.message = message;
    }
    public Message getMessage() { return this.message; }
    public String toString() { return "message " + message; }
    public byte[] serialize() {
	byte[] messageBytes = message.serialize();
	ByteBuffer bytes = ByteBuffer.allocate(1 + messageBytes.length);
	bytes.put(Instruction.Type.KeywordMessageInstruction.id());
	bytes.put(messageBytes);
	return bytes.array();
    }
}
//-------
class SuperUnaryMessageInstruction extends MessageInstruction {
    SuperUnaryMessage message;
    SuperUnaryMessageInstruction(UnaryMessage message) {
	this.message = message;
    }
    public Message getMessage() { return this.message; }
    public String toString() { return "superMessage " + message; }
    public byte[] serialize() {
	byte[] messageBytes = message.serialize();
	ByteBuffer bytes = ByteBuffer.allocate(1 + messageBytes.length);
	bytes.put(Instruction.Type.SuperUnaryMessageInstruction.id());
	bytes.put(messageBytes);
	return bytes.array();
    }
}
class SuperBinaryMessageInstruction extends MessageInstruction {
    SuperBinaryMessage message;
    SuperBinaryMessageInstruction(BinaryMessage message) {
	this.message = message;
    }
    public Message getMessage() { return this.message; }
    public String toString() { return "superMessage " + message; }
    public byte[] serialize() {
	byte[] messageBytes = message.serialize();
	ByteBuffer bytes = ByteBuffer.allocate(1 + messageBytes.length);
	bytes.put(Instruction.Type.SuperBinaryMessageInstruction.id());
	bytes.put(messageBytes);
	return bytes.array();
    }
}
class SuperKeywordMessageInstruction extends MessageInstruction {
    SuperKeywordMessage message;
    SuperKeywordMessageInstruction(KeywordMessage message ) {
	this.message = message;
    }
    public Message getMessage() { return this.message; }
    public String toString() { return "superMessage " + message; }
    public byte[] serialize() {
	byte[] messageBytes = message.serialize();
	ByteBuffer bytes = ByteBuffer.allocate(1 + messageBytes.length);
	bytes.put(Instruction.Type.SuperKeywordMessageInstruction.id());
	bytes.put(messageBytes);
	return bytes.array();
    }
}
//-------
class PushInstruction extends Instruction {
    String identifier;
    PushInstruction(String identifier) { this.identifier = identifier; }
    public String getIdentifier() { return this.identifier; }
    public String toString() { return "push " + identifier; }
    public byte[] serialize() {
	byte[] identifierBytes = identifier.getBytes();
	ByteBuffer bytes = ByteBuffer.allocate(1 + identifierBytes.length + 1);
	bytes.put(Instruction.Type.PushInstruction.id());
	bytes.put(identifierBytes);
	bytes.put((byte)0);
	return bytes.array();
    }
}
class PushSelfInstruction extends Instruction {
    PushSelfInstruction() { }
    public String toString() { return "pushSelf"; }
    public byte[] serialize() { return new byte[]{Instruction.Type.PushSelfInstruction.id()}; }
}
class PushNothingInstruction extends Instruction {
    PushNothingInstruction() { }
    public String toString() { return "pushNothing"; }
    public byte[] serialize() { return new byte[]{Instruction.Type.PushNothingInstruction.id()}; }
}
class ReturnInstruction extends Instruction {
    ReturnInstruction() { }
    public String toString() { return "return"; }
    public byte[] serialize() { return new byte[]{Instruction.Type.ReturnInstruction.id()}; }
}
class AssignmentInstruction extends Instruction {
    String variable;
    AssignmentInstruction(String variable) {
	this.variable = variable;
    }
    public String getVariable() { return this.variable; }
    public String toString() { return "set " + variable; }
    public byte[] serialize() {
	byte[] variableBytes = variable.getBytes();
	ByteBuffer bytes = ByteBuffer.allocate(1 + variableBytes.length + 1);
	bytes.put(Instruction.Type.AssignmentInstruction.id());
	bytes.put(variableBytes);
	bytes.put((byte)0);
	return bytes.array();
    }
}
class PushLambdaInstruction extends Instruction {
    int lambdaSize;
    String[] parameters;
    PushLambdaInstruction(int lambdaSize, String[] parameters) {
	this.lambdaSize = lambdaSize;
	this.parameters = parameters;
    }
    public int getLambdaSize() { return this.lambdaSize; }
    public String[] getParameters() { return this.parameters; }
    public String toString() {
	return "pushLambda " + lambdaSize + " {" + Arrays.toString(parameters) + " }";
    }
    public byte[] serialize() {
	if(parameters.length > 256)
	    System.err.println("FATAL DECODING ERROR."); // TODO
	int totalBytes = 0;
	ArrayList<byte[]> stringsBytes = new ArrayList<byte[]>();
	for(String parameter : parameters) {
	    byte[] stringBytes = parameter.getBytes();
	    stringsBytes.add(stringBytes);
	    totalBytes += stringBytes.length + 1;
	}
	ByteBuffer bytes = ByteBuffer.allocate(1 + 4 + 1 + totalBytes);
	bytes.put(Instruction.Type.PushLambdaInstruction.id());
	bytes.putInt(lambdaSize);
	bytes.put((byte)stringsBytes.size());
	for(byte[] stringBytes : stringsBytes) {
	    bytes.put(stringBytes);
	    bytes.put((byte)0);
	}
	return bytes.array();
    }
}
class PushVectorInstruction extends Instruction {
    int vectorSize;
    PushVectorInstruction(int vectorSize) {
	this.vectorSize = vectorSize;
    }
    public String toString() { return "pushVector " + vectorSize; }
    public byte[] serialize() {
	ByteBuffer bytes = ByteBuffer.allocate(1 + 4);
	bytes.put(Instruction.Type.PushVectorInstruction.id());
	bytes.putInt(vectorSize);
	return bytes.array();
    } 
}
class PushIntegerInstruction extends Instruction {
    int value;
    PushIntegerInstruction(int value) { this.value = value; }
    public int getInteger() { return this.value; }
    public String toString() { return "pushInteger " + value; }
    public byte[] serialize() {
	ByteBuffer bytes = ByteBuffer.allocate(1 + 4);
	bytes.put(Instruction.Type.PushIntegerInstruction.id());
	bytes.putInt(value);
	return bytes.array();
    }
}
class PushDoubleInstruction extends Instruction {
    double value;
    PushDoubleInstruction(double value) { this.value = value; }
    public double getDouble() { return this.value; }
    public String toString() { return "pushDouble " + value; }
    public byte[] serialize() {
	ByteBuffer bytes = ByteBuffer.allocate(1 + 8);
	bytes.put(Instruction.Type.PushDoubleInstruction.id());
	bytes.putDouble(value);
	return bytes.array();
    }
}
class PushCharacterInstruction extends Instruction {
    char value;
    PushCharacterInstruction(char value) { this.value = value; }
    public char getCharacter() { return this.value; }
    public String toString() { return "pushCharacter '" + value + "'"; }
    public byte[] serialize() {
	ByteBuffer bytes = ByteBuffer.allocate(1 + 2);
	bytes.put(Instruction.Type.PushCharacterInstruction.id());
	bytes.putChar(value);
	return bytes.array();
    }
}
class PushStringInstruction extends Instruction {
    String value;
    PushStringInstruction(String value) { this.value = value; }
    public String getString() { return this.value; }
    public String toString() { return "pushString \"" + value + "\""; }
    public byte[] serialize() {
	byte[] stringBytes = value.getBytes();
	ByteBuffer bytes = ByteBuffer.allocate(1 + 4 + stringBytes.length);
	bytes.put(Instruction.Type.PushStringInstruction.id());
	bytes.putInt(stringBytes.length);
	bytes.put(stringBytes);
	return bytes.array();
    }
}
class PushMessageInstruction extends Instruction {
    Message message;
    PushMessageInstruction(Message message) { this.message = message; }
    public Message getMessage() { return this.message; }
    public String toString() { return "pushMessage #" + message; }
    public byte[] serialize() {
	byte[] messageBytes = message.serialize();
	ByteBuffer bytes = ByteBuffer.allocate(1 + 1 + messageBytes.length);
	bytes.put(Instruction.Type.PushMessageInstruction.id());
	if(message instanceof UnaryMessage)
	    bytes.put(Instruction.Type.UnaryMessageInstruction.id());
	else if(message instanceof BinaryMessage)
	    bytes.put(Instruction.Type.BinaryMessageInstruction.id());
	else if(message instanceof KeywordMessage)
	    bytes.put(Instruction.Type.KeywordMessageInstruction.id());
	bytes.put(messageBytes);
	return bytes.array();
    }
}
