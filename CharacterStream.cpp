class StreamState {
public:
  int index;
  bool ended;
  Location location;
  StreamState(int index, bool ended, Location location) :
    index(index), ended(ended), location(location) {}
};
class StreamException : public SolException {
public:
  StreamException(string s) :
    SolException("Stream", s) {}
};
/*abstract*/class CharacterStream {
public:
  int index;
  bool ended;
  bool closed;
  Location location;
  CharacterStream(string filename) :
    location(filename), index(0), ended(false), closed(false){}
  CharacterStream(Location location) :
    location(location), index(0), ended(false), closed(false) {}
  bool isEOF() { return ended; }
  bool EOFState() { ended = true; }
  void close() { closed = true; closeSpecialized(); }
  virtual char getCharacterSpecialized() = 0;
  virtual void nextCharacterSpecialized() = 0;
  virtual void restoreStateSpecialized(StreamState *state) = 0;
  virtual void closeSpecialized() {};
  char getCharacter() {
    if(isEOF())
      return EOF;
    else
      return getCharacterSpecialized();
  }
  void nextCharacter() {
    if(isEOF())
      return;
    if(getCharacter() == '\n')
      location.advanceNextLine();
    else
      location.advanceNextCharacter();
    index++;
    nextCharacterSpecialized();
  }
  StreamState *saveState() {
    if(closed)
      throw StreamException("Attempting to save a closed stream state.");
    return new StreamState(index, ended, location);
  }
  void restoreState(StreamState *state) {
    if(closed)
      throw StreamException("Attempting to restore a closed stream state.");
    index = state->index;
    ended = state->ended;
    location = state->location;
    restoreStateSpecialized(state);
  }
  Location currentLocation() { return location; }
};

class StringStream : public CharacterStream {
public:
  string line;
  StringStream(string filename, string line) :
    CharacterStream(filename), line(line) {}
  char getCharacterSpecialized() {
    return line[index];
  }
  void nextCharacterSpecialized() {
    if(index >= line.length())
      EOFState();
  }
  void restoreStateSpecialized(StreamState *state) {}
  void close() {}
};

class FileStream : public StringStream {
public:
  string filename;
  string content;
  FileStream(string filename) :
    StringStream(filename, readWholeFile(filename)) {}

  static string readWholeFile(string filename) {
    ifstream file(filename);
    if(!file.good())
      throw StreamException("Unable to open \"" + filename + "\".");
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
  }
};

class MultilineReplStream : public CharacterStream {
public:
  string prompt;
  string multilinePrompt;
  string content;
  string lastLine;
  MultilineReplStream(string prompt) :
    CharacterStream("<input>"), prompt(prompt) {
    nextCharacterSpecialized();
  }
  char getCharacterSpecialized() {
    return content[index];
  }
  void nextCharacterSpecialized() {
    if(index >= content.length()) {
      cout<<prompt;
      getline(cin, lastLine);
      if(lastLine == "") 
	EOFState();
      else
	content += lastLine + '\n';
    }
  }
  void restoreStateSpecialized(StreamState *state) {
    lastLine = "";
  }
  void close() {}
};
