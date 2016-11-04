Infinito = SelfDescriptive new; name: "Infinito".
@ Infinito > a @ { True }.
@ Infinito < a @ { False }.
@ Infinito + a @ { Infinito }.
@ Infinito toString @ { "Inf" }.
--------------------------------------------------------------------------------
Nodo = Class new; name: "Nodo";
    variables: [#nome, #visitato, #distanza, #precedente]; defineAccessors.
@ Nodo toString @ {
    "Nodo " + nome + ", " + (visitato ifFalse: "non" ifTrue: "già") + " visitato, distanza: " +
                            (distanza == -1 ifTrue: "infinito" ifFalse: distanza)
}.
--------------------------------------------------------------------------------
Arco = Class new; name: "Arco";
    variables: [#a, #b, #peso]; defineAccessors.
@ Arco toString @ { "(" + a + "->" + b + ", peso: " + peso + ")" }.
--------------------------------------------------------------------------------
Albero = Class new; name: "Albero";
    variables: [#nodi, #archi, #radice]; defineAccessors.
@ Albero initialize @ { archi = []. nodi = []. }.
@ Albero nodo: nome @ { nodi find: {x | x nome == nome} }.
@ Albero archiNodo: nodo @ { archi filter: {arco | arco a == nodo} }.
@ Albero print @ { self print: radice indenta: 0 }.
@ Integer indenta @ { {" " print} repeat: self }.
@ Albero print: nodo indenta: i @ {
  [nodo, " (", self nodo: nodo; distanza, ")\n"] map: #print action.
  self archiNodo: nodo; do: {arcoFiglio |
     i indenta. "|" printLine.
     i indenta. ["+--(", arcoFiglio peso, ")--+"] map: #print action.
     self print: arcoFiglio b indenta: i + 8.
  }
}.
--------------------------------------------------------------------------------
Grafo = Class new; name: "Grafo";
    variables: [#archi, #nodi].
@ Grafo initialize @ { archi = []. nodi = []. }.
@ Grafo sorgente: sorgente @ {
    self nodo: sorgente;
       distanza: 0;
       visitato: True
}.
@ Grafo addNodo: nodo @ { nodi push: nodo. self}.
@ Grafo addArco: arco @ { archi push: arco. self}.
@ Grafo nodo: nome @ { nodi find: {x | x nome == nome} }.
@ Grafo arcoDa: a a: b @ { archi find: {x | x a == a and: x b == b} }.
@ Grafo resetVisitati @ { nodi do: {nodo | nodo visitato: False}}.
@ Grafo cutVisitati @ {
    archi filter:
      {arco | (self nodo: arco a; visitato)
         and: (self nodo: arco b; visitato) not}
}.
@ Grafo arcoMinimo: archi @ {
    archi size == 0 ?
      {"Non riesco a calcolare l'arco minimo senza archi." error}.
    minimo = archi first.
    archi map: {arco |
       (self nodo: arco a; distanza; + arco peso) <
       (self nodo: minimo a; distanza; + minimo peso) ?
	   {minimo = arco} : nothing}.
    minimo.	
}.
@ Grafo allVisitati @ { nodi all: {x | x visitato}}.
@ Grafo dijkstraDa: sorgente @ {
    archiMinimi = [].
    self resetVisitati.
    self sorgente: sorgente.
    emptyCut = False.
    {self allVisitati; or: emptyCut} whileFalse: {
        cut = self cutVisitati.
	cut printLine.
	cut size == 0
          ifTrue: { emptyCut = True. }
	  ifFalse: {
             minimo = self arcoMinimo: cut.
	     self nodo: minimo b;
	        visitato: True;
		precedente: minimo a;
		distanza: (self nodo: minimo a; distanza + minimo peso).
	     archiMinimi push: minimo.
	     archiMinimi printLine.
          }
    }.
    Albero new nodi: nodi archi: archiMinimi radice: sorgente.
}.
@ Grafo toString @ { archi toString + '\n' + nodi toString }.
--------------------------------------------------------------------------------
-- A B C D E F
-- A B 3  A D 1  A C 5  D E 2  E D 1  E F 2  E C 1  C F 1  F C 2  end

G = Grafo new.
[#A, #B, #C, #D, #E, #F] map: {x | G addNodo: (Nodo new nome: x)}.
G addArco: (Arco new a: #A b: #B peso: 3).
G addArco: (Arco new a: #A b: #D peso: 1).
G addArco: (Arco new a: #A b: #C peso: 5).
G addArco: (Arco new a: #D b: #E peso: 2).
G addArco: (Arco new a: #E b: #D peso: 1).
G addArco: (Arco new a: #E b: #F peso: 2).
G addArco: (Arco new a: #C b: #E peso: 1).
G addArco: (Arco new a: #C b: #F peso: 1).
G addArco: (Arco new a: #F b: #C peso: 1).
G addArco: (Arco new a: #B b: #C peso: 4).


A = G dijkstraDa: #A
