import java.io.*;
import java.nio.file.*;
import java.util.*;

public abstract class CharacterStream {
    int index;
    boolean eof;
    Location location;

    class State {
        public int index;
        public boolean eof;
        public Location location;
        State(int index, boolean eof, Location location) {
            this.index = index;
            this.eof = eof;
            this.location = new Location(location);
        }
    }

    CharacterStream(String filename) {
        this.location = new Location(filename);
    }
    public boolean hasCharacters() { return !eof; }
    public Location getLocation() { return location; }
    public void EOFState() { eof = true; }
    public abstract void close();
    public abstract char getCharacterSpecialized();
    public abstract void nextCharacterSpecialized();
    public abstract void restoreStateSpecialized(State state);
    public State saveState() {
        return new State(index, eof, location);
    }
    public void restoreState(State state) {
        index    = state.index;
        eof      = state.eof;
        location = state.location;

        restoreStateSpecialized(state);
    }
    public char getCharacter() {
        return !hasCharacters() ? 0 : getCharacterSpecialized();
    }
    public void nextCharacter() {
        if(!hasCharacters())
            return;
        if(getCharacter() == '\n')
            location.advanceNextLine();
        else
            location.advanceNextCharacter();
        index++;
        nextCharacterSpecialized();
    }
}

class StringStream extends CharacterStream {
    String line;
    StringStream(String line) {
        super("<input>");
        this.line = line;
        if(this.line.length() == 0)
            EOFState();
    }
    StringStream(String filename, String line) {
        super(filename);
        this.line = line;
        if(this.line.length() == 0)
            EOFState();
    }
    public char getCharacterSpecialized() { return line.charAt(index); }
    public void nextCharacterSpecialized() {
        if(index >= line.length())
            EOFState();
    }
    public void restoreStateSpecialized(State state) {}
    public void close() {}
}

class FileStream extends StringStream {
    String line;
    String content;
    FileStream(String filename) throws IOException {
        super(filename, readWholeFile(filename));
    }
    static String readWholeFile(String filename) throws IOException {
        return new String(Files.readAllBytes(Paths.get(filename)));
    }
}

class MultilineReplStream extends CharacterStream {
    String prompt;
    String multilinePrompt;
    String content;
    String lastLine;
    MultilineReplStream(String prompt) {
        super("<input>");
        this.content = "";
        this.lastLine = "";
        this.prompt = prompt;
        nextCharacterSpecialized();
    }
    public char getCharacterSpecialized() { return content.charAt(index); }
    public void nextCharacterSpecialized() {
        if(index >= content.length()) {
            try {
                System.out.print(prompt);
                BufferedReader bufferedIn = new BufferedReader(new InputStreamReader(System.in));
                lastLine = bufferedIn.readLine();
                if(lastLine == null)
                    EOFState();
                else
                    content += lastLine + '\n';
            } catch(IOException e) {
                EOFState();
            }
        }
    }
    public void restoreStateSpecialized(State state) { lastLine = ""; }
    public void close() {}
}
