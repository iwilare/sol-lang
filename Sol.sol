@ Class defineAccessors @ {
    instanceVariables do: {variable |
        self methods at: variable set: (variable asGetMethod).
        self methods at: (variable + #:) set: (variable asSetMethod).
    }.
    constructorSymbol = instanceVariables reduce: {x y | x + y + #:} with: Symbol new.
    self methods at: constructorSymbol set: (constructorSymbol asSetMethod).
    self
}.
@ Class subclass @ {
  Class new; superclass: self
}.
@ Nothing toString @ { "nothing" }.
@ Nothing value @ { nothing }.
@ Object toString @ {
    "<" + self class + " at " + self id + ">"
}.

SelfDescriptive = Class subclass; name: "SelfDescriptive".
@ SelfDescriptive initialize @ {
    self superclass: SelfDescriptive.
    self class: self.
}.

True = SelfDescriptive new; name: "True".
False = SelfDescriptive new; name: "False".

@ True  and: b  @ { b }.
@ True  or:  b  @ { True }.
@ True  not @ { False }.
@ False and: b  @ { False }.
@ False or:  b  @ { b }.
@ False not @ { True }.

@ True  ifTrue: true ifFalse: false @ { true value }.
@ True  ifFalse: false ifTrue: true @ { true value }.
@ True  ifTrue: true @ { true value }.
@ True  => true @ { true value }.
@ True  ifFalse: false @ { }.

@ False ifTrue: true ifFalse: false @ { false value }.
@ False ifFalse: false ifTrue: true @ { false value }.
@ False ifFalse: false @ { false value }.
@ False ifTrue: true @ { }.
@ False => true @ { }.

TruePromise = Class new; name: "TruePromise"; instanceVariables: [#promise].
@ TruePromise promise: promise @ { promise = promise }.
FalsePromise = SelfDescriptive new; name: "FalsePromise"; instanceVariables: [].

@ True  ? promise @ { TruePromise new promise: promise. }.
@ False ? promise @ { FalsePromise }.
@ TruePromise : false @ { promise value }.
@ FalsePromise : false @ { false value }.

Number = Class new; name: "Number".
Integer superclass: Number.
Double superclass: Number.

@ Number < b @ { self >= b; not }.
@ Number > b @ { self <= b; not }.
@ Number >= b @ { self > b; or: self == b }.
@ Number <= b @ { self < b; or: self == b }.

@ Lambda whileTrue: block @ {
  self value ifTrue: { block value. self whileTrue: block } ifFalse: nothing
}.
@ Lambda whileFalse: block @ {
  {self value not} whileTrue: block
}.
@ Lambda whileTrue @ {
  self value
     ifTrue: {self whileTrue}
     ifFalse: nothing.
}.
@ Lambda whileFalse @ {
  self value
     ifFalse: {self whileFalse}
     ifTrue: nothing.
}.

@ Symbol action @ { {x | x send: self} }.
@ Symbol section @ { {x y | x send: self with: [y]} }.

@ Lambda repeat @ {
  self value.
  self repeat.
}.
@ Lambda repeat: times @ {
  times == 0 ? nothing : {self value. self repeat: times-1}.
}.
@ Integer fact @ {
  self == 0 
    ifTrue: {1}
    ifFalse: {self * (self - 1) fact}
}.
@ Integer to: end do: function @ {
  self <= end ? {function value: self. (self+1) to: end do: function} : {}
}.
@ Integer timesDo: function @ {
  self == 0 ? {} : {function value. self-1 timesDo: function}
}.

@ Vector + vector @ {
  sum = [].
  self   do: {x | sum push: x}.
  vector do: {x | sum push: x}.
  sum.
}.
@ Vector first @ { self at: 0 }.
@ Vector second @ { self at: 1 }.
@ Vector map: function @ {
  vector = [].
  i = 0.
  {i < self size} whileTrue: {
     vector push: (function value: (self at: i)).
     i = i next.
  }.
  vector.
}.
@ Vector do: function @ {
  i = 0.
  {i < self size} whileTrue: {function value: (self at: i). i = i next}.
}.
@ Vector from: start do: function @ {
  i = start.
  {i < self size} whileTrue: {function value: (self at: i). i = i next}.
}.
@ Vector filter: predicate @ {
  vector = [].
  i = 0.
  {i < self size} whileTrue: {
     predicate value: (self at: i);
        ifTrue: {vector push: (self at: i)}.
     i = i next.
  }.
  vector. 
}.
@ Vector filter: predicate do: action @ {
  i = 0.
  {i < self size} whileTrue: {
     predicate value: (self at: i);
        ifTrue: {action value: (self at: i)}.
     i = i next.
  }.
}.
@ Vector all: predicate @ {
  self all: predicate from: 0
}.
@ Vector all: predicate from: i @ {
  (i >= self size) ? True : {
     (predicate value: (self at: i)) not ?
        False : {self all: predicate from: i next}}
}.
@ Vector find: predicate @ {
  self find: predicate from: 0
}.
@ Vector find: predicate from: i @ {
  (i >= self size) ? nothing : {
        (predicate value: (self at: i)) ?
	   {self at: i} :
           {self find: predicate from: i next}}
}.
@ Vector reduce: function with: value @ {
  self do: {element |
    value = function values: [value, element].
  }.
  value.
}.
@ Vector reduce: function @ {
  value = self first.
  self from: 1 do: {element | 
    value = function values: [value, element].
  }.
  value.
}.


Metaclass = Class subclass; name: "Metaclass";
    instanceVariables: [#uniqueInstance]; defineAccessors.
@ Metaclass new @ {
    uniqueInstance == nothing 
        => {uniqueInstance = super new}.
    uniqueInstance.
}.
@ Class metaclassify @ {
    self class:
        (Metaclass new;
            superclass: self superclass class;
            uniqueInstance: self;
            name: self name + " class").
}.