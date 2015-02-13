#ifndef SCANNER_H
#define SCANNER_H

#include <regex.h>
#include <string>
#include "regex.h"

class Token ;

/* This enumerated type is used to keep track of what kind of
   construct was matched. 
 */

enum tokenEnumType { 

	intKwd, floatKwd, boolKwd,
	trueKwd, falseKwd, stringKwd,  matrixKwd,
	letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd,
	forKwd, whileKwd, printKwd,

	// Constants
	intConst, floatConst, stringConst, 

	// Names
	variableName ,

	// Punctuation
	leftParen, rightParen, 
	leftCurly, rightCurly, 
	leftSquare, rightSquare,

	comma, semiColon, colon,

	//Operators
	assign, 
	plusSign, star, dash, forwardSlash,
	lessThan, lessThanEqual, greaterThan, greaterThanEqual,
	equalsEquals, notEquals,
	andOp, orOp, notOp,

	// Special terminal types
	endOfFile ,
	lexicalError
} ;
typedef enum tokenEnumType tokenType ;

// Below you need to write your class definitions for Token and Scanner.

class Token {
	public:
		Token (std::string, tokenType, Token *);
		std::string lexeme ;
		tokenType terminal ;
		Token * next ;
} ;

class Scanner {
	public:
		regex_t* regexes[lexicalError+1] ;
		Scanner() {
			regexes[intKwd] 			= makeRegex("^Int") ;
			regexes[floatKwd] 			= makeRegex("^Float") ;
			regexes[boolKwd]			= makeRegex("^Bool") ;
			regexes[trueKwd]			= makeRegex("^True") ;
			regexes[falseKwd] 			= makeRegex("^False") ;
			regexes[stringKwd] 			= makeRegex("^Str") ;
			regexes[matrixKwd] 			= makeRegex("^Matrix") ;
			regexes[letKwd] 			= makeRegex("^let") ;
			regexes[inKwd] 				= makeRegex("^in") ;
			regexes[endKwd] 			= makeRegex("^end");
			regexes[ifKwd] 				= makeRegex("^if") ;
			regexes[thenKwd] 			= makeRegex("^then") ;
			regexes[elseKwd] 			= makeRegex("^else") ;
			regexes[forKwd] 			= makeRegex("^for") ;
			regexes[whileKwd] 			= makeRegex("^while") ;
			regexes[printKwd] 			= makeRegex("^print") ;
			regexes[intConst]   		= makeRegex("^[0-9]+") ;
			regexes[floatConst] 		= makeRegex("^[0-9]*\\.?[0-9]*") ;
			regexes[stringConst] 		= makeRegex("^\"(\\.|[^\"])*\"");
			regexes[variableName] 		= makeRegex("^[a-zA-Z_][a-zA-Z_0-9]*") ; 
			regexes[leftParen] 			= makeRegex("^\\(") ;
			regexes[rightParen] 		= makeRegex("^)") ;
			regexes[leftCurly] 			= makeRegex("^\\{") ;
			regexes[rightCurly] 		= makeRegex("^}") ;
			regexes[leftSquare] 		= makeRegex("^\\[") ;
			regexes[rightSquare] 		= makeRegex("^]") ;
			regexes[comma] 				= makeRegex("^,") ;
			regexes[semiColon] 			= makeRegex("^;") ;
			regexes[colon] 				= makeRegex("^:") ;
			regexes[assign] 			= makeRegex("^=") ;
			regexes[plusSign] 			= makeRegex("^\\+") ;
			regexes[star] 				= makeRegex("^\\*") ;
			regexes[dash] 				= makeRegex("^-") ;
			regexes[forwardSlash] 		= makeRegex("^/") ;
			regexes[lessThan] 			= makeRegex("^<") ;
			regexes[lessThanEqual] 		= makeRegex("^<=") ;
			regexes[greaterThan] 		= makeRegex("^>") ;
			regexes[greaterThanEqual]	= makeRegex("^>=") ;
			regexes[equalsEquals] 		= makeRegex("^==") ;
			regexes[notEquals] 			= makeRegex("^!=") ;
			regexes[andOp] 				= makeRegex("^&&") ;
			regexes[orOp] 				= makeRegex("^\\|\\|") ;
			regexes[notOp] 				= makeRegex("^!") ;
			regexes[endOfFile] 			= makeRegex("^EOF") ;
			regexes[lexicalError] 		= makeRegex("[^ ]") ;
		}
		Token * scan(const char *text) ;
		Token * matchNextToken(const char *text) ;
		int consumeWhiteSpaceAndComments (regex_t *whiteSpace, regex_t *blockComment, regex_t *lineComment, const char *text) ;
} ;

#endif /* SCANNER_H */
