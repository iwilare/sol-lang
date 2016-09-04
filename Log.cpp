class LogException : public SolException {
public:
  LogException(string message) :
    SolException("Log", message) {}
};

class LogStream;
vector<LogStream*> GlobalLogStreams;
class LogEndT {} LogEnd;
class LogStream {
private:
  string name;
  bool active;
  bool ended;
public:
  LogStream(string name, bool active) :
    name(name), active(active), ended(true) {
    GlobalLogStreams.push_back(this);
  }
  bool isActive() { return active; }
  template<class T> LogStream &operator<<(T value) {
    if(!active) return *this;
    if(ended) {
      printStreamLabel(name);
      ended = false;
    }
    cerr<<value;
    return *this;
  }
  void operator<<(LogEndT end) {
    if(!active) return;
    ended = true;
    cerr<<endl;
  }
  static void printStreamLabel(string name) {
    int maxSpaces = 0;
    for(LogStream *logStream : GlobalLogStreams)
      if(logStream->isActive() and logStream->name.length() > maxSpaces)
	maxSpaces = logStream->name.length();
    for(int i=0; i<maxSpaces-name.size(); i++, cerr<<" ");
    cerr<<name;
    cerr<<" | ";
  }
  static void switchLog(string name) {
    for(LogStream *logStream : GlobalLogStreams)
      if(logStream->name == name) {
	logStream->active = !logStream->active;
	return;
      }
    throw LogException("Could not find \"" + name + "\" Log.");
  }
};

LogStream LogGC("GC", false);
LogStream LogEvaluation("Evaluation", false);
LogStream LogMessages("Messages", false);
LogStream LogRuntime("Runtime", false);
LogStream LogREPL("REPL", false);
