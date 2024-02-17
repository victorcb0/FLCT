#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum {
	ID, INT, REAL, STR, VAR, FUNCTION, IF, ELSE, WHILE, END, RETURN,
	TYPE_INT, TYPE_REAL, TYPE_STR, COMMA, COLON, SEMICOLON, LPAR,
	RPAR, FINISH, ADD, SUB, MUL, DIV, AND, OR, NOT, EQUAL, ASSIGN, NOTEQ, LESS
};

char atomStrings[][31] = { "ID", "INT", "REAL", "STR", "VAR", "FUNCTION", "IF", "ELSE", "WHILE", "END", "RETURN",
	"TYPE_INT", "TYPE_REAL", "TYPE_STR", "COMMA", "COLON", "SEMICOLON", "LPAR",
	"RPAR", "FINISH", "ADD", "SUB", "MUL", "DIV", "AND", "OR", "NOT", "EQUAL", "ASSIGN", "NOTEQ", "LESS" };

typedef struct {
	int cod;
	int linie;
	union {
		char s[100];
		double r;
		int i;
	};
}Atom;
Atom atomi[10000];
int nAtomi = 0;

char bufin[30001];
char* pch;
int linie = 1;

void addAtom(int atom)
{
	atomi[nAtomi].cod = atom;
	atomi[nAtomi].linie = linie;
	nAtomi++;
}

void afisare()
{
	int linie = atomi[0].linie;
	printf("%d ", linie);
	for (int i = 0; i < nAtomi; i++)
	{
		if (!(atomi[i].linie == linie)) {
			linie = atomi[i].linie;
			printf("\n%d ", linie);
		}
		if (atomi[i].cod == ID || atomi[i].cod == STR)
		{
			printf("%s:%s ", atomStrings[atomi[i].cod], atomi[i].s);
		}
		else if (atomi[i].cod == INT)
		{
			printf("%s:%d ", atomStrings[atomi[i].cod], atomi[i].i);
		}
		else if (atomi[i].cod == REAL)
		{
			printf("%s:%f ", atomStrings[atomi[i].cod], atomi[i].r);
		}
		else
		{
			printf("%s ", atomStrings[atomi[i].cod]);
		}
	}
	printf("\n");
}

int getNextTk()
{
	int state = 0;
	char buf[100];
	int n = 0;
	for (;;) {
		char ch = *pch;
		//printf("#%d %c(%d)\n", state, ch, ch);
		switch (state) {
		case 0:
			if (isalpha(ch) || ch == '_') { state = 1; pch++; buf[n++] = ch; }
			else if (isdigit(ch)) { state = 3; pch++; buf[n++] = ch; }
			else if (ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t') {
				pch++;
				if (ch == '\n') linie++;
			}
			else if (ch == '#') { state = 31; pch++; }
			else if (ch == '"') { state = 8; pch++; buf[n++] = ch; }
			else if (ch == ',') { state = 10; pch++; }
			else if (ch == ':') { state = 11; pch++; }
			else if (ch == ';') { state = 12; pch++; }
			else if (ch == '(') { state = 13; pch++; }
			else if (ch == ')') { state = 14; pch++; }
			else if (ch == '\0') { state = 15; }
			else if (ch == '+') { state = 16; pch++; }
			else if (ch == '-') { state = 17; pch++; }
			else if (ch == '*') { state = 18; pch++; }
			else if (ch == '/') { state = 19; pch++; }
			else if (ch == '&') { state = 20; pch++; }
			else if (ch == '|') { state = 22; pch++; }
			else if (ch == '=') { state = 24; pch++; }
			else if (ch == '!') { state = 27; pch++; }
			else if (ch == '<') { state = 30; pch++; }
			else {
				printf("Caracter invalid\n");
				exit(-1);
			}
			break;
		case 1:
			if (isalnum(ch) || ch == '_') { pch++; buf[n++] = ch; }
			else { state = 2; }
			break;
		case 2:
			buf[n] = '\0';
			if (strcmp(buf, "var") == 0) addAtom(VAR);
			else if (strcmp(buf, "function") == 0) addAtom(FUNCTION);
			else if (strcmp(buf, "if") == 0) addAtom(IF);
			else if (strcmp(buf, "else") == 0) addAtom(ELSE);
			else if (strcmp(buf, "while") == 0) addAtom(WHILE);
			else if (strcmp(buf, "end") == 0) addAtom(END);
			else if (strcmp(buf, "return") == 0) addAtom(RETURN);
			else if (strcmp(buf, "int") == 0) addAtom(TYPE_INT);
			else if (strcmp(buf, "real") == 0) addAtom(TYPE_REAL);
			else if (strcmp(buf, "str") == 0) addAtom(TYPE_STR);
			else {
				strcpy(atomi[nAtomi].s, buf);
				addAtom(ID);
			}
			return ID;
		case 3:
			if (isdigit(ch)) { pch++; buf[n++] = ch; }
			else if (ch == '.') { state = 5; pch++; buf[n++] = ch; }
			else { state = 4; }
			break;
		case 4:
			buf[n] = '\0';
			atomi[nAtomi].i = atoi(buf);
			addAtom(INT);
			return INT;
		case 5:
			if (isdigit(ch)) { state = 6; pch++; buf[n++] = ch; }
			else {
				printf("Dupa punctul zecimal trebuie sa urmeze o cifra");
				exit(-1);
			}
			break;
		case 6:
			if (isdigit(ch)) { pch++; buf[n++] = ch; }
			else { state = 7; }
			break;
		case 7:
			buf[n] = '\0';
			atomi[nAtomi].r = atof(buf);
			addAtom(REAL);
			return REAL;
		case 8:
			if (!(ch == '"')) { pch++; buf[n++] = ch; }
			else if (ch == '"') { state = 9; pch++; buf[n++] = ch; }
			break;
		case 9:
			buf[n] = '\0';
			strcpy(atomi[nAtomi].s, buf);
			addAtom(STR);
			return STR;
		case 10:
			addAtom(COMMA);
			return COMMA;
		case 11:
			addAtom(COLON);
			return COLON;
		case 12:
			addAtom(SEMICOLON);
			return SEMICOLON;
		case 13:
			addAtom(LPAR);
			return LPAR;
		case 14:
			addAtom(RPAR);
			return RPAR;
		case 15:
			addAtom(FINISH);
			return FINISH;
		case 16:
			addAtom(ADD);
			return ADD;
		case 17:
			addAtom(SUB);
			return SUB;
		case 18:
			addAtom(MUL);
			return MUL;
		case 19:
			addAtom(DIV);
			return DIV;
		case 20:
			if (ch == '&') { state = 21; pch++; }
			break;
		case 21:
			addAtom(AND);
			return AND;
		case 22:
			if (ch == '|') { state = 24; pch++; }
			break;
		case 23:
			addAtom(OR);
			return OR;
		case 24:
			if (ch == '=') { state = 25; pch++; }
			else { state = 26; }
			break;
		case 25:
			addAtom(EQUAL);
			return EQUAL;
		case 26:
			addAtom(ASSIGN);
			return ASSIGN;
		case 27:
			if (ch == '=') { state = 28; pch++; }
			else { state = 29; }
			break;
		case 28:
			addAtom(NOTEQ);
			return NOTEQ;
		case 29:
			addAtom(NOT);
			return NOT;
		case 30:
			addAtom(LESS);
			return LESS;
		case 31:
			if (ch == ' ') { state = 32; pch++; }
			break;
		case 32:
			if (!(ch == '\n' || ch == '\r' || ch == '\0')) {
				state = 32;
				if (ch == '\n') linie++;
				pch++;
			}
			else { state = 0; }
			break;
		default:
			printf("stare invalida %d\n", state);
		}
	}
}