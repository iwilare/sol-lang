# Sol - Simple Object Language

A Smalltalk-inspired language implemented in Java, with metaclasses, dictionary-based reflection, closures, tail recursion, with threads, regexps, and sockets bindings.

## Getting started

Simply clone the repo and run

```sh
javac src/*.java -d out/
```

then run the interpreter and start the REPL with

```sh
java -cp out/ Sol [-no-init] [file1.sol] [file2.sol]
```

By default the interpreter tries to search in the current folder for a `Sol.sol` file, which is ran as initialization. Disable this behaviour with the `-no-init` flag.

## Examples

Wish to learn by examples? Check out the default standard library in [Sol.sol](Sol.sol), or the [examples/](examples/) folder.

# Overview

- Objects can receive *unary*, *binary*, and *keyword* messages. Expressions are parsed by giving priority to unary, then binary, then keyword messages.

```
> 3 next
4
> 3 * 4 next
13
> (True or: False) not
True
```

- Binary operators *always associate* to the left, and start with anything that is not an ASCII letter.

```
> 3 + 4 * 5
35
```

- Mutable assignments and declarations are given by `=`.

```smalltalk
v = [].
i = 0.
```

- Closures are delimited with curly brackets `{ ... }`, and are evaluated with the `value` unary message. Closures capture the enclosing environment (even future variables that are currently undefined in that environment)

```c
> name = "Mark"
> printer = {("Oh, hi " + name) printLine}
> printer value.
Oh, hi Mark
"Oh, hi Mark"
```

- One-argument closures can be given a parameter with `{a | ...}`, and receive the keyword message `value: v` with the argument.

```haskell
> inc = {a | a + 1}
> inc value: 30
31
```

- Multiple parameters are separated by spaces, to evaluate the closure use the message `values: [a, b]` with the arguments passed in a Vector.

```haskell
> sump = {a b | a + b * a} -- Remember the precedence!
> sump values: [10, 2]
40
```
- Sequences of statements is separated by "`.`".
```haskell
n = 3.
"Nice!" print.
3 + n print.
```
- Comments are Haskell style with `--` as line comments and `{-` and `-}` for block comments.
```haskell
-- This line will be ignored.
"So long," print.
{-
    This is also a comment.
    Another commented line.
-}
"and thanks for all the fish!" print
```

- Vector literals with `[a,b,c].`

```haskell
> a = [3,7,1]
> a size
3
> a + [5,11]
[3,7,1,5,11]
```

- Message literals:

```
> #at:do:
#at:do:
> #+
#+
> #next class
Message
```

- Methods can be chained with `;`. This is also useful with the "return `self`" pattern.

```haskell
> 3 + 1; * 2
8
> [1,2,3]; map: {x | x + 1}; filter: {x | x < 3}; length
1
```

- The ternary operator exists, but it is simply mimicked with the `TruePromise` and `FalsePromise` classes and the `?` and `:` binary messages. Lambdas are evaluated.

```c
> True ? 1 : "nope"
1
> True ? 1; class
TruePromise
> False ? {"Hello" printLine} : {"Meow" printLine}
Meow
"Meow"
```

- New methods can be declared using the following syntactic sugars. Using `self` inside a method refers to the object receiving the message.

```
@ Vector all: predicate from: i @ {
  (i >= self size) ? True : {
    (predicate value: (self at: i)) not
      ? False
      : { self all: predicate from: i next }
  }
}.
@ Vector + vector @ {
  sum = [].
  self   do: {x | sum push: x}.
  vector do: {x | sum push: x}.
  sum.
}.
```

- Use the `class` and `methods` messages on Objects and Classes to see which methods each object can receive.
- And most importantly, *explore and have fun!*

## Files description

### Frontend

#### Reading

- [Location.java](src/Location.java): file, line, position later attached to tokens.
- [CharacterStream.java](src/CharacterStream.java): abstractions for reading and localizing characters in a stream, either a multiline string, a single line, or a file.

#### Lexing

- [Token.java](src/Token.java): lexer tokens.
- [Tokenizer.java](src/Tokenizer.java): lexer reading from CharacterStream sources, outputting tokens.

#### Parsing

- [Atom.java](src/Atom.java): abstract syntax tree nodes.
- [Parser.java](src/Parser.java): reading from a stream of tokens, parsing entire files.

### Backend

- [Message.java](src/Message.java): message objects.
- [Lambda.java](src/Lambda.java): closures and their tail-recursive evaluation.
- [Environment.java](src/Environment.java): linked hash-mapped environments with link, define, has, set, get.
- [EvaluationRequest.java](src/EvaluationRequest.java): special exception raised to implement tail-recursion within Java methods.
- [Evaluator.java](src/Evaluator.java): classic recursive evaluation of expressions, within a `self` object and a current `contextClass` being referenced.
- [Sol.java](src/Sol.java):
    - runtime *Sol objects*, containing:
        - a class,
        - an hash map of instance variables,
        - an optional internal `Object` pointer.
    - the Sol message cycle logic, where messages are looked up in the object class.
    - arguments reading and REPL.

### Runtime

- [Classes.java](src/Classes.java): list of all builtin Sol class objects used at runtime.
- [Methods.java](src/Methods.java): global initializer of *all* methods for built-in classes.

### Examples

- [DijkstraGraph.sol](examples/DijkstraGraph.sol): Dijkstra's algorithm, with a centralized Graph objects maintaining all information.
- [DijkstraNodes.sol](examples/DijkstraNodes.sol): Dijkstra's algorithm, with each node keeping track of (and querying) their neighbours.
- [Server.sol](examples/Server.sol): basic HTTP server, served with Java sockets and parsing requests with Java regular expressions.

## Prerequisites

- JDK
