class CommandLineError : public SolException {
public:
  CommandLineError(string message) :
    SolException("CommandLine", message) {}
};

void printGreeting() {
  cout<<"                 Sol"<<endl;
  cout<<"              Version "<<Version<<endl;
  cout<<"       Copyright (C) 2016 Iwilare"<<endl;
  cout<<endl;
}  

bool startsWith(string s, string start) { return s. substr(0,start.size()) == start; }
class SolState {
public:
  bool REPL = false;

  bool REPLPrint = true;
  bool SolBase = true;

  vector<string> Files;
  Environment GlobalEnvironment = EnvironmentCreate();
  
  bool Multiline = false;
  bool Skip = false;
  SolState(int argn, char *args[]) {
    for(int i=1; i<argn; i++)
      parseOption(args[i]);
    logicallyImply();
  }
  void parseOption(char *arg) {
    string argument(arg);
    if(argument[0] != '-')
      Files.push_back(argument);
    else
      if(argument == "-h") {
	printGreeting();
	cout<<"------ Command line options--------"<<endl;
	cout<<"   <filename>  Evaluate the file"<<endl;
	cout<<"   -h          Display this help"<<endl;
	cout<<"--- Activate/deactivate options ---"<<endl;
	cout<<"   -i              REPL      "
	  " (default: "<<(REPL?"true":"false")<<")"<<endl;
	cout<<"---------- Debug options-----------"<<endl;
	cout<<"   -no-sol         Sol.sol base file evaluation      "<<endl;
	cout<<"   -no-repl-print  Disable REPL result printing      "<<endl;
	cout<<"   -d<option>      Debug option                      "<<endl;
	exit(0);
      }
      else if(argument == "-i")
	REPL = true;
      else if(argument == "-no-sol")
	SolBase = false;
      else if(argument == "-no-repl-print")
	REPLPrint = false;
      else if(startsWith(argument, "-d")){
	string streamName = argument.substr(2);
	LogStream::switchLog(streamName);
      } else
	throw CommandLineError("Unrecognized option \"" + argument + "\".");
  }
  void logicallyImply() {
    if(Files.size() == 0)
      REPL = true;
  }
};

Sol completeEvalSource(SolState &state, CharacterStream *stream) {
  Tokenizer *tokenizer = new Tokenizer(stream);
  if(LogREPL.isActive()) {
    LogREPL<<"----- Tokenizer"<<LogEnd;
    TokenizerState *state = tokenizer->saveState();
    while(tokenizer->hasTokens()) {
      LogREPL<<tokenizer->getToken().toString()<<LogEnd;
      tokenizer->nextToken();
    }
    tokenizer->restoreState(state);
  }
  LogREPL<<"----- Parser"<<LogEnd;
  Parser parser(tokenizer);
  Atom *atom = parser.parse();
  LogREPL<<atom->toString()<<LogEnd;
  LogREPL<<"----- Result"<<LogEnd;
  return eval(atom, state.GlobalEnvironment);
}

void interpretCommand(SolState &state, string command) {
  state.Multiline = false;
  state.Skip = true;
  if(command == "") {
    state.Multiline = true;
    state.Skip = false;
  }
  // Check
  if(startsWith(command, ":")) {
    command = command.substr(1);
    if(startsWith(command, "e"))
      try {
	if(command.size() < 2)
	  throw RuntimeException("Expected space after file evaluation command.");
      	completeEvalSource(state, new FileStream(command.substr(2)));
      } catch(SolException e) {
	cout<<e.what()<<endl;
      }
    else if(startsWith(command, "q"))
      exit(0);
    else if(startsWith(command, "v"))
      printGreeting();
    else if(startsWith(command, "d")) {
      if(command.size() < 2)
	throw RuntimeException("Invalid Log switch command format.");
      string streamName = command.substr(2);
      LogStream::switchLog(streamName);
    }
    else if(startsWith(command, "h") or startsWith(command, "?")) {
      cout<<"--- REPL commands ---"<<endl;
      cout<<"   :e <filename>    Evaluate the file"<<endl;
      cout<<"   :d <option>      Debug mode switch"<<endl;
      cout<<"   :q               Exit the program"<<endl;
      cout<<"   :v               Display version"<<endl;
      cout<<"   :h or :?         Display this help"<<endl;
      cout<<"   <empty line>     Enable multiline mode"<<endl;
    } else
      throw RuntimeException("Unrecognized REPL command.");
  } else
    state.Skip = false;
}

int main(int argn, char *args[]) {
  SolState state(argn, args);
  
  LogREPL<<"Initializing classes..."<<LogEnd;
  initializeClasses();
  LogREPL<<"Starting Class reification..."<<LogEnd;
  classReification();

  LogREPL<<"Initializing global environment..."<<LogEnd;
  initializeGlobalEnvironment(state.GlobalEnvironment);
  
  if(state.SolBase)
    try {
      LogREPL<<"Evaluating Sol.sol..."<<LogEnd;
      completeEvalSource(state, new FileStream("Sol.sol"));      
    } catch(SolException e) {
      GlobalEvaluationStackCount = 0;
      cout<<e.what()<<endl;
    }
  LogREPL<<"Initialization complete."<<LogEnd;
  
  for(string file : state.Files)
    try {
      completeEvalSource(state, new FileStream(file));
    } catch(SolException e) {
      GlobalEvaluationStackCount = 0;
      cout<<e.what()<<endl;
    }
 
 if(state.REPL) {
    printGreeting();
    while(true)
      try {
	string s;
	cout<<"> "; getline(cin,s);
	interpretCommand(state, s);
	if(state.Skip)
	  continue;
	CharacterStream *stream;
	if(state.Multiline)
	  stream = new MultilineReplStream("| ");
	else
	  stream = new StringStream("<input>", s);
	Sol result = completeEvalSource(state, stream);
	LogREPL<<"Sending toString..."<<LogEnd;
	if(result != nullptr)
	  if(state.REPLPrint)
	    result->send("toString")->send("printLine");
      } catch(SolException e) {
	GlobalEvaluationStackCount = 0;
	cout<<e.what()<<endl;
      }
  }
  return 0;
}
