#include "main.h"

Text tInceput,tMain,tFunctii,tAntetFn;
Text *crtCod;
Text *crtVar;

void Text_write(Text *text,const char *fmt,...){
	va_list va;	
	va_start(va,fmt);	// "va" contine parametrii optionali, de dupa "fmt"
	// vsnprintf apelat cu buffer NULL sau nr max de caractere 0
	// returneaza nr total de caractere, fara \0, care se vor emite
	// daca exista un buffer corespunzator
	int n=vsnprintf(NULL,0,fmt,va);
	// realoca bufferul dinamic pentru a se putea adauga noile caractere
	char *p=(char*)realloc(text->buf,(text->n+n+1)*sizeof(char));
	if(p==NULL){
		puts("memorie insuficienta");
		exit(EXIT_FAILURE);
		}
	// adauga noile caractere la bufferul dinamic
	vsnprintf(p+text->n,n+1,fmt,va);
	text->buf=p;
	text->n+=n;
	va_end(va);
	}

void Text_clear(Text *text){
	free(text->buf);
	text->buf=NULL;
	text->n=0;
	}

const char *cType(int type){
	switch(type){
		case TYPE_INT:return "int";
		case TYPE_REAL:return "double";
		case TYPE_STR:return "str";
		default:
			printf("tip eronat: %d\n",type);
			exit(EXIT_FAILURE);
		}
	}
