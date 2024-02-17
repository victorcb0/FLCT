#pragma once

#include <stdbool.h>

typedef struct{
	int tip;		// TYPE_*
	bool lval;	// daca este left-value (necesar pentru analiza de tipuri)
	}Ret;

extern Ret ret;	// var folosita pentru a stoca date returnate de unele dintre regulile sintactice

enum{FEL_VAR,FEL_ARG,FEL_FN};

struct Simbol;typedef struct Simbol Simbol;
struct Simbol{
	const char *nume;		// referinta la numele stocat intr-un atom
	int fel;		// FEL_*
	int tip;	// TYPE_*
	union{
		Simbol *args;	// pentru functii: lista cu argumentele functiei
		bool local;		// pentru variabile: daca este locala
		};
	Simbol *urm;		// urmator - pentru inlantuire in lista
	};

struct Domeniu;typedef struct Domeniu Domeniu;
struct Domeniu{
	Domeniu *parinte;		// domeniul parinte
	Simbol *simboluri;		// lista simplu inlantuita de simboluri
	};

extern Domeniu *tabelaSim;	// tabela de simboluri (stiva de domenii)

// pointer la simbolul functiei curente, daca se parseaza o functie
// sau NULL in afara functiilor
extern Simbol *crtFn;

Domeniu *adaugaDomeniu();		// adauga un nou domeniu in TS ca fiind domeniul curent
void stergeDomeniu();	// sterge domeniul curent din TS si revine in cel parinte
Simbol *cautaInDomeniulCurent(const char *nume);		// cauta doar in domeniul curent
Simbol *cautaSimbol(const char *nume);		// cauta in toate domeniile
Simbol *adaugaSimbol(const char *nume,int fel);	// adauga un simbol la domeniul curent
Simbol *adaugaArgFn(Simbol *fn,const char *argName);	// adauga un argument la simbolul fn



