Infinite = SelfDescriptive new; name: "Infinite".
@ Infinite > a @ { True }.
@ Infinite < a @ { False }.
@ Infinite + a @ { Infinite }.
@ Infinite toString @ { "Inf" }.
--------------------------------------------------------------------------------
Node = Class new; name: "Node";
    variables: [#name, #visited, #distance, #previous]; defineAccessors.
@ Node toString @ {
    "Node " + name + ", " + (visited ifFalse: "non" ifTrue: "already") + " visited, distance: " +
                            (distance == -1 ifTrue: "infinite" ifFalse: distance)
}.
--------------------------------------------------------------------------------
Arc = Class new; name: "Arc";
    variables: [#a, #b, #weight]; defineAccessors.
@ Arc toString @ { "(" + a + "->" + b + ", weight: " + weight + ")" }.
--------------------------------------------------------------------------------
Tree = Class new; name: "Tree";
    variables: [#nodes, #arcs, #root]; defineAccessors.
@ Tree initialize @ { arcs = []. nodes = []. }.
@ Tree node: name @ { nodes find: {x | x name == name} }.
@ Tree arcs: node @ { arcs filter: {arc | arc a == node} }.
@ Tree print @ { self print: root indent: 0 }.
@ Integer indent @ { {" " print} repeat: self }.
@ Tree print: node indent: i @ {
  [node, " (", self node: node; distance, ")\n"] map: #print action.
  self arcs: node; do: {child |
     i indent. "|" printLine.
     i indent. ["+--(", child weight, ")--+"] map: #print action.
     self print: child b indent: i + 8.
  }
}.
@ Tree toString @ { self print. super toString }.
--------------------------------------------------------------------------------
Graph = Class new; name: "Graph";
    variables: [#arcs, #nodes].
@ Graph initialize @ { arcs = []. nodes = []. }.
@ Graph source: source @ {
    self node: source;
       distance: 0;
       visited: True
}.
@ Graph addNodo: node @ { nodes push: node. self}.
@ Graph addArc: arc @ { arcs push: arc. self}.
@ Graph node: name @ { nodes find: {x | x name == name} }.
@ Graph arcFrom: a a: b @ { arcs find: {x | x a == a and: x b == b} }.
@ Graph resetVisited @ { nodes do: {node | node visited: False}}.
@ Graph cutVisited @ {
    arcs filter:
      {arc | (self node: arc a; visited)
         and: (self node: arc b; visited) not}
}.
@ Graph minArc: arcs @ {
    arcs size == 0 ?
      {"Cannot compute min arc without arcs." error}.
    min = arcs first.
    arcs map: {arc |
       (self node: arc a; distance; + arc weight) <
       (self node: min a; distance; + min weight) ?
           {min = arc} : nothing}.
    min.
}.
@ Graph allVisited @ { nodes all: {x | x visited}}.
@ Graph dijkstraFrom: source @ {
    minArcs = [].
    self resetVisited.
    self source: source.
    emptyCut = False.
    {self allVisited; or: emptyCut} whileFalse: {
        cut = self cutVisited.
        cut size == 0
          ifTrue: { emptyCut = True. }
          ifFalse: {
             min = self minArc: cut.
             self node: min b;
                  visited: True;
                  previous: min a;
                  distance: (self node: min a; distance + min weight).
             minArcs push: min.
          }
    }.
    Tree new nodes: nodes arcs: minArcs root: source.
}.
@ Graph toString @ { arcs toString + '\n' + nodes toString }.
--------------------------------------------------------------------------------
-- A B C D E F
-- A B 3  A D 1  A C 5  D E 2  E D 1  E F 2  E C 1  C F 1  F C 2  end

G = Graph new.
[#A, #B, #C, #D, #E, #F] map: {x | G addNodo: (Node new name: x)}.
G addArc: (Arc new a: #A b: #B weight: 3).
G addArc: (Arc new a: #A b: #D weight: 1).
G addArc: (Arc new a: #A b: #C weight: 5).
G addArc: (Arc new a: #D b: #E weight: 2).
G addArc: (Arc new a: #E b: #D weight: 1).
G addArc: (Arc new a: #E b: #F weight: 2).
G addArc: (Arc new a: #C b: #E weight: 1).
G addArc: (Arc new a: #C b: #F weight: 1).
G addArc: (Arc new a: #F b: #C weight: 1).
G addArc: (Arc new a: #B b: #C weight: 4).

A = G dijkstraFrom: #A
