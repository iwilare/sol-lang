@ Nothing toString @ { "nothing" }.
@ Object toString @ {
  "<" + self class toString + " at " + self id + ">"
}.

@ Integer count @ {
   self == 0 ifTrue: 0 ifFalse: {(self-1) count}
}.

SelfDescriptive = 
    Class new 
        superclass: Class; 
        name: "SelfDescriptive".
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
@ True  ifFalse: false @ { }.
@ False ifTrue: true ifFalse: false @ { false value }.
@ False ifFalse: false ifTrue: true @ { false value }.
@ False ifFalse: false @ { false value }.
@ False ifTrue: true @ { }.

Number = Class new.
Integer superclass: Number.
Double superclass: Number.

@ Number < b @ { self >= b; not }.
@ Number > b @ { self <= b; not }.
@ Number >= b @ { self > b; or: self == b }.
@ Number <= b @ { self < b; or: self == b }.

@ Lambda whileTrue: block @ {
  self value ifTrue: { block value. self whileTrue: block } ifFalse: self
}.
@ Lambda repeat @ {
  self value.
  self repeat.
}.

@ Integer callerino @ {
  "HELLOWROLD" print.
  self > 3 ifTrue: {^ 10}.
  "Case 2..." print.
  self == 2 ifTrue: {^ 3}.
  "Case 4..." print.
  90.
}.

@ Integer fact @ {
  self == 0 
    ifTrue: {1}
    ifFalse: {
       self * (self - 1) fact }}.
