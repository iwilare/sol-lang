class Compiler {
private:
  Bytecode code;
public:
  Compiler() {}
  int instructionCount() { return code.size(); }
  Bytecode &getCode() { return code; }
  int compile(Atom *a) {
    int size = code.size();
    switch(a->type) {
    case Atom::Type::MessageT:
      if(a->message.receiver->type == Atom::Type::Identifier and
	 a->message.receiver->identifier == "super") {
	for(Atom *argument : a->message.arguments)
	  compile(argument);
	code.serialize(Instruction(a->message.message, true));
      } else {
	compile(a->message.receiver);
	for(Atom *argument : a->message.arguments)
	  compile(argument);
	code.serialize(Instruction(a->message.message));  
      }
      break;
    case Atom::Type::Identifier:
      if(a->identifier == "self")
	code.serialize(Instruction(Instruction::Type::PushSelf));
      else if(a->identifier == "null")
	code.serialize(Instruction(Instruction::Type::PushNull));
      else
	code.serialize(Instruction(Instruction::Type::Push, a->identifier));
      break;
    case Atom::Type::Assignment:
      compile(a->assignment.value);
      code.serialize(Instruction(Instruction::Type::Set, a->assignment.variable));
      break;
    case Atom::Type::Sequence: {
      if(a->sequence.size() == 0) { 
	code.serialize(Instruction(Instruction::Type::PushNull));
	break;
      }
      int i; for(i=0; i<a->sequence.size()-1; i++) {
	compile(a->sequence[i]);
	code.serialize(Instruction(Instruction::Type::Pop));
      }
      compile(a->sequence[i]);
      break;
    }
    case Atom::Type::Lambda: {
      int lambdaStart = code.serialize(Instruction(0, a->lambda.parameters));
      int bodySize = 0;
      if(a->lambda.body != nullptr)
	bodySize = compile(a->lambda.body);
      code.serialize(Instruction(Instruction::Type::Return));
      swap(code.getIndex(), lambdaStart);
      code.serialize(Instruction(bodySize, a->lambda.parameters));
      swap(code.getIndex(), lambdaStart);
      /*std::Reverse(a->lambda.parameters.begin(),
	a->lambda.parameters.end());*/
      break;
    }
    case Atom::Type::Vector: {
      int size = a->vectorElements.size();
      for(Atom *element : a->vectorElements)
	compile(element);
      code.serialize(Instruction(Instruction::Type::PushVector, size));
      break;
    }
    case Atom::Type::Symbol:
      code.serialize(Instruction(Instruction::Type::PushSymbol,    a->symbol));
      break;
    case Atom::Type::Integer:
      code.serialize(Instruction(Instruction::Type::PushInteger,   a->integerValue));
      break;
    case Atom::Type::Double:
      code.serialize(Instruction(Instruction::Type::PushDouble,    a->doubleValue));
      break;
    case Atom::Type::Character:
      code.serialize(Instruction(Instruction::Type::PushCharacter, a->characterValue));
      break;
    case Atom::Type::String:
      code.serialize(Instruction(Instruction::Type::PushString,    a->stringValue));
      break;
    default:
      cerr<<"Fatal error, unrecognized Atom."<<endl;
    }
    return code.size() - size;
  }
};
