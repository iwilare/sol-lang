import java.io.*;
import java.nio.*;
import java.util.*;

public abstract class Message {
    public abstract int getParametersNumber();
    public abstract byte[] serialize();
    public boolean equals(Object that) { 
	return hashCode() == that.hashCode();
    }
    public static Message utilityParse(String s) {
	if(!Tokenizer.isIdentifier(s.charAt(0)))
	    return new BinaryMessage(s);
	else {
	    ArrayList<String> parts = new ArrayList<String>();
	    String part = "";
	    for(char c : s.toCharArray())
		if(c == ':') {
		    if(parts.equals(""))
			;
		    parts.add(part);
		    part = "";
		} else
		    part += c;
	    if(!part.equals(""))
		return new UnaryMessage(part);
	    else 
		return new KeywordMessage(parts.toArray(new String[parts.size()]));
	}
    }
}
class UnaryMessage extends Message {
    String message;
    UnaryMessage(String message) { this.message = message; }
    public String getUnaryMessage() { return this.message; }
    public int getParametersNumber() { return 0; }
    public String toString() { return message; }
    public int hashCode() { return 0 + message.hashCode(); }
    public KeywordMessage toKeywordMessage() {
	return new KeywordMessage(new String[]{message});
    }
    public byte[] serialize() {
	byte[] stringBytes = message.getBytes();
	ByteBuffer bytes = ByteBuffer.allocate(stringBytes.length + 1);
	bytes.put(stringBytes);
	bytes.put((byte)0);
	return bytes.array();
    }
    public static UnaryMessage deserialize(ByteBuffer bytes) {
	return new UnaryMessage(Instruction.deserializeString(bytes));
    }
}
class BinaryMessage extends Message {
    String message;
    BinaryMessage(String message) { this.message = message; }
    String getBinaryMessage() { return this.message; }
    public int getParametersNumber() { return 1; }
    public String toString() { return message; }
    public int hashCode() { return 1 + message.hashCode(); }
    public byte[] serialize() {
	byte[] stringBytes = message.getBytes();
	ByteBuffer bytes = ByteBuffer.allocate(stringBytes.length + 1);
	bytes.put(stringBytes);
	bytes.put((byte)0);
	return bytes.array();
    }
    public static BinaryMessage deserialize(ByteBuffer bytes) {
	return new BinaryMessage(Instruction.deserializeString(bytes));
    }
}
class KeywordMessage extends Message {
    String[] keywords;
    KeywordMessage(String[] keywords) { this.keywords = keywords; }
    String[] getKeywords() { return this.keywords; }
    public int getParametersNumber() { return keywords.length; }
    public String toString() {
	String string = "";
	for(String keyword : keywords)
	    string += keyword + ":";
	return string;
    }
    public int hashCode() {
	int hashCode = 2, i = 1;
	for(String keyword : keywords) 
	    hashCode += keyword.hashCode() * i++;
	return hashCode;
    }
    public byte[] serialize() {
	if(keywords.length > 256)
	    ;//throw new RuntimeException("Fatal decoding error.");
	int totalBytes = 0;
	ArrayList<byte[]> stringsBytes = new ArrayList<byte[]>();
	for(String keyword : keywords) {
	    byte[] stringBytes = keyword.getBytes();
	    stringsBytes.add(stringBytes);
	    totalBytes += stringBytes.length + 1;
	}
	ByteBuffer bytes = ByteBuffer.allocate(1 + totalBytes);
	bytes.put((byte)stringsBytes.size());
	for(byte[] stringBytes : stringsBytes) {
	    bytes.put(stringBytes);
	    bytes.put((byte)0);
	}
	return bytes.array();
    }
    public static KeywordMessage deserialize(ByteBuffer bytes) {
	byte size = bytes.get();
	String[] keywords = new String[size];
	for(int i=0; i<size; i++)
	    keywords[i] = Instruction.deserializeString(bytes);
	return new KeywordMessage(keywords);
    }
}
