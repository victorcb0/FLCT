#pragma once

#include <stdbool.h>

// adauga in TS functiile predefinite din exemplu: puti, putr, puts.
// altfel, la intalnirea acestor identificatori s-ar fi emis mesaj de eroare
// ca ei sunt necunoscuti.
void adaugaFnPredefinite();

// seteaza variabila globala "ret" cu tipul rezultat dintr-o regula
void setRet(int tip,bool lval);