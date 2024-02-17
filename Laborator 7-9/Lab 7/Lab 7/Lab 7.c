#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <Lab 5.h>

int idxCrtAtom = 0;	

int consume(int cod) {
	if (atomi[idxCrtAtom].cod == cod) {
		idxCrtAtom++;
		return true;
	}
	return false;
}

void err(const char* msg);

int program();
int defVar();
int baseType();
int defFunc();
int block();
int funcParams();
int funcParam();
int instr();
int expr();
int exprLogic();
int exprAssign();
int exprComp();
int exprAdd();
int exprMul();
int exprPrefix();
int factor();

// program ::= ( defVar | defFunc | block )* FINISH
int program() {
	int startIdx = idxCrtAtom;
	for (;;) {
		if (defVar()) {}
		else if (defFunc()) {}
		else if (block()) {}
		else break;
	}
	if (consume(FINISH)) { return 1; }
	idxCrtAtom = startIdx;
	return 0;
}

// defVar ::= VAR ID COLON baseType SEMICOLON   
int defVar() {
	int startIdx = idxCrtAtom;
	if (consume(VAR)) {
		if (consume(ID)) {
			if (consume(COLON)) {
				if (baseType()) {
					if (consume(SEMICOLON)) { return 1; }
					else err("lipseste ; la finalul declaratiei de variabila");
				} else err("tip invalid de variabila");
			} else err("lipseste : dupa numele variabilei");
		} else err("lipseste numele variabilei");
	}
	idxCrtAtom = startIdx;
	return 0;
}

// baseType ::= TYPE_INT | TYPE_REAL | TYPE_STR
int baseType() {
	int startIdx = idxCrtAtom;
	if (consume(TYPE_INT)) { return 1; }
	else if (consume(TYPE_REAL)) { return 1; }
	else if (consume(TYPE_STR)) { return 1; } 
	//else err("tip invalid");
	idxCrtAtom = startIdx;
	return 0;
}

// defFunc ::= FUNCTION ID LPAR funcParams RPAR COLON baseType defVar* block END
int defFunc() {
	int startIdx = idxCrtAtom;
	if (consume(FUNCTION)) {
		if (consume(ID)) {
			if (consume(LPAR)) {
				if (funcParams()) {
					if (consume(RPAR)) {
						if (consume(COLON)) {
							if (baseType()) {
								for (;;) {
									if (!(defVar())) break;
								}
								if (block()) {
									if (consume(END)) { return 1; }
									else err("lipseste end dupa continutul functiei");
								} else err("continut invalid de functie");
							} else err("lipseste tipul functiei");
						} else err("lipseste : dupa )");
					} else err("lipseste ) dupa parametrii functiei");
				} else err("parametri invalizi de functie");
			} else err("lipseste ( dupa numele functiei");
		} else err("lipseste numele functiei");
	}
	idxCrtAtom = startIdx;
	return 0;
}

// block ::= instr+
int block() {
	int startIdx = idxCrtAtom;
	if (instr()) {
		for (;;) {
			if (!(instr())) break;
		}
		return 1;
	}
	idxCrtAtom = startIdx;
	return 0;
}

// funcParams ::= ( funcParam ( COMMA funcParam )* )?
int funcParams() {
	int startIdx = idxCrtAtom;
	if (funcParam()) {
		for (;;) {
			if (consume(COMMA)) {
				if (funcParam()) {}
				else {
					idxCrtAtom = startIdx;
					err("lipseste parametru de functie dupa ,");				
					return 0;
				}
			} else break;
		}
	}
	return 1;
}

// funcParam ::= ID COLON baseType
int funcParam() {
	int startIdx = idxCrtAtom;
	if (consume(ID)) {
		if (consume(COLON)) {
			if (baseType()) { return 1; }
			else err("lipseste tipul parametrului de functie dupa :");
		} else err("lipseste : dupa numele parametrului de functie");
	}
	idxCrtAtom = startIdx;
	return 0;
}

// instr ::= expr? SEMICOLON
//			| IF LPAR expr RPAR block (ELSE block)? END
//			| RETURN expr SEMICOLON
//			| WHILE LPAR expr RPAR block END
int instr() {
	int startIdx = idxCrtAtom;
	expr();
	if (consume(SEMICOLON)) { return 1; }
	idxCrtAtom = startIdx;
	if (consume(IF)) {
		if (consume(LPAR)) {
			if (expr()) {
				if (consume(RPAR)) {
					if (block()) {
						if (consume(ELSE)) {
							if (!(block())) err("continut invalid de dupa else");
						}
						if (consume(END)) { return 1; }
						else err("lipseste end la sfarsit de if");
					} else err("continut invalid dupa )");
				} else err("lipseste ) dupa expresie");
			} else err("expresie invalida dupa (");
		} else err("lipseste ( dupa if");
	}
	idxCrtAtom = startIdx;
	if (consume(RETURN)) {
		if (expr()) {
			if (consume(SEMICOLON)) { return 1; }
			else err("lipseste ; dupa expresie");
		}
		else err("lipseste expresia dupa return");
	}
	idxCrtAtom = startIdx;
	if (consume(WHILE)) {
		if (consume(LPAR)) {
			if (expr()) {
				if (consume(RPAR)) {
					if (block()) {
						if (consume(END)) { return 1; }
						else err("lipseste end la sfarsit de while");
					} else err("continut invalid dupa )");
				} else err("lipseste ) dupa expresie");
			} else err("expresie invalida dupa (");
		} else err("lipseste ( dupa while");
	}
	idxCrtAtom = startIdx;
	return 0;
}

// expr ::= exprLogic
int expr() {
	int startIdx = idxCrtAtom;
	if (exprLogic()) { return 1; }
	idxCrtAtom = startIdx;
	return 0;
}

// exprLogic ::= exprAssign ( (AND | OR) exprAssign )*
int exprLogic() {
	int startIdx = idxCrtAtom;
	if (exprAssign()) {
		for (;;) {
			if (consume(AND)) {
				if (!(exprAssign())) err("expresie invalida dupa &&");
			} else if (consume(OR)) {
				if (!(exprAssign())) err("expresie invalida dupa ||");
			} else break;
		}
		return 1;
	}
	idxCrtAtom = startIdx;
	return 0;
}

// exprAssign ::= (ID ASSIGN)? exprComp
int exprAssign() {
	int startIdx = idxCrtAtom;
	if (consume(ID)) {
		if (!(consume(ASSIGN))) 
			idxCrtAtom = startIdx;
	}
	if (exprComp()) { return 1; }
	idxCrtAtom = startIdx;
	return 0;
}

// exprComp ::= exprAdd ( (LESS | EQUAL) exprAdd )?
int exprComp() {
	int startIdx = idxCrtAtom;
	if (exprAdd()) {
		if (consume(LESS)) {
			if (!(exprAdd())) err("expresie invalida dupa <");
		} else if (consume(EQUAL)) {
			if (!(exprAdd())) err("expresie invalida dupa ==");
		}
		return 1;
	}
	idxCrtAtom = startIdx;
	return 0;
}

// exprAdd ::= exprMul ( (ADD | SUB) exprMul)*
int exprAdd() {
	int startIdx = idxCrtAtom;
	if (exprMul()) {
		for (;;) {
			if (consume(ADD)) {
				if (!(exprMul())) err("expresie invalida dupa +");
			} else if (consume(SUB)) {
				if (!(exprMul())) err("expresie invalida dupa -");
			} else break;
		}
		return 1;
	}
	idxCrtAtom = startIdx;
	return 0;
}

// exprMul ::= exprPrefix ( (MUL | DIV) exprPrefix )*
int exprMul() {
	int startIdx = idxCrtAtom;
	if (exprPrefix()) {
		for (;;) {
			if (consume(MUL) ) {
				if (!(exprPrefix())) err("expresie invalida dupa *");
			} else if (consume(DIV)) {
				if (!(exprPrefix())) err("expresie invalida dupa /");
			} else break;
		}
		return 1;
	}
	idxCrtAtom = startIdx;
	return 0;
}

// exprPrefix ::= (SUB | NOT)? factor
int exprPrefix() {
	int startIdx = idxCrtAtom;
	if (consume(SUB)) {}
	else consume(NOT);
	if (factor()) { return 1; }
	idxCrtAtom = startIdx;
	return 0;
}

//factor ::= INT
//			| REAL
//			| STR
//			| LPAR expr RPAR
//			| ID (LPAR (expr (COMMA expr)* )? RPAR)?
int factor() {
	int startIdx = idxCrtAtom;
	if (consume(INT)) { return 1; }
	else if (consume(REAL)) { return 1; }
	else if (consume(STR)) { return 1; }
	else if (consume(LPAR)) {
		if (expr()) {
			if (consume(RPAR)) { return 1; }
			else err("lipseste ) dupa expresie");
		} else err("expresie invalida dupa (");
		idxCrtAtom = startIdx;
	}
	else if (consume(ID)) {
		if (consume(LPAR)) {
			if (expr()) {
				for (;;) {
					if (consume(COMMA)) {
						if (!(expr()))
							err("expresie invalida dupa ,");
					} else break;
				}
			}
			if (!(consume(RPAR)))
				err("lipseste ) dupa expresie");
		}
		return 1;
	}
	idxCrtAtom = startIdx;
	return 0;
}

int main() {

	FILE* fis;
	fis = fopen("1.q", "rb");
	if (fis == NULL) {
		printf("Nu s-a putut deschide fisierul");
		return -1;
	}
	int n = fread(bufin, 1, 30000, fis);
	bufin[n] = '\0';
	fclose(fis);
	pch = bufin;
	while (getNextTk() != FINISH) {}
	afisare();

	printf("\n");
	if (program()) {
		printf("Sintaxa OK\n");
	}
	else {
		printf("Eroare de sintaxa\n");
	}
	return 1;
}

void err(const char* msg) {
	printf("\nEroare in linia %d: %s\n", atomi[idxCrtAtom].linie, msg);
	exit(-1);
}