
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <regex.h>
#include <sys/stat.h>
#include <sys/types.h>  
#include "regex.h"
#include "scanner.h"

using namespace std;

Token::Token (string _lexeme, tokenType _terminal, Token *_next) {
	lexeme = _lexeme;
	terminal = _terminal;
	next = _next ;
}

Token * Scanner::matchNextToken(const char *text) {
	int maxNumMatchedChars = 0, numMatchedChars = 0, token_holder;
	tokenType token_value;
	string lexeme = "";
	for (int i=intKwd; i != lexicalError + 1; i++){
		numMatchedChars = matchRegex(regexes[i], text);
		if (numMatchedChars > maxNumMatchedChars) {
			maxNumMatchedChars = numMatchedChars;
			token_holder = i;
			lexeme.assign(text, numMatchedChars);
		}
	}
	token_value = static_cast<tokenType>(token_holder);
	return new Token(lexeme, token_value, NULL) ;
}

int Scanner::consumeWhiteSpaceAndComments (regex_t *whiteSpace, 
                                  regex_t *blockComment, 
				  				  regex_t *lineComment,
                                  const char *text) {
    int numMatchedChars = 0 ;
    int totalNumMatchedChars = 0 ;
    int stillConsumingWhiteSpace ;

    do {
        stillConsumingWhiteSpace = 0 ;  // exit loop if not reset by a match

        // Try to match white space
        numMatchedChars = matchRegex (whiteSpace, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match block comments
        numMatchedChars = matchRegex (blockComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match single-line comments
        numMatchedChars = matchRegex (lineComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }
    }
    while ( stillConsumingWhiteSpace ) ;    

    return totalNumMatchedChars ;
}

Token * Scanner::scan(const char *text) {

	regex_t *whiteSpace ;
    whiteSpace = makeRegex ("^[\n\t\r ]+") ;

    regex_t *blockComment ;
    blockComment = makeRegex ("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/") ;

    regex_t *lineComment ;
    lineComment = makeRegex ("^//[^\n]*\n") ;

    // Removes any initial whitespace or comments
    // Declares what the first token is so that 
	// we can later return the front of the linked list
	Token *startingToken = NULL ;
	Token *curToken = startingToken ;
	Token *newestToken = startingToken ;
	int numMatchedChars = consumeWhiteSpaceAndComments(whiteSpace, blockComment, lineComment, text) ;
	text = text + numMatchedChars ;
	while ( text[0] != '\0') {
		
		newestToken = matchNextToken(text) ;
		if (startingToken == NULL) {
			startingToken = newestToken;
			curToken = newestToken;
		} else {
			curToken->next = newestToken ;
			curToken = newestToken ;
		}
		text = text + curToken->lexeme.length();
		numMatchedChars = consumeWhiteSpaceAndComments(whiteSpace, blockComment, lineComment, text) ;
		text = text + numMatchedChars;
	}
	newestToken = new Token("endOfFile", endOfFile, NULL);
	if (startingToken == NULL) {
		return newestToken;
	}
	curToken->next = newestToken;
	return startingToken;
}
