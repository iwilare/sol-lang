import java.util.*;

public abstract class Message {
    public abstract int getParametersNumber();
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
    /*public boolean equals(Object message) {
        if(!(message instanceof UnaryMessage))
            return false;
        UnaryMessage that = (UnaryMessage)message;
        return this.message.equals(that.message);
    }*/
}
class BinaryMessage extends Message {
    String message;
    BinaryMessage(String message) { this.message = message; }
    String getBinaryMessage() { return this.message; }
    public int getParametersNumber() { return 1; }
    public String toString() { return message; }
    public int hashCode() { return 1 + message.hashCode(); }
    /*public boolean equals(Object message) {
        if(!(message instanceof BinaryMessage))
            return false;
        BinaryMessage that = (BinaryMessage)message;
        return this.message.equals(that.message);
    }*/
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
    /*public boolean equals(Object message) {
        if(!(message instanceof KeywordMessage))
            return false;
        KeywordMessage that = (KeywordMessage)message;
        if(this.keywords.length != that.keywords.length)
            return false;
        for(int i=0; i<keywords.length; i++)
            if(!this.keywords[i].equals(that.keywords[i]))
                return false;
        return true;
        }*/
}
