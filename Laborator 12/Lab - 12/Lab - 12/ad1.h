#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ad.h"

Ret ret;
Domeniu* tabelaSim;
Simbol* crtFn;

void* alocare(size_t nOcteti) {
	void* p = malloc(nOcteti);
	if (p == NULL) {
		puts("memorie insuficienta");
		exit(EXIT_FAILURE);
	}
	return p;
}

Domeniu* adaugaDomeniu() {
	puts("creaza domeniu nou");
	Domeniu* d = (Domeniu*)alocare(sizeof(Domeniu));
	d->parinte = tabelaSim;
	d->simboluri = NULL;
	tabelaSim = d;
	return d;
}

void stergeSimboluri(Simbol* lista);

void stergeSimbol(Simbol* s) {
	printf("\tsterge simbolul %s\n", s->nume);
	if (s->fel == FEL_FN) {
		stergeSimboluri(s->args);
	}
	free(s);
}

void stergeSimboluri(Simbol* lista) {
	for (Simbol* s1 = lista, *s2; s1; s1 = s2) {
		s2 = s1->urm;
		stergeSimbol(s1);
	}
}

void stergeDomeniu() {
	puts("stergere domeniu curent");
	Domeniu* parinte = tabelaSim->parinte;
	stergeSimboluri(tabelaSim->simboluri);
	free(tabelaSim);
	tabelaSim = parinte;
	puts("revine in domeniul parinte");
}

Simbol* cautaInLista(Simbol* lista, const char* nume) {
	for (Simbol* s = lista; s; s = s->urm) {
		if (!strcmp(s->nume, nume))return s;
	}
	return NULL;
}

Simbol* cautaInDomeniulCurent(const char* nume) {
	return cautaInLista(tabelaSim->simboluri, nume);
}

Simbol* cautaSimbol(const char* nume) {
	for (Domeniu* d = tabelaSim; d; d = d->parinte) {
		Simbol* s = cautaInLista(d->simboluri, nume);
		if (s)return s;
	}
	return NULL;
}

Simbol* creeazaSimbol(const char* nume, int fel) {
	Simbol* s = (Simbol*)alocare(sizeof(Simbol));
	s->nume = nume;
	s->fel = fel;
	return s;
}

Simbol* adaugaSimbol(const char* nume, int fel) {
	printf("\tadauga simbolul %s\n", nume);
	Simbol* s = creeazaSimbol(nume, fel);
	s->urm = tabelaSim->simboluri;
	tabelaSim->simboluri = s;
	return s;
}

Simbol* adaugaArgFn(Simbol* fn, const char* argName) {
	printf("\tadauga simbolul %s ca argument\n", argName);
	Simbol* s = creeazaSimbol(argName, FEL_ARG);
	s->urm = NULL;
	if (fn->args) {
		Simbol* p;
		for (p = fn->args; p->urm; p = p->urm) {}
		p->urm = s;
	}
	else {
		fn->args = s;
	}
	return s;
}