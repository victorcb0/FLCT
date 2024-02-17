#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

#include <Ex 5.h>
#include <ad1.h>
#include <at.h>

int idxCrtAtom = 0;

Atom* consumed;

int consume(int cod) {
	if (atomi[idxCrtAtom].cod == cod) {

		consumed = &atomi[idxCrtAtom];

		idxCrtAtom++;
		return true;
	}
	return false;
}

void tkerr(const char* msg, const char* nume);
void tkerr1(const char* msg);
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

	adaugaDomeniu(); // adauga domeniul global in TS

	adaugaFnPredefinite(); // se insereaza dupa codul de la analiza de domeniu

	for (;;) {
		if (defVar()) {}
		else if (defFunc()) {}
		else if (block()) {}
		else break;
	}
	if (consume(FINISH)) { 

		stergeDomeniu(); // sterge domeniul global din TS

		return 1; 
	}
	idxCrtAtom = startIdx;
	return 0;
}

// defVar ::= VAR ID COLON baseType SEMICOLON   
int defVar() {
	int startIdx = idxCrtAtom;
	if (consume(VAR)) {
		if (consume(ID)) {

			const char* nume = consumed->s;
			Simbol* s = cautaInDomeniulCurent(nume);
			if (s) { tkerr("redefinire simbol: %s", nume); }
			s = adaugaSimbol(nume, FEL_VAR);
			s->local = crtFn != NULL;

			if (consume(COLON)) {
				if (baseType()) {

					s->tip = ret.tip;

					if (consume(SEMICOLON)) { return 1; }
					else err("lipseste ; la finalul declaratiei de variabila");
				}
				else err("tip invalid de variabila");
			}
			else err("lipseste : dupa numele variabilei");
		}
		else err("lipseste numele variabilei");
	}
	idxCrtAtom = startIdx;
	return 0;
}

// baseType ::= TYPE_INT | TYPE_REAL | TYPE_STR
int baseType() {
	int startIdx = idxCrtAtom;
	if (consume(TYPE_INT)) {
		ret.tip = TYPE_INT;

		return 1;
	}
	else if (consume(TYPE_REAL)) {
		ret.tip = TYPE_REAL;

		return 1;
	}
	else if (consume(TYPE_STR)) {
		ret.tip = TYPE_STR;

		return 1;
	}
	//else err("tip invalid");
	idxCrtAtom = startIdx;
	return 0;
}

// defFunc ::= FUNCTION ID LPAR funcParams RPAR COLON baseType defVar* block END
int defFunc() {
	int startIdx = idxCrtAtom;
	if (consume(FUNCTION)) {
		if (consume(ID)) {

			const char* nume = consumed->s;
			Simbol* s = cautaInDomeniulCurent(nume);
			if (s) { tkerr("redefinire simbol: %s", nume); }
			crtFn = adaugaSimbol(nume, FEL_FN);
			crtFn->args = NULL;
			adaugaDomeniu();

			if (consume(LPAR)) {
				if (funcParams()) {
					if (consume(RPAR)) {
						if (consume(COLON)) {
							if (baseType()) {

								crtFn->tip = ret.tip;

								for (;;) {
									if (!(defVar())) break;
								}
								if (block()) {
									if (consume(END)) { 
										
										stergeDomeniu();
										crtFn = NULL;

										return 1; 
									}
									else err("lipseste end dupa continutul functiei");
								}
								else err("continut invalid de functie");
							}
							else err("lipseste tipul functiei");
						}
						else err("lipseste : dupa )");
					}
					else err("lipseste ) dupa parametrii functiei");
				}
				else err("parametri invalizi de functie");
			}
			else err("lipseste ( dupa numele functiei");
		}
		else err("lipseste numele functiei");
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
			}
			else break;
		}
	}
	return 1;
}

// funcParam ::= ID COLON baseType
int funcParam() {
	int startIdx = idxCrtAtom;
	if (consume(ID)) {

		const char* nume = consumed->s;
		Simbol* s = cautaInDomeniulCurent(nume);
		if (s) { tkerr("redefinire simbol: %s", nume); }
		s = adaugaSimbol(nume, FEL_ARG);
		Simbol* argFn = adaugaArgFn(crtFn, nume);

		if (consume(COLON)) {
			if (baseType()) { 
				
				s->tip = ret.tip;
				argFn->tip = ret.tip;

				return 1; 
			}
			else err("lipseste tipul parametrului de functie dupa :");
		}
		else err("lipseste : dupa numele parametrului de functie");
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

				if (ret.tip == TYPE_STR) tkerr1("conditia lui if trebuie sa aiba tipul int sau real");

				if (consume(RPAR)) {
					if (block()) {
						if (consume(ELSE)) {
							if (!(block())) err("continut invalid de dupa else");
						}
						if (consume(END)) { return 1; }
						else err("lipseste end la sfarsit de if");
					}
					else err("continut invalid dupa )");
				}
				else err("lipseste ) dupa expresie");
			}
			else err("expresie invalida dupa (");
		}
		else err("lipseste ( dupa if");
	}
	idxCrtAtom = startIdx;
	if (consume(RETURN)) {
		if (expr()) {

			if (!crtFn) tkerr1("return poate fi folosit doar intr-o functie");
			if (ret.tip != crtFn->tip) tkerr1("tipul lui return este diferit de tipul returnat de functie");

			if (consume(SEMICOLON)) { return 1; }
			else err("lipseste ; dupa expresie");
		}
		else err("lipseste expresia dupa return");
	}
	idxCrtAtom = startIdx;
	if (consume(WHILE)) {
		if (consume(LPAR)) {
			if (expr()) {
				
				if (ret.tip == TYPE_STR) tkerr1("conditia lui while trebuie sa aiba tipul int sau real");

				if (consume(RPAR)) {
					if (block()) {
						if (consume(END)) { return 1; }
						else err("lipseste end la sfarsit de while");
					}
					else err("continut invalid dupa )");
				}
				else err("lipseste ) dupa expresie");
			}
			else err("expresie invalida dupa (");
		}
		else err("lipseste ( dupa while");
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

				Ret tipStanga = ret;
				if (tipStanga.tip == TYPE_STR) tkerr1("operandul stang al lui && nu poate fi de tip str");

				if (exprAssign()) {

					if (ret.tip == TYPE_STR) tkerr1("operandul drept al lui && nu poate fi de tip str");
					setRet(TYPE_INT, false);

				}
				else err("expresie invalida dupa &&");
			}
			else if (consume(OR)) {

				Ret tipStanga = ret;
				if (tipStanga.tip == TYPE_STR) tkerr1("operandul stang al lui || nu poate fi de tip str");

				if (exprAssign()) {

					if (ret.tip == TYPE_STR) tkerr1("operandul drept al lui || nu poate fi de tip str");
					setRet(TYPE_INT, false);

				}
				else err("expresie invalida dupa ||");
			}
			else break;
		}
		return 1;
	}
	idxCrtAtom = startIdx;
	return 0;
}

// exprAssign ::= (ID ASSIGN)? exprComp -> ID ASSIGN exprComp | exprComp
int exprAssign() {
	int startIdx = idxCrtAtom;
	//if (consume(ID)) {
	//	if (!(consume(ASSIGN)))
	//		idxCrtAtom = startIdx;
	//}
	//if (exprComp()) { return 1; }
	if (consume(ID)) {

		const char* nume = consumed->s;

		if (consume(ASSIGN))
		{
			if (exprComp()) {

				Simbol* s = cautaSimbol(nume);
				if (!s) tkerr("identificator necunoscut: %s", nume);
				if (s->tip == FEL_FN) tkerr("o functie (%s) nu poate fi folosita ca destinatie a unei atribuiri", nume);
				if (s->tip != ret.tip) tkerr1("sursa si destinatia atribuirii au tipuri diferite");
				ret.lval = false;

				return 1;
			}
			else err("expresie invalida dupa =");
		}
	}
	idxCrtAtom = startIdx;
	if (exprComp()) {
		return 1;
	}
	idxCrtAtom = startIdx;
	return 0;
}

// exprComp ::= exprAdd ( (LESS | EQUAL) exprAdd )?
int exprComp() {
	int startIdx = idxCrtAtom;
	if (exprAdd()) {
		if (consume(LESS)) {

			Ret tipStanga = ret;

			if (exprAdd()) {

				if (tipStanga.tip != ret.tip) tkerr1("tipuri diferite pentru operanzii lui <");
				setRet(TYPE_INT, false); // rezultatul comparatiei este int 0 sau 1

			}
			else err("expresie invalida dupa <");
		}
		else if (consume(EQUAL)) {

			Ret tipStanga = ret;

			if (exprAdd())  {

				if (tipStanga.tip != ret.tip) tkerr1("tipuri diferite pentru operanzii lui ==");
				setRet(TYPE_INT, false); // rezultatul comparatiei este int 0 sau 1

			}
			else err("expresie invalida dupa ==");
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

				Ret tipStanga = ret;
				if (tipStanga.tip == TYPE_STR) tkerr1("operanzii lui + nu pot fi de tip str");

				if (exprMul()) {

					if (tipStanga.tip != ret.tip) tkerr1("tipuri diferite pentru operanzii lui +");
					ret.lval = false;

				}
				else err("expresie invalida dupa +");
			}
			else if (consume(SUB)) {

				Ret tipStanga = ret;
				if (tipStanga.tip == TYPE_STR) tkerr1("operanzii lui - nu pot fi de tip str");

				if (exprMul()) {

					if (tipStanga.tip != ret.tip) tkerr1("tipuri diferite pentru operanzii lui -");
					ret.lval = false;

				}
				else err("expresie invalida dupa -");
			}
			else break;
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
			if (consume(MUL)) {

				Ret tipStanga = ret;
				if (tipStanga.tip == TYPE_STR) tkerr1("operanzii lui * nu pot fi de tip str");

				if (exprPrefix()) {

					if (tipStanga.tip != ret.tip) tkerr1("tipuri diferite pentru operanzii lui *");
					ret.lval = false;
					
				}
				else err("expresie invalida dupa *");
			}
			else if (consume(DIV)) {

				Ret tipStanga = ret;
				if (tipStanga.tip == TYPE_STR) tkerr1("operanzii lui / nu pot fi de tip str");

				if (exprPrefix()) {

					if (tipStanga.tip != ret.tip) tkerr1("tipuri diferite pentru operanzii lui /");
					ret.lval = false;

				}
				else err("expresie invalida dupa /");
			}
			else break;
		}
		return 1;
	}
	idxCrtAtom = startIdx;
	return 0;
}

// exprPrefix ::= (SUB | NOT)? factor -> SUB factor | NOT factor | factor
int exprPrefix() {
	int startIdx = idxCrtAtom;
	/*if (consume(SUB)) {}
	else consume(NOT);
	if (factor()) { return 1; }*/
	if (consume(SUB)) {
		if (factor()) {

			if (ret.tip == TYPE_STR) tkerr1("expresia lui - unar trebuie sa aiba tipul int sau real");
			ret.lval = false;

			return 1;
		}
	}
	idxCrtAtom = startIdx;
	if (consume(NOT)) {
		if (factor()) {

			if (ret.tip == TYPE_STR) tkerr1("expresia sa aiba tipul int sau real");
			setRet(TYPE_INT, false);

			return 1;
		}
	}
	idxCrtAtom = startIdx;
	if (factor()) {
		return 1;
	}
	idxCrtAtom = startIdx;
	return 0;
}

//factor ::= INT
//			| REAL
//			| STR
//			| LPAR expr RPAR
//			| ID (LPAR (expr (COMMA expr)* )? RPAR)? -> ID (LPAR (expr (COMMA expr)* )? RPAR | epsilon)
int factor() {
	int startIdx = idxCrtAtom;
	if (consume(INT)) { 

		setRet(TYPE_INT, false);

		return 1; 
	}
	else if (consume(REAL)) { 
		
		setRet(TYPE_REAL, false);
		
		return 1; 
	}
	else if (consume(STR)) { 
		
		setRet(TYPE_STR, false);

		return 1; 
	}
	else if (consume(LPAR)) {
		if (expr()) {
			if (consume(RPAR)) { return 1; }
			else err("lipseste ) dupa expresie");
		}
		else err("expresie invalida dupa (");
	}
	idxCrtAtom = startIdx;
	if (consume(ID)) {

		Simbol* s = cautaSimbol(consumed->s);
		if (!s) tkerr("identificator necunoscut: %s", consumed->s);

		if (consume(LPAR)) {

			if (s->fel != FEL_FN) tkerr("%s nu poate fi apelata, deoarece nu este o functie", s->nume); // modificat
			Simbol* argDef = s->args;

			if (expr()) {

				if (!argDef) tkerr("functia %s este apelata cu prea multe argumente", s->nume);
				if (argDef->tip != ret.tip) tkerr("tipul argumentului de la apelul functiei %s este diferit de cel de la definirea ei", s->nume);
				argDef = argDef->urm;

				for (;;) {
					if (consume(COMMA)) {
						if (expr()) {

							if (!argDef) tkerr("functia %s este apelata cu prea multe argumente", s->nume);
							if (argDef->tip != ret.tip) tkerr("tipul argumentului de la apelul fucntiei %s este diferit de cel de la definirea ei", s->nume);
							argDef = argDef->urm;

						}
						else err("expresie invalida dupa ,");
					}
					else break;
				}
			}
			if (consume(RPAR)) { 

				if (argDef) tkerr("functia %s este apelata cu prea putine argumente", s->nume);
				setRet(s->tip, false);

				return 1;
			}
			else err("lipseste ) dupa expresie");
		}

		// epsilon
		if (s->fel == FEL_FN) tkerr("functia %s se poate doar apela", s->nume);
		setRet(s->tip, true);

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
		printf("\nSintaxa OK\n");
	}
	else {
		printf("\nEroare de sintaxa\n");
	}
	return 1;
}

void err(const char* msg) {
	printf("\nEroare in linia %d: %s\n", atomi[idxCrtAtom].linie, msg);
	exit(-1);
}

void tkerr(const char* msg, const char* nume) {
	printf("\nEroare in linia %d : ", consumed->linie);
	printf(msg, nume);
	printf("\n");
	exit(-1);
}

void tkerr1(const char* msg) {
	printf("\nEroare in linia %d : ", consumed->linie);
	printf(msg);
	printf("\n");
	exit(-1);
}

// adauga in tabela de simboluri o functie care are un argument 
// argumentul are tipul tipArg, iar functia are tipul returnat tipRet
Simbol* adaugaFn1Arg(const char* numeFn, int tipArg, int tipRet) {
	Simbol* fn = adaugaSimbol(numeFn, FEL_FN);
	fn->tip = tipRet;
	fn->args = NULL;
	Simbol* arg = adaugaArgFn(fn, "arg");
	arg->tip = tipArg;
	return fn;
}

void adaugaFnPredefinite() {
	adaugaFn1Arg("puti", TYPE_INT, TYPE_INT);
	adaugaFn1Arg("putr", TYPE_REAL, TYPE_REAL);
	adaugaFn1Arg("puts", TYPE_STR, TYPE_STR);
}

void setRet(int tip, bool lval) {
	ret.tip = tip;
	ret.lval = lval;
}