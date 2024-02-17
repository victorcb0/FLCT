# FLCT (Formal Languages and Compilation Techniques - Quick Language)
Formal Languages and Compilation Techniques (FLCT) covers key principles in programming language development and compilation techniques. Regular expressions (regex), transition diagram, lexical analyzer, syntactic analyzer, domain analyzer, type analyzer and code generation are presented. The goal is to understand and create a programming language (Quick Language), by approaching its various components step by step.

Example of a program in the Quick language:
```
# program de test

function max(x:int, y:int):int
    if(x<y)
        return y;
        else
        return x;
        end
    end

var i:int;
i=0;    # iterator
while(i<10)
    puti(max(i,5));
    i=i+1;
    end

puts("PI=");
putr(3.14159);
```

## Laboratory 1-2 - REGEX
Regular expressions are text search and/or replacement patterns used for efficient manipulation and processing of strings in various applications and programming languages. They provide a powerful and flexible method of identifying and manipulating text according to certain defined criteria.

1. Application 1.1: In a text there are calendar dates of the form "day/month/year", where the day and month consist of 1-2 digits and the year consists of 2 or 4 digits. Write a regular expression that finds all this data. It is not necessary to validate the calendar data.
- Ex: Ion was born on 21/5/1990 and Maria on 7/12/92.

3. Application 1.2: Propose a text in which there is a sequence of characters that matches the regular expression:
- [A-Z][a-zA-Z]*,[a-z]+ [0-9]+(\/.)*(\.|\?)

3. Application 1.3: Write a regular expression that tests a simplified syntax for character classes. They must start with "[" and end with "]"; optionally after "[" can be "^"; inside can be single characters or ranges of characters; a character can be plain or prefixed by "\".
- Valid examples: [abc] [^0-9] [\r\n\t ] [., !?;\-] [\\]
- Invalid examples (must not be recognized): [\] [\\\] [a

## Laboratory 3-4 - Diagram of Transitions
A transition diagram is a graphical representation of the states of a system and the transitions between those states. These states can represent various conditions, situations, or states of the system, and transitions indicate the changes between these states following specific events or actions. It provides an intuitive way to visualize and understand the operation of a system and can be useful in designing, developing and debugging it.

![picture alt](https://github.com/victorcb0/FLCT/blob/main/Laborator%203-4/Diagrama%20de%20tranzi%C8%9Bie%20-%20Ciobanu%20Victor.png)

## Laboratory 5-6 - Lexical Analyzer
The lexical analyzer in the Quick language is the component that scans and analyzes the string in the source code to identify and classify lexical units or "tokens". These tokens are atomic components of the language, such as identifiers, keywords, operators, constants, etc. The lexical analyzer has the role of separating and recognizing these lexical units according to the predefined rules for the Quick language. Lexical rules are defined to specify how the different types of tokens in the source code are recognized and interpreted.

Lexical rules:
1. Identifiers
```
ID: [a-zA-Z_] [a-zA-Z0-9_]*
```

2. Keywords
```
VAR: 'var'
FUNCTION: 'function'
IF: 'if'
ELSE: 'else'
WHILE: 'while'
END: 'end'
RETURN: 'return'
TYPE_INT: 'int'
TYPE_REAL: 'real'
TYPE_STR: 'str'
```

3. Constants
```
INT ::= [0-9]+
REAL ::= [0-9]+ [.] [0-9]+
STR ::= ["] [^"]* ["]
```

4. Delimiters
```
COMMA: ','
COLON: ':'
SEMICOLON: ';'
LPAR: '('
RPAR: ')'
FINISH: '\0' | EOF // '\0' - string terminator; EOF - C code for End Of File
```

5. Operators
```
ADD: '+'
SUB: '-'
MUL: '*'
DIV: '/'
AND: '&&'
OR: '||'
NOT: '!'
ASSIGN: '='
EQUAL: '=='
NOTEQ: '!='
LESS: '<'
```

6. Spaces - these do not form lexical atoms
```
SPACE: [ \n\r\t] // also contains the space character
COMMENT: '#' [^\n\r\0]*
```

## Laboratory 7-9 - Parser
The parser in the Quick language is the component responsible for checking and correctly structuring the syntax of the source code according to the grammar rules defined for that language. The parser receives the sequence of tokens produced by the lexical analyzer and applies the syntax rules to build a parse tree or generate intermediate code or machine code. Syntax rules define how tokens can be combined and organized to form valid expressions in the Quick language. The parser checks that the token sequence follows these syntactic rules and generates errors or warnings if incorrect or ambiguous syntax is detected. The parser provides a coherent and well-defined structure of the source code, which can then be used in later stages of the process of compiling or interpreting programs written in the Quick language.

Syntactic rules:
```
program ::= ( defVar | defFunc | block )* FINISH
defVar ::= VAR ID COLON baseType SEMICOLON
baseType ::= TYPE_INT | TYPE_REAL | TYPE_STR
defFunc ::= FUNCTION ID LPAR funcParams RPAR COLON baseType defVar* block END
block ::= instr+
funcParams ::= ( funcParam ( COMMA funcParam )* )?
funcParam ::= ID COLON baseType
instr ::= expr? SEMICOLON
| IF LPAR expr RPAR block ( ELSE block )? END
| RETURN expr SEMICOLON
| WHILE LPAR expr RPAR block END
expr ::= exprLogic
exprLogic ::= exprAssign ( ( AND | OR ) exprAssign )*
exprAssign ::= ( ID ASSIGN )? exprComp
exprComp ::= exprAdd ( ( LESS | EQUAL ) exprAdd )?
exprAdd ::= exprMul ( ( ADD | SUB ) exprMul )*
exprMul ::= exprPrefix ( ( MUL | DIV ) exprPrefix )*
exprPrefix ::= ( SUB | NOT )? factor
factor ::= INT
| REAL
| STR
| LPAR expr RPAR
| ID ( LPAR ( expr ( COMMA expr )* )? RPAR )?
```

## Laboratory 10 - The Domain Analyzer
The domain analyzer in the Quick language is a component that checks the correctness and semantic coherence of the source code within the specific domain of the respective language. This parser examines the meaning and correct interpretation of statements and data structures according to the semantic rules and conventions of the Quick language. The domain analyzer is essential to ensure that the source code follows the programmer's intentions and is logically meaningful. By applying semantic rules, this analyzer helps prevent interpretation or execution errors during program execution.

Semantic rules for domain analysis:
```
1. The global scope starts at the beginning of the program and lasts until its end
2. At '(' from the arguments of a function starts the local domain of that function, which lasts until the end of the function
3. In a field, there are not allowed to have more than one symbol with the same name
```

Messages that appear when domain analysis is performed on the lab example:
```
create new domain
         add the max symbol
create new domain
         add the x symbol
         add the x symbol as an argument
         add the y symbol
         add the symbol y as an argument
delete current domain
         delete the y symbol
         delete the x symbol
returns to the parent domain
         add the i symbol
delete current domain
         delete the i symbol
         delete the max symbol
         delete the x symbol
         delete the y symbol
returns to the parent domain
```

## Laboratory 11 - The Type Analyzer
The type analyzer in the Quick language is a component responsible for checking the correctness and consistency of the use of data types within the program. This parser examines how the various data types are declared, used, and converted according to the semantic rules and conventions of the Quick language. The type analyzer is essential to ensure program consistency and correctness, helping to prevent common errors related to data type manipulation and ensuring logical and predictable program behavior at runtime.

Semantic rules for type analysis:
```
1. Symbols must be defined before their use
2. Functions can only be called
3. A function call must have the same number of arguments as the function definition
4. The arguments in the function call must have the same types as in the definition
5. A variable cannot be called as a function
6. The return type of the return must be identical to that returned by the function
7. Return can only exist in a function
8. If and while conditions must be of type int or real
9. There are no implicit conversions between data types, not even between int and real. For this reason the operands of binary arithmetic and comparison operators ( + - * / < == ) must have identical types. The operands of the binary logical operators ( && || ) can have different types.
10. Arithmetic and logical operators ( + - (including unary) * / && || ! ) are defined only for int and real types
11. Assignment and comparison operators ( = < == ) are defined for all types
12. Comparison and logical operators ( && || ! < == ) return int 0 or 1 (false/true)
```

## Laboratory 12 - The Domain Analyzer
Code generation in the Quick language is the process of transforming an intermediate representation of the program into executable code, or a form that can be used for interpretation or compilation. This process involves translating and optimizing instructions and data structures from source code into machine code or another lower-level language. Code generation in the Quick language is an essential process in software development, enabling the transformation of source code into a form that can be executed by a computer or by a specific execution environment.
