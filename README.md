//Urse Andrei 312CB

BATTLESHIPS

PASUL I: Citirea si plasarea navelor

    Am definit un struct "ship" care retine datele unei nave (tip, lungime,
coordonatele originii si al capatului, cat si orientarea) pentru a le accesa
ulterior mai rapid in faza de atac.
    Pentru acest pas am alocat dinamic trei matrici (una in care memorez lungimea navelor,
una in care memorez indexul din vectorul de nave pentru a fi mai rapida accesarea datelor
navei in timpul atacurilor, si inca una in care voi memora mai tarziu atacurile efectuate)
si doi vectori in care retin fiecare nava cat si datele acesteia, respecitv numarul curent
de nave al fiecarui jucator.
    In final, se afiseaza tablele jucatorilor, adica matricile care memoreaza lungimea navelor.

PASUL II: Efectuarea atacurilor

    In faza de atacuri, verific ca celula lovita de jucator sa fie goala, astfel pentru un atac
reusit, in matricile cu lungimea, respectiv indexul, celula nimerita trebuie sa fie diferita de zero,
iar in matricea pentru atac sa fie chiar zero.
    In acest timp, pentru statisticile de la taskul 3, memorez numarul total de nave, de atacuri, cat
si atacurile reusite.
    Pe parcurs ce se distrug nave, vectorul care retine numarul de nave al fiecarui jucator se
decrementeaza, pana cand una dintre valori devine zero, jucatorul cu valoarea >0 castigand.
    Dupa finalizarea fiecarui meci, se elibereaza memoria alocata celor cinci tablouri mentionate.

PASUL III: Statistici

    Dupa finalizarea tuturor jocurilor, calculez statisticile cerute din mesajul citit.
    Aloc dinamic doua siruri de caractere, unul va fi cheia, iar celalalt mesajul.
    Extrag in variabile diferite informatiile din mesaj.
    In total ar fi 8 cazuri diferite de calcul, asa ca am construit o functie ce calculeaza
statisticile pentru un singur joc, si alta pentru toate jocurile. 
    Am folosit functii din biblioteca math.h pentru a ajunge exact la rezultatele dorite.
    Pentru statisticile unui joc am calculat fie numarul de atacuri pe care le-ar fi trebuit
sa le nimereasca jucatorul pentru a atinge acuratetea dorita, fie numarul de lovituri pe care le-ar
putut rata, dar totusi acuratetea jucatorului sa ramana peste cea dorita.
    Pentru statisticile tuturor jocurilor am folosit metoda Greedy. Pentru cazul I, am ales jocurile
care aduc cel mai mare castig, iar pentru cazul III, am ales jocurile care aduc cea mai mare pierdere.