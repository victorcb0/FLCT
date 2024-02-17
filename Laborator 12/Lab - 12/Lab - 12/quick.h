// acest antet cuprinde biblioteca de functii predefinite din Quick
// el nu se va include in codul sursa al compilatorului

#include <stdio.h>

// defineste tipul de date pentru TYPE_STR
typedef char *str;

// static pentru o fn o face sa fie locala intr-un fisier de cod si sa poata exista 
// cu acelasi nume in multiple fisiere de cod
// s-a folosit "static" pentru ca "quick.h" sa poata fi inclus in fisiere multiple de cod
// fara sa apara eroare de redefinire simbol "puti"
static void puti(int val){
	printf("%d\n",val);
	}

static void putr(double val){
	printf("%g\n",val);
	}

// "puts" este deja declarat in C
