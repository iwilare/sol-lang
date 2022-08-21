public class Location {
    String filename;
    int line;
    int position;
    Location() {
        this.filename = "<internal>";
    }
    Location(String filename) {
        this.filename = filename;
    }
    Location(String filename, int line, int position) {
        this.filename = filename;
        this.line = line;
        this.position = position;
    }
    Location(Location location) {
        this.filename = location.filename;
        this.line = location.line;
        this.position = location.position;
    }
    public String toString() {
        return filename + ":" + Integer.toString(line+1) + ":" + Integer.toString(position+1);
    }
    void advanceNextLine() { line++; position = 0; }
    void advanceNextCharacter() { position++; }
}
