class Context {
public:
  enum Type { lambdaT, builtinT } type;
  Address nextAddress;
  //union {
  Environment *environment;
  struct {
    Sol *self;
    vector<Sol*> arguments;
  } builtin;
  Context(Address nextAddress, Environment *environment) :
    type(lambdaT), nextAddress(nextAddress), environment(environment) {}
  Context(Address nextAddress, Sol *self, vector<Sol*> arguments) :
    type(builtinT), nextAddress(nextAddress), builtin({self, arguments}) {}
  string toString() {
    switch(type) {
    case lambdaT:
      return "Context | Lambda  -> " + nextAddress.toString();
    case builtinT:
      return "Context | Builtin -> " + nextAddress.toString();      
    }
  }
};

class VirtualMachineException : public SolException {
  public:
  VirtualMachineException(VirtualMachine *vm);
  VirtualMachineException(VirtualMachine *vm, string message);
};

class VirtualMachine {
public:
  Code *code;
  Address nextAddress;
  bool virtualMachineCalmer = false;
  list<Sol*> stack;
  list<Context> contextStack;

  Context context;
  VirtualMachine(Code *code, Environment *environment) :
    code(code), context(Address(false), environment) {}
  void start() {
    while(true) {
      cout<<"@@@@@@@@@@@@@@@@"<<endl;
      cout<<"EXECUTING: "<<nextAddress.toString()<<endl;
      cout<<"CONTEXT: "<<context.toString()<<endl;
      cout<<"WHOLE CONTEXT"<<endl;
      for(auto c : contextStack)
	cout<<"\t"<<c.toString()<<endl;
      cout<<"WHOLE STACK"<<endl;
      for(auto c : stack)
	cout<<"\t"<<c<<endl;
      
      
      switch(nextAddress.type) {
      case Address::Type::Virtual: {
	if(code->outOfBounds(nextAddress.codeAddress))
	  return;
	Instruction *instruction = code->atAddress(nextAddress.codeAddress);
	nextAddress.codeAddress = code->nextAddress(nextAddress.codeAddress);
	interpretInstruction(instruction);
	break;
      }
      case Address::Type::Builtin: {
	if(context.type != Context::Type::Builtin)
	  cerr<<"Fatal error in context."<<endl;
	StandardSolFunction method = nextAddress.function;
	Sol *result = method(this, context.builtin.self,
			     context.builtin.arguments);
	if(virtualMachineCalmer) {
	  virtualMachineCalmer = false;
	  continue;
	}
	push(result);
	contextPop();
	break;
      }
      case Address::Type::Internal:
	return;
      }
    }
  }
  void contextPush(Context context) {
    contextStack.push_back(this->context);
    this->context = context;
  }
  void contextPop() {
    nextAddress = this->context.nextAddress;
    this->context = contextStack.back();
    contextStack.pop_back();
  }
  Sol *peek() { return stack.back(); }
  Sol *pop() {
    //if(stack.size() == 0)
    //  throw new VirtualMachineException(this);
    Sol *value = stack.back();
    stack.pop_back();
    return value;
  }
  void push(Sol *value) { stack.push_back(value); }
  void interpretInstruction(Instruction *instruction) {
    switch(instruction->type) {
    case Instruction::Type::Message:
      message(instruction->message); break;
    case Instruction::Type::Push:
      push(context.environment->getVariable(instruction->pushVariable)); break;
    case Instruction::Type::Set:
      context.environment->setVariable(instruction->setVariable, peek());
    case Instruction::Type::Define: 
      context.environment->defineVariable(instruction->defineVariable, peek()); break;
    case Instruction::Type::PushSelf:
      push(context.environment->getSelf()); break;
    case Instruction::Type::SuperMessage:
      superMessage(instruction->message); break;
    case Instruction::Type::Pop:
      pop(); break;
    case Instruction::Type::Return:
      contextPop();
      break;
    case Instruction::Type::PushLambda:
      push(new Sol(Lambda, new LambdaStructure(nextAddress.codeAddress,
					       instruction->lambda.parameters,
					       context.environment)));
      nextAddress.codeAddress = nextAddress.codeAddress.add(instruction->lambda.skipSize);
      break;
    case Instruction::Type::PushVector: {
      Sol *result = new Sol(Vector);
      vector<Sol*> actualVector;
      for(int i=0; i<instruction->vectorCollectionSize; i++)
	actualVector.push_back(pop());
      result->data = new vector<Sol*>(actualVector);
      push(result);
      break;
    }
    case Instruction::Type::PushSymbol:
      push(new Sol(Symbol, new string(instruction->symbol)));
      break;
    case Instruction::Type::PushInteger:
      push(new Sol(Integer, new int(instruction->integerValue)));
      break;
    case Instruction::Type::PushDouble:
      push(new Sol(Double, new double(instruction->doubleValue)));
      break;
    case Instruction::Type::PushCharacter:
      push(new Sol(Character, new char(instruction->characterValue)));
      break;
    case Instruction::Type::PushString:
      push(new Sol(String, new string(instruction->stringValue)));
      break;
    case Instruction::Type::DeleteStackFrame:
      Address next = nextAddress;
      contextPop();
      nextAddress = next;
      break;
    }
  }
  inline Sol *send(Sol *self, string message_) { return send(self, Message(message_), {}); }
  Sol *send(Sol *self, Message message_, vector<Sol*> arguments) {
    if(message_.parametersNumber != arguments.size())
      throw RuntimeException("Trying to send message \"" + message_.toString() +
			     "\" with wrong number of arguments.");
    push(self);
    for(Sol *argument : arguments)
      push(argument);
    nextAddress = Address(false);
    message(message_);
    start();
    return pop();
  }
  inline Sol *superSend(Sol *self, string message_) { return superSend(self, Message(message_), {}); }
  Sol *superSend(Sol *self, Message message_, vector<Sol*> arguments) {
    if(message_.parametersNumber != arguments.size())
      throw RuntimeException("Trying to send message \"" + message_.toString() +
			     "\" with wrong number of arguments.");
    push(self);
    for(Sol *argument : arguments)
      push(argument);
    //nextAddress = Address(false);
    superMessage(message_);
    start();
    return pop();
  }
  inline void message(Message message_)      { return message(message_, false); }
  inline void superMessage(Message message_) { return message(message_, true); }
  void message(Message message_, bool superMessage) {
    vector<Sol*> arguments;
    Sol *self;
    for(int i=0; i<message_.parametersNumber; i++)
      arguments.push_back(pop());
    self = pop();
    Sol *startingClass;
    if(self == nullptr)
      startingClass = Null;
    else
      startingClass = self->klass;
    if(superMessage)
      startingClass = startingClass->getVariable("superclass");
    return message(message_, self, arguments, startingClass);
  }
  void message(Message message_, Sol *self, vector<Sol*> arguments, Sol *currentClass) {
    while(currentClass != nullptr) {
      Sol *methodDictionary = currentClass->getVariable("methodDictionary");
      typeCheck(methodDictionary, SymbolDictionary);       
      map<string, Sol*> methodMap = *(map<string, Sol*>*)methodDictionary->data;
      if(methodMap.count(message_.toString()) == 1) {
	Sol *methodObject = methodMap[message_.toString()];
	if(methodObject->klass == BuiltinMethod) {
	  StandardSolFunction method = *(StandardSolFunction*)methodObject->data;
	  evalBuiltinMethod(method, self, arguments);
	} else if(methodObject->klass == Lambda) {
	  LambdaStructure *lambda = (LambdaStructure*)methodObject->data;    
	  evalLambda(lambda, self, arguments);
	} else {
	  Sol *argumentsObject = new Sol(Vector, new vector<Sol*>(arguments));
	  push(send(self, Message("valueIn:with:"), {self, argumentsObject}));
	}
	return;
      }
      currentClass = currentClass->getVariable("superclass");
    }
    Sol *messageObject = UTILITY_createSymbol(message_.toString());
    push(send(self, Message("doesNotUnderstand:"), {messageObject}));
  }
  void evalBuiltinMethod(StandardSolFunction method, Sol *self, vector<Sol*> arguments) {
    evalBuiltinMethod(method, self, arguments, true);
  }
  void evalBuiltinMethod(StandardSolFunction method, Sol *self, vector<Sol*> arguments, bool returning) {
    if(returning)
      contextPush(Context(nextAddress, self, arguments));
    nextAddress = Address(method);	 
  }
  void evalLambda(LambdaStructure *lambda, Sol *self, vector<Sol*> arguments) {
    evalLambda(lambda, self, arguments, true);
  }
  void evalLambda(LambdaStructure *lambda, Sol *self, vector<Sol*> arguments, bool returning) {
    Environment *environment = new Environment(lambda->environment, self);
    if(arguments.size() != lambda->parameters.size())
      throw RuntimeException("Invalid number of arguments.");
    for(int i=0; i<arguments.size(); i++)
      environment->defineVariable(lambda->parameters[i], arguments[i]);
    if(returning)
      contextPush(Context(nextAddress, environment));
    nextAddress = Address(lambda->address);
  }	  
};


string VirtualMachineExceptionString(VirtualMachine *vm) {
  stringstream ss;
  ss<<"@@@@ VirtualMachine exception @@@@"<<endl;
  ss<<"--- Stack ---"<<endl;
  for(Sol *v : vm->stack)
    ss<<"\t"<<v<<endl;
  ss<<"--- Current context ---"<<endl;
  ss<<vm->context.toString()<<endl;
  ss<<"--- Context stack ---";
  for(Context a : vm->contextStack)
    ss<<endl<<"\t"<<a.toString();
  return ss.str();
}
VirtualMachineException::VirtualMachineException(VirtualMachine *vm) :
  SolException("VirtualMachine", VirtualMachineExceptionString(vm)) {}
VirtualMachineException::VirtualMachineException(VirtualMachine *vm, string message) :
  SolException("VirtualMachine", message + "\n" + VirtualMachineExceptionString(vm)) {}

//    @ Object toString @ { "<" + self class toString + " at " + self id + ">" }.
