class Location {
public:
  string filename;
  int line;
  int position;
  Location() :
    filename("<internal>"), line(0), position(0) {}
  Location(string filename) :
    filename(filename), line(0), position(0) {}
  Location(string filename, int line, int position) :
    filename(filename), line(line), position(position) {}
  void advanceNextLine() { line++; position=0; }
  void advanceNextCharacter() { position++; }
  string toString() {
    return filename + ":" + intToString(line+1) + ":" + intToString(position+1);
  }
};
