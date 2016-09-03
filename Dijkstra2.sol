Infinito = SelfDescriptive new; name: "Infinito".
@ Infinito > a @ { True }.
@ Infinito < a @ { False }.
@ Infinito + a @ { Infinito }.
@ Infinito toString @ { "Inf" }.
--------------------------------------------------------------------------------
Nodo = Class new; name: "Nodo";
    variables: [#archi, #nome, #visitato, #distanza]; defineAccessors.
@ Nodo initialize @ { archi = []. visitato = False. distanza = Infinito. }.
@ Nodo toString @ {
    "Nodo " + nome + ", " + (visitato ? "già" : "non") + " visitato, distanza: " +
                            (distanza == -1 ? "infinito" : distanza) +
			      " [ " + (archi reduce: {x y | x + y next nome + " "} with: "") + "]".
}.
---------------------------------------
@ Integer indenta @ { {" " print} repeat: self }.
@ Nodo printAlberoMinimo @ { self printAlberoMinimo: 0 }.
@ Nodo printAlberoMinimo: i @ {
  [nome, " (", distanza, ")\n"] map: #print action.
  archi filter: #minimo action do: {arco |
     i indenta. "|" printLine.
     i indenta. ["+--(", arco peso, ")--+"] map: #print action.
     arco next printAlberoMinimo: i + 8.
  }
}.
--------------------------------------------------------------------------------
Arco = Class new; name: "Arco";
    variables: [#next, #peso, #minimo]; defineAccessors.
@ Arco initialize @ { minimo = False }.
@ Arco toString @ { "-(" + peso + ")->" + next }.
--------------------------------------------------------------------------------
Grafo = Class new; name: "Grafo";
    variables: [#nodi]; defineAccessors.
@ Grafo initialize @ { nodi = []. }.
@ Grafo sorgente: sorgente @ {
    sorgente distanza: 0; visitato: True
}.
@ Grafo resetVisitati @ { nodi do: {nodo | nodo visitato: False}}.
@ Grafo arcoUscenteMinimo @ {
    nodoMinimo = nothing.
    arcoMinimo = nothing.
    nodi filter: {x | x visitato} do: {nodo |
        nodo archi filter: {x | x next visitato not} do: {arco |
            nodoMinimo == nothing => {nodoMinimo = nodo}.	 
            arcoMinimo == nothing => {arcoMinimo = arco}.
            (nodo distanza + arco peso) <
	      (nodoMinimo distanza + arcoMinimo peso) => 
                 {nodoMinimo = nodo. arcoMinimo = arco}}}.
    arcoMinimo != nothing => {arcoMinimo minimo: True}.
    [nodoMinimo, arcoMinimo].
}.
@ Grafo allVisitati @ { nodi all: #visitato action}.
@ Grafo dijkstraDa: sorgente @ {
    self resetVisitati.
    self sorgente: sorgente.
    {pair = self arcoUscenteMinimo.
     nodoMinimo = pair first.
     arcoMinimo = pair second.
     (arcoMinimo == nothing) ? False : {
        arcoMinimo next visitato: True;
	                distanza: (nodoMinimo distanza + arcoMinimo peso).
        True}
    } whileTrue.
	sorgente printAlberoMinimo
}.
--------------------------------------------------------------------------------

A = Nodo new; nome: #A.
B = Nodo new; nome: #B.
C = Nodo new; nome: #C.
D = Nodo new; nome: #D.
E = Nodo new; nome: #E.
F = Nodo new; nome: #F.

A archi: [Arco new; next: B; peso: 3,
          Arco new; next: D; peso: 1,
	  Arco new; next: C; peso: 5].
B archi: [Arco new; next: C; peso: 4].
C archi: [Arco new; next: E; peso: 1,
	  Arco new; next: F; peso: 1].
D archi: [Arco new; next: E; peso: 2].
E archi: [Arco new; next: D; peso: 1,
	  Arco new; next: F; peso: 2].
F archi: [Arco new; next: C; peso: 1].

G = Grafo new; nodi: [A, B, C, D, E, F].

-- G dijkstraDa: A.
-- A printAlberoMinimo.