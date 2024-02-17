#pragma once

#include <stddef.h>

// implementare simpla a unui buffer dinamic in care se scriu caractere
// pe masura ce se scriu caractere, bufferul creste
typedef struct{
	char *buf;		// buffer
	size_t n;		// nr de caractere din buf
	}Text;

// analog lui printf, dar caracterele sunt scrise in bufferul "text", nu pe ecran
void Text_write(Text *text,const char *fmt,...);

//sterge caracterele dintr-un buffer
void Text_clear(Text *text);

extern Text tInceput	// fisier antet si variabile globale
	,tMain		// in functia main din C se va genera codul global din Quick
	,tFunctii	// functiile din Quick
	,tAntetFn	// folosit temporar pentru generarea antetului unei functii Quick
	;

// pointeri care pointeaza la diverse buffere
// in fuctie de domeniul curent (intr-o functie sau global)
extern Text *crtCod;		// intr-o fn pointeaza la tFunctii, altfel la tMain
extern Text *crtVar;		// intr-o fn pointeaza la tFunctii, altfel la tInceput

// returneaza numele din C corespunzator lui type din Quick (ex: TYPE_REAL -> double)
// type = TYPE_*
const char *cType(int type);
