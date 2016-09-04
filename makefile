Flags = -std=c++11
LinuxCompiler = g++
LinuxFlags = -g
WindowsCompiler = x86_64-w64-mingw32-g++
WindowsFlags = -static
Dependencies = Log.cpp \
	Location.cpp \
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
	SolStructure.cpp \
	Program.cpp \
	Sol.cpp
linux : $(Dependencies)
	$(LinuxCompiler) $(Flags) $(LinuxFlags) Sol.cpp -o Sol.elf
windows : $(Dependencies)
	$(WindowsCompiler) $(Flags) $(WindowsFlags) Sol.cpp -o Sol.exe
