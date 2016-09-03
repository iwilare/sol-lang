Flags = -std=c++11 -static -g 
Compiler = g++ $(Flags)
WindowsCompiler = x86_64-w64-mingw32-g++ $(Flags)
Dependencies = Location.cpp \
	CharacterStream.cpp \
	Token.cpp \
	Tokenizer.cpp \
	Atom.cpp \
	Parser.cpp \
	Collectable.cpp \
	Environment.cpp \
	Classes.cpp \
	RuntimeExceptions.cpp \
	LambdaStructure.cpp \
	Evaluator.cpp \
	Runtime.cpp \
	Semantics.cpp \
	Program.cpp \
	Sol.cpp
linux : $(Dependencies)
	$(Compiler) Sol.cpp -o Sol.elf
windows : $(Dependencies)
	$(WindowsCompiler) Sol.cpp -o Sol.exe
