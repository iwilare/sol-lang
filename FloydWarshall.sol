Infinito = SelfDescriptive new; name: "Infinito".
@ Infinito > a @ { True }.
@ Infinito < a @ { False }.
@ Infinito + a @ { Infinito }.
@ Infinito toString @ { "Inf" }.
--------------------------------------------------------------------------------
TabellaDistanze = Class new; name: "TabellaDistanze";
    instanceVariables: [#verso]; defineAccessors.
@ TabellaDistanze initialize @ { verso = SymbolDictionary new }.
@ TabellaDistanze verso: B passandoPer: C set: v @ {
    (verso has: B; not or: {verso at: B; == nothing}) ifTrue:
       { verso at: B set: SymbolDictionary new }.
    verso at: B; at: C set: v.
}.
@ TabellaDistanze verso: B passandoPer: C @ {
    verso at: B; at: C
}.
@ TabellaDistanze doesNotUnderstand: m with: w @ {
    verso send: m with: w.
}.
--------------------------------------------------------------------------------
Nodo = Class new; name: "Nodo";
    instanceVariables: [#tabella, #nome]; defineAccessors; metaclassify.
@ Nodo class new: nome @ {
    self new; nome: nome 
}.
@ Nodo initialize @ {
    tabella = TabellaDistanze new.
}.
@ Nodo toString @ {
    "Nodo " + nome + '\n'  +
    "Tabella delle distanze\n" +
       (tabella map:
          {B bDist | "" + B + " ->\n" +
	       (bDist map: {C peso |
	      "    " + C + " peso: " + peso + '\n'};
	        reduce: #+:)}; reduce: #+:)
}.
@ Nodo aggiorna: tabellaRoutingEsterna da: nodoEsterno @ {
    tabellaRoutingEsterna do: {nodo peso |
       tabella verso: nodo passandoPer: nodoEsterno set:
           [tabella verso: nodo passandoPer: nodoEsterno,
	    tabella verso: nodo passandoPer: (self nome) + peso] minimum
    }
}.
@ Nodo tabellaRouting @ {
    tabellaRouting = SymbolDictionary new.
    tabella do: { verso links |
       links do: { passandoPer peso |
          passandoPer == self nome ?
	    {tabellaRouting at: verso set: peso}}}.
    tabellaRouting.
}.
@ Nodo collega: nodo con: peso @ {
    tabella verso: nodo passandoPer: self nome set: peso.
}.
--------------------------------------------------------------------------------
A = Nodo new: #A.
A collega: #B con: 10.
