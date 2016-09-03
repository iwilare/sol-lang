class CommandLineError : public SolException {
public:
  CommandLineError(string message) :
    SolException("CommandLine", message) {}
};

struct SolState {
  bool Debug = true;
  bool REPL = false;
  bool SolBase = true;
  bool InitializeEnvironment = true;
  vector<string> files;
  Environment *GlobalEnvironment = new Environment();
  
  bool Multiline = false;
  bool Skip = false;
};
void printGreeting() {
  cout<<"                 Sol"<<endl;
  cout<<"              Version "<<VERSION<<endl;
  cout<<"       Copyright (C) 2016 Iwilare"<<endl;
  cout<<endl;
}  
void parseCharacterOption(SolState &state, char c) {
  switch(c) {
  case 'h':
    printGreeting();
    cout<<"--- Command line options ---"<<endl;
    cout<<"   <filename>  Evaluate the file"<<endl;
    cout<<"   -h          Display this help"<<endl;
    cout<<"--- Activate/deactivate options ---"<<endl;
    cout<<"   -i          REPL                              "
      " (default: "<<(state.REPL?"true":"false")<<")"<<endl;
    cout<<"   -b          Sol.sol base file evaluation      "
      " (default: "<<(state.SolBase?"true":"false")<<")"<<endl;
    cout<<"   -d          Debug mode                        "
      " (default: "<<(state.Debug?"true":"false")<<")"<<endl;
    cout<<"   -e          Global environment initialization "
      " (default: "<<(state.Debug?"true":"false")<<")"<<endl;
    exit(0);
    break;
  case 'd':
    state.Debug = !state.Debug;
    break;
  case 'i':
    state.REPL = !state.REPL;
    break;
  case 'b':
    state.SolBase = !state.SolBase;
    break;
  case 'e':
    state.InitializeEnvironment = !state.InitializeEnvironment;
    break;
  default:
    throw CommandLineError("Unrecognized option \"-" + std::string(1,c) + "\".");
  }
}
void parseOption(SolState &state, char *arg) {
  string argument(arg);
  if(argument[0] == '-')
    for(int i=1; i<argument.length(); i++)
      parseCharacterOption(state, argument[i]);
  else
    state.files.push_back(argument);
}
void logicallyImplyState(SolState &state) {
  if(state.files.size() == 0)
    state.REPL = true;
}

Sol *completeEvalSource(SolState state, CharacterStream *stream) {
  Tokenizer *tokenizer = new Tokenizer(stream);
  if(state.Debug) {
    cout<<"--- Tokenizer --------------------------------------"<<endl;
    TokenizerState *state = tokenizer->saveState();
    while(tokenizer->hasTokens()) {
      cout<<tokenizer->getToken().toString()<<endl;
      tokenizer->nextToken();
    }
    tokenizer->restoreState(state);
  }
  if(state.Debug)
    cout<<"--- Parser -----------------------------------------"<<endl;
  Parser parser(tokenizer);
  Atom *atom = parser.parse();
  if(state.Debug)
    cout<<atom->toString()<<endl;
  if(state.Debug)
    cout<<"--- Result -----------------------------------------"<<endl;
  try {
    return eval(0, atom, state.GlobalEnvironment);
  } catch(SolContinuation c) {
    return c.returnValue;
  }
}

bool startsWith(string s, char c) { return s[0] == c; }
void interpretCommand(SolState &state, string command) {
  state.Multiline = false;
  state.Skip = true;
  if(command == "") {
    state.Multiline = true;
    state.Skip = false;
  }
  // Check
  if(startsWith(command, ':')) {
    command = command.substr(1);
    if(startsWith(command, 'e'))
      try {
	// Check
	completeEvalSource(state, new FileStream(command.substr(2)));
      } catch(SolException e) {
	cout<<e.what()<<endl;
      }
    else if(startsWith(command, 'q'))
      exit(0);
    else if(startsWith(command, 'v'))
      printGreeting();
    else if(startsWith(command, 'd'))
      state.Debug = !state.Debug;
    else if(startsWith(command, 'h') or startsWith(command, '?')) {
      cout<<"--- REPL commands ---"<<endl;
      cout<<"   :e <filename>    Evaluate the file"<<endl;
      cout<<"   :q               Exit the program"<<endl;
      cout<<"   :d               Debug mode"<<endl;
      cout<<"   :v               Display version"<<endl;
      cout<<"   :h or :?         Display this help"<<endl;
      cout<<"   <empty line>     Enable multiline mode"<<endl;
    } else
      throw RuntimeException("Unrecognized REPL command.");
  } else
    state.Skip = false;
}

int main(int argn, char *args[]) {
  cout<<"BEFORE CREATION OF TIME ITSELF:"<<endl;
  SolState state;
  state.GlobalEnvironment->reference();
  
  for(int i=1; i<argn; i++)
    parseOption(state, args[i]);
  logicallyImplyState(state);
  cout<<"BEFORE CREATION OF TIME ITSELF:"<<endl;
  
  if(state.Debug) cout<<"--- Initializing classes... ------------------------"<<endl;
  initializeClasses();
  if(state.Debug) cout<<"--- Starting Class reification... ------------------"<<endl;
  classReification();

  if(state.InitializeEnvironment) {
    if(state.Debug)
      cout<<"--- Initializing global environment... -------------"<<endl;
    initializeGlobalEnvironment(state.GlobalEnvironment);
  }
  if(state.SolBase)
    try {
      if(state.Debug)
	cout<<"--- Evaluation Sol.sol... --------------------------"<<endl;
      completeEvalSource(state, new FileStream("Sol.sol"));      
    } catch(SolException e) {
      cout<<e.what()<<endl;
    }
  if(state.Debug) cout<<"--- Initialization complete. -----------------------"<<endl;
  
  vector<string> files;
  for(string file : files)
    try {
      completeEvalSource(state, new FileStream(file));
    } catch(SolException e) {
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
	Sol *result = completeEvalSource(state, stream);
	if(state.Debug)
	  cout<<"--- Sending toString... ----------------------------"<<endl;
	result->send("toString")->send("printLine")->finalize();
      } catch(SolException e) {
	cout<<e.what()<<endl;
      }
  }
  return 0;
}
