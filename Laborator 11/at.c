#include "main.h"


// adauga in tabela de simboluri o functie care are un argument 
// argumentul are tipul tipArg, iar functia are tipul returnat tipRet
Simbol *adaugaFn1Arg(const char *numeFn,int tipArg,int tipRet){
    Simbol *fn=adaugaSimbol(numeFn,FEL_FN);
    fn->tip=tipRet;
    fn->args=NULL;
    Simbol *arg=adaugaArgFn(fn,"arg");
    arg->tip=tipArg;
    return fn;
    }

void adaugaFnPredefinite(){
    adaugaFn1Arg("puti",TYPE_INT,TYPE_INT);
    adaugaFn1Arg("putr",TYPE_REAL,TYPE_REAL);
    adaugaFn1Arg("puts",TYPE_STR,TYPE_STR);
    }

void setRet(int tip,bool lval){
    ret.tip=tip;
    ret.lval=lval;
    }
