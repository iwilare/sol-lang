-- Dijkstra, but

Infinite = SelfDescriptive new; name: "Infinite".
@ Infinite > a @ { True }.
@ Infinite < a @ { False }.
@ Infinite + a @ { Infinite }.
@ Infinite toString @ { "Inf" }.
--------------------------------------------------------------------------------
Node = Class new; name: "Node";
    variables: [#arcs, #name, #visited, #distance]; defineAccessors.
@ Node initialize @ { arcs = []. visited = False. distance = Infinite. }.
@ Node toString @ {
    "Node " + name + ", " + (visited ? "already" : "not") + " visited, distance: " +
                            (distance == -1 ? "infinite" : distance) +
                              " [ " + (arcs reduce: {x y | x + y next name + " "} with: "") + "]".
}.
---------------------------------------
@ Integer indent @ { {" " print} repeat: self }.
@ Node printMST @ { self printMST: 0 }.
@ Node printMST: i @ {
  [name, " (", distance, ")\n"] map: #print action.
  arcs filter: #min action do: {arc |
     i indent. "|" printLine.
     i indent. ["+--(", arc weight, ")--+"] map: #print action.
     arc next printMST: i + 8.
  }
}.
--------------------------------------------------------------------------------
Arc = Class new; name: "Arc";
    variables: [#next, #weight, #min]; defineAccessors.
@ Arc initialize @ { min = False }.
@ Arc toString @ { "-(" + weight + ")->" + next }.
--------------------------------------------------------------------------------
Graph = Class new; name: "Graph";
    variables: [#nodes]; defineAccessors.
@ Graph initialize @ { nodes = []. }.
@ Graph source: source @ {
    source distance: 0; visited: True
}.
@ Graph resetVisited @ { nodes do: {node | node visited: False}}.
@ Graph minOutgoingArc @ {
    nodeMin = nothing.
    arcMin = nothing.
    nodes filter: {x | x visited} do: {node |
        node arcs filter: {x | x next visited not} do: {arc |
            nodeMin == nothing => {nodeMin = node}.
            arcMin == nothing => {arcMin = arc}.
            (node distance + arc weight) <
              (nodeMin distance + arcMin weight) =>
                 {nodeMin = node. arcMin = arc}}}.
    arcMin != nothing => {arcMin min: True}.
    [nodeMin, arcMin].
}.
@ Graph allVisited @ { nodes all: #visited action}.
@ Graph dijkstraFrom: source @ {
    self resetVisited.
    self source: source.
    {pair = self minOutgoingArc.
     nodeMin = pair first.
     arcMin = pair second.
     (arcMin == nothing) ? False : {
        arcMin next visited: True;
                        distance: (nodeMin distance + arcMin weight).
        True}
    } whileTrue.
        source printMST
}.
--------------------------------------------------------------------------------

A = Node new; name: #A.
B = Node new; name: #B.
C = Node new; name: #C.
D = Node new; name: #D.
E = Node new; name: #E.
F = Node new; name: #F.

A arcs: [Arc new; next: B; weight: 3,
          Arc new; next: D; weight: 1,
          Arc new; next: C; weight: 5].
B arcs: [Arc new; next: C; weight: 4].
C arcs: [Arc new; next: E; weight: 1,
          Arc new; next: F; weight: 1].
D arcs: [Arc new; next: E; weight: 2].
E arcs: [Arc new; next: D; weight: 1,
          Arc new; next: F; weight: 2].
F arcs: [Arc new; next: C; weight: 1].

G = Graph new; nodes: [A, B, C, D, E, F].

-- G dijkstraFrom: A.
-- A printMST.
