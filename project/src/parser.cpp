/* A recursive descent parser with operator precedence.

   Author: Eric Van Wyk
   Modified: Robert Edge        

   This algorithm is based on the work of Douglas Crockford in "Top
   Down Operator Precedence", a chapter in the book "Beautiful Code".
   Mr. Crockford describes in his chapter how his work is based on an
   algorithm described Vaughan Pratt in "Top Down Operator
   Precedence", presented at the ACM Symposium on Principles of
   Programming Languages.

   Douglas Crockford's chapter is available at 
    http://javascript.crockford.com/tdop/tdop.html

   Vaughan Pratt's paper is available at 
    http://portal.acm.org/citation.cfm?id=512931

   These are both quite interesting works and worth reading if you
   find the problem of parsing to be an interesting one.

   Last modified: Oct 13, 2013.

*/

#include "parser.h"
#include "scanner.h"
#include "extToken.h"
#include <stdio.h>
#include <assert.h>
using namespace std ;

/*! \class Parser 
    \brief Organizes FCAL text into an tree

    Parser is a collection of methods headlined by Parser::parse which takes
    in text. It then proceeds to march through the text and determines
    the syntax of the code, while creating a tree. It is heavily tied
    to ASt.h and AST.cpp in that it uses classes defined in those files
    to create the structure of the tree. After creating the tree, it can
    easily be traversed by way of executing methods for each Node - ie
    the classes declard in AST.h.

    Though it cannot do it on it's own, when paired with a grammar, this
    can translate code from one language to another. In its current form, 
    it is setup to translate FCAL code into C++ code. Heavy modifications 
    would be needed to alter the two languages.
 */

/*! \brief Default destructor
*/
Parser::~Parser() {
    if (s) delete s ;

    ExtToken *extTokenToDelete ;
    currToken = tokens ;
    while (currToken) {
        extTokenToDelete = currToken ;
        currToken = currToken->next ;
        delete extTokenToDelete ;
    }

    Token *tokenToDelete ;
    Token *currScannerToken = stokens ;
    while (currScannerToken) {
        tokenToDelete = currScannerToken ;
        currScannerToken = currScannerToken->next ;
        delete tokenToDelete ;
    }

}


/*! \brief Default constructor. Sets everything to NULL
*/
Parser::Parser ( ) { 
    currToken = NULL; prevToken = NULL ; tokens = NULL; 
    s = NULL; stokens = NULL; 
}

/*! \brief Parses the program

    First creates a list of tokens that represent the 
    input text. Second, it checks to make sure that the 
    tokens, and therfore the program, is well-formed 
    without any lexical errors. Third and final, 
    creates a tree out of the tokens.

    @return ParseResult 
*/
ParseResult Parser::parse (const char *text) {
    assert (text != NULL) ;

    ParseResult pr ;
    try {
        s = new Scanner() ;
        stokens = s->scan (text) ;        
        tokens = extendTokenList ( this, stokens ) ;

        assert (tokens != NULL) ;
        currToken = tokens ;
        pr = parseProgram( ) ;
    }
    catch (string errMsg) {
        pr.ok = false ;
        pr.errors = errMsg ;
        pr.ast = NULL ;
    }
    return pr ;
}

/* 
 * parse methods for non-terminal symbols
 * --------------------------------------
 */


/*! \brief Parses the program

    This is the initial part of parsing. It is finds the name
    of the function being parsed and then proceeds to find all 
    the subsequent statements held within the the body of the 
    method.

    @see Root

    @return ParseResult 
*/
ParseResult Parser::parseProgram () {
    ParseResult pr ;
    // root
    // Program ::= varName '(' ')' '{' Stmts '}' 
    match(variableName) ;
    string name( prevToken->lexeme ) ;
    VarName* varname = new VarName(name) ;
    match(leftParen) ;
    match(rightParen) ;
    match(leftCurly);
    ParseResult prStmts = parseStmts() ;
    Stmts* s;
    if (prStmts.ast) {
        s = dynamic_cast<Stmts *>(prStmts.ast) ;
    }
    match(rightCurly);
    match(endOfFile) ;
    
    pr.ast = new Root(varname, s) ;

    return pr ;
}


/*! \brief Parses a Matrix Declaration

    This serves the same purpose as a standard declaration
    parse, however, it is specifically tailored for matrices
    as they are a unique subset of declarations. It also is 
    able to correctly determine which type of matrix declaration
    is being implemented and act accordingly.

    @return ParseResult 
*/
ParseResult Parser::parseMatrixDecl () {
    ParseResult pr ;
    VarName *varName1, *varName2, *varName3;
    Expr *ex1, *ex2, *ex3;

    match(matrixKwd);
    match(variableName) ;
    varName1 = new VarName(prevToken->lexeme);

    // Decl ::= 'Matrix' varName '[' Expr ',' Expr ']' varName ',' varName  '=' Expr ';'
    if(attemptMatch(leftSquare)){
        ParseResult exPr1 = parseExpr(0);
        match(comma);
        ParseResult exPr2 = parseExpr(0);
        match(rightSquare);
        ParseResult namePr1 = parseVariableName();
        match(comma);
        ParseResult namePr2 = parseVariableName();
        match(assign);
        ParseResult exPr3 = parseExpr(0);

        if (exPr1.ast && exPr2.ast && exPr3.ast && namePr1.ast and namePr2.ast) {
            ex1 = dynamic_cast<Expr *>(exPr1.ast) ;
            ex2 = dynamic_cast<Expr *>(exPr2.ast) ;
            ex3 = dynamic_cast<Expr *>(exPr3.ast) ;
            varName2 = dynamic_cast<VarName *>(namePr1.ast) ;
            varName3 = dynamic_cast<VarName *>(namePr2.ast) ;

            pr.ast = new MatrixAdvDecl(varName1, varName2, varName3, ex1, ex2, ex3);
        }
    }
    // Decl ::= 'Matrix' varName '=' Expr ';'
    else if(attemptMatch(assign)){
        ParseResult exPr = parseExpr(0);
        if (exPr.ast) {
            ex1 = dynamic_cast<Expr *>(exPr.ast) ;
        }
        pr.ast = new MatrixDecl(varName1, ex1);
    }
    else{
        throw ( (string) "Bad Syntax of Matrix Decl in in parseMatrixDecl" ) ;
    }   

    match(semiColon) ;

    return pr ;
}
/*! \brief Parses a standard declaration of a variable

    Finds the keyword - ie int, float, etc - and creates a 
    node that accurately reflects the declaration happening. 

    @return ParseResult 
*/
ParseResult Parser::parseStandardDecl(){
    ParseResult pr ;
    
    //ParseResult prType = parseType() ;
    string keyword ;
    if ( attemptMatch(intKwd) || attemptMatch(floatKwd) || attemptMatch(stringKwd) || attemptMatch(boolKwd) ) {
        // Type ::= intKwd | floatKwd | strKwd | boolKwd
        keyword = prevToken->lexeme;
    } else {
        throw ( (string) "Invalid keyword in parseStandardDecl" ) ;
    }

    match(variableName) ;
    VarName* varName = new VarName(prevToken->lexeme);
    match(semiColon) ;
    pr.ast = new StandardDecl(keyword, varName);
    return pr ;
}

/*! \brief Parses declarations of variables

    Finds the keyword - ie int, float, matrix, etc - and 
    correctly passes responsibilities to the function 
    tailored to handle the task. If it sees 'Matrix' it
    will call Parser::parseMatrixDecl, else it will call
    Parser::parseStandardDecl.

    @return ParseResult 
*/
ParseResult Parser::parseDecl () {
    ParseResult pr ;
    // Decl :: Matrix variableName ....
    if(nextIs(matrixKwd)){
        pr = parseMatrixDecl();
    } 
    // Decl ::= Type variableName semiColon
    else{
        pr = parseStandardDecl();
    }
    return pr ;
}



/*! \brief Starting place to parse any type of Stmts

    Based on context clues, determines which type of 
    statements is next and pinpoints a function which
    is tailored to handle such a parse.

    @return ParseResult 
*/
ParseResult Parser::parseStmts () {
    ParseResult pr ;
    if ( ! nextIs(rightCurly) && !nextIs(inKwd)  ) {
        // Stmts ::= Stmt Stmts
        ParseResult prStmt = parseStmt() ;
        ParseResult prStmts = parseStmts() ;
    	Stmt* s;
    	Stmts* ss;
    	if (prStmt.ast && prStmts.ast) {
     	    s = dynamic_cast<Stmt *>(prStmt.ast) ;
    	    ss = dynamic_cast<Stmts *>(prStmts.ast) ;
    	}
	    pr.ast = new StmtStmts(s, ss);
    }
    else {
        // Stmts ::= 
        // nothing to match.
	    pr.ast = new EmptyStmts() ;
    }
    return pr ;
}


/*! \brief Starting place to parse any type of Stmt

    Based on context clues, determines which type of 
    statement is next and pinpoints a function which
    is tailored to handle such a parse

    @return ParseResult 
*/
ParseResult Parser::parseStmt () {
    ParseResult pr ;

    //Stmt ::= Decl
    if(nextIs(intKwd)||nextIs(floatKwd)||nextIs(matrixKwd)||nextIs(stringKwd)||nextIs(boolKwd)){
        pr = parseDecl();
    }
    //Stmt ::= '{' Stmts '}'
    else if (attemptMatch(leftCurly)){
        Stmts *stmts;
        ParseResult pr1 = parseStmts() ; 
        match(rightCurly);
        if (pr1.ast) {
            stmts = dynamic_cast<Stmts *>(pr1.ast);
        }
        pr.ast = new StmtBlock(stmts);
    }   
    //Stmt ::= 'if' '(' Expr ')' Stmt
    //Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    else if (attemptMatch(ifKwd)){
        Expr *ex;
        Stmt *stmt1, *stmt2;

        match(leftParen);
        ParseResult exPr = parseExpr(0);
        match(rightParen);
        ParseResult stmtPr1 = parseStmt();

        if (exPr.ast && stmtPr1.ast) {
            ex = dynamic_cast<Expr *>(exPr.ast);
            stmt1 = dynamic_cast<Stmt *>(stmtPr1.ast);
        }
        
        if(attemptMatch(elseKwd)){
            ParseResult stmtPr2 = parseStmt();
            if (stmtPr2.ast) {
                stmt2 = dynamic_cast<Stmt *>(stmtPr2.ast);
            }
            pr.ast = new IfElseStmt(ex, stmt1, stmt2);
        } else {
            pr.ast = new IfStmt(ex, stmt1);
        }

    }
    //Stmt ::= varName '=' Expr ';'  | varName '[' Expr ',' Expr ']' '=' Expr ';'
    else if  ( attemptMatch (variableName) ) {
        Expr *ex1 = NULL, *ex2, *ex3;
        VarName *varName = new VarName(prevToken->lexeme);

        if (attemptMatch ( leftSquare ) ) {
              ParseResult exPr1 = parseExpr(0);
              match ( comma ) ;
              ParseResult exPr2 = parseExpr (0) ;
              match  ( rightSquare ) ;
              if (exPr1.ast && exPr2.ast) {
                ex1 = dynamic_cast<Expr *>(exPr1.ast);
                ex2 = dynamic_cast<Expr *>(exPr2.ast);
              }
        }
        match(assign);
        ParseResult exPr3 = parseExpr(0);
        match(semiColon);
        if (exPr3.ast) {
            ex3 = dynamic_cast<Expr *>(exPr3.ast);
        }

        if (ex1 != NULL) {
            //Stmt ::= varName '[' Expr ',' Expr ']' '=' Expr ';'
            pr.ast = new MatrixAssignStmt(varName, ex1, ex2, ex3);
        } else {
            //Stmt ::= varName '=' Expr ';'
            pr.ast = new StandardAssignStmt(varName, ex3);
        }

    }
    //Stmt ::= 'print' '(' Expr ')' ';'
    else if ( attemptMatch (printKwd) ) {
        Expr *ex;
        match (leftParen) ;
        ParseResult exPr = parseExpr(0);
        match (rightParen) ;
        match (semiColon) ;
        if (exPr.ast) {
            ex = dynamic_cast<Expr *>(exPr.ast);
        }
        pr.ast = new PrintStmt(ex);
    }
    //Stmt ::= 'for' '(' varName '=' Expr ':' Expr ')' Stmt
    else if ( attemptMatch (forKwd) ) {
        VarName *varName;
        Expr *ex1, *ex2;
        Stmt *stmt;

        match (leftParen) ;
        match (variableName) ;
        varName = new VarName(prevToken->lexeme);
        match (assign) ;
        ParseResult ex1Pr = parseExpr (0) ;
        match (colon) ;
        ParseResult ex2Pr = parseExpr (0) ;
        match (rightParen) ;
        ParseResult stmtPr = parseStmt () ;
        if (ex1Pr.ast && ex2Pr.ast && stmtPr.ast) {
            ex1 = dynamic_cast<Expr *>(ex1Pr.ast);
            ex2 = dynamic_cast<Expr *>(ex2Pr.ast);
            stmt = dynamic_cast<Stmt *>(stmtPr.ast);
        }
        pr.ast = new ForStmt(varName, ex1, ex2, stmt);
    }
    //Stmt ::= 'while' '(' Expr ')' Stmt
    else if (attemptMatch(whileKwd)) {
        Expr *ex;
        Stmt *stmt;
        match(leftParen);
        ParseResult exPr = parseExpr(0);
        match(rightParen);
        ParseResult stmtPr = parseStmt();
        if (exPr.ast && stmtPr.ast) {
            ex = dynamic_cast<Expr *>(exPr.ast);
            stmt = dynamic_cast<Stmt *>(stmtPr.ast);
        }
        pr.ast = new WhileStmt(ex, stmt);
    }
    //Stmt ::= ';
    else if ( attemptMatch (semiColon) ) {
        // parsed a skip
    }
    else{
        throw ( makeErrorMsg ( currToken->terminal ) + " while parsing a statement" ) ;
    }
    // Stmt ::= variableName assign Expr semiColon
    return pr ;
}



/*! \brief Starting place to parse any type of Expr

    Based on context clues, determines which type of 
    expression is next and pinpoints a function which
    is tailored to handle such a parse

    @return ParseResult 
*/
ParseResult Parser::parseExpr (int rbp) {
    /* Examine current token, without consuming it, to call its
       associated parse methods.  The ExtToken objects have 'nud' and
       'led' methods that are dispatchers that call the appropriate
       parse methods.*/
    ParseResult left = currToken->nud() ;
   
    while (rbp < currToken->lbp() ) {
        left = currToken->led(left) ;
    }

    return left ;
}


/*
 * parse methods for Expr productions
 * ----------------------------------
 */

/*! \brief Parses the 'True' keyword

    @return ParseResult 
*/
 ParseResult Parser::parseTrueKwd ( ) {
     ParseResult pr ;
     match ( trueKwd ) ;
     pr.ast = new AnyConst(prevToken->lexeme);
     return pr ;
 }

/*! \brief Parses the 'False' keyword

    @return ParseResult 
*/
 ParseResult Parser::parseFalseKwd ( ) {
     ParseResult pr ;
     match ( falseKwd ) ;
     pr.ast = new AnyConst(prevToken->lexeme);
     return pr ;
 }

/*! \brief Parses a constant of type Int

    @return ParseResult 
*/
ParseResult Parser::parseIntConst ( ) {
    ParseResult pr ;
    match ( intConst ) ; 
    pr.ast = new AnyConst(prevToken->lexeme) ;
    return pr ;
}

/*! \brief Parses a constant of type Float

    @return ParseResult 
*/
ParseResult Parser::parseFloatConst ( ) {
    ParseResult pr ;
    match ( floatConst ) ;
    pr.ast = new AnyConst(prevToken->lexeme);
    return pr ;
}

/*! \brief Parses a constant of type string

    @return ParseResult 
*/
ParseResult Parser::parseStringConst ( ) {
    ParseResult pr ;
    match ( stringConst ) ;
    pr.ast = new AnyConst(prevToken->lexeme);
    return pr ;
}

/*! \brief Parses a variable name use

    Determines whether indeed a variable is being
    referenced or if a program is being called and
    parses appropriately

    @return ParseResult 
*/
ParseResult Parser::parseVariableName ( ) {
    ParseResult pr ;
    Expr *ex1, *ex2;

    match ( variableName ) ;
    VarName* varName = new VarName(prevToken->lexeme);
    if(attemptMatch(leftSquare)){
        ParseResult exPr1 = parseExpr(0);
        match(comma);
        ParseResult exPr2 = parseExpr(0);
        match(rightSquare);
        if (exPr1.ast && exPr2.ast) {
            ex1 = dynamic_cast<Expr *>(exPr1.ast);
            ex2 = dynamic_cast<Expr *>(exPr2.ast);
        }
        pr.ast = new MatrixRefExpr(varName, ex1, ex2);
    }
    //Expr ::= varableName '(' Expr ')'        //NestedOrFunctionCall
    else if(attemptMatch(leftParen)){
        ParseResult exPr = parseExpr(0);
        match(rightParen);
        if (exPr.ast) {
            ex1 = dynamic_cast<Expr *>(exPr.ast);
        }
        pr.ast = new FunctionCall(varName, ex1);
    }
    //Expr := variableName
    else{
        // variable 
        pr.ast = varName;
    }
    
    return pr ;
}


/*! \brief Parses a nested expression

    Important for maintaining order of operations
    within the program. For example: (1 + 1) * 2.

    @return ParseResult 
*/
ParseResult Parser::parseNestedExpr ( ) {
    ParseResult pr ;
    Expr *ex;
    match ( leftParen ) ;
    ParseResult exPr = parseExpr(0) ; 
    match(rightParen) ;
    if (exPr.ast) {
        ex = dynamic_cast<Expr *>(exPr.ast);
    }
    pr.ast = new ParensExpr(ex);
    return pr ;
}

/*! \brief Parses if-then-else expression

    @return ParseResult 
*/ 
ParseResult Parser::parseIfExpr(){  
    ParseResult pr ; 
    Expr *ex1, *ex2, *ex3;

    match(ifKwd);
    ParseResult exPr1 = parseExpr(0);
    match(thenKwd);
    ParseResult exPr2 = parseExpr(0);
    match(elseKwd);
    ParseResult exPr3 = parseExpr(0);
    if (exPr1.ast && exPr2.ast && exPr3.ast) {
        ex1 = dynamic_cast<Expr *>(exPr1.ast);
        ex2 = dynamic_cast<Expr *>(exPr2.ast);
        ex3 = dynamic_cast<Expr *>(exPr3.ast);
    }

    pr.ast = new IfExpr(ex1, ex2, ex3);
    return pr;
}


/*! \brief Parses let expression

    @return ParseResult 
*/ 
ParseResult Parser::parseLetExpr(){
    ParseResult pr ;
    Stmts *stmts;
    Expr *ex;

    match(letKwd);
    ParseResult stmtsPr = parseStmts();
    match(inKwd);
    ParseResult exPr = parseExpr(0);
    match(endKwd);
    if (stmtsPr.ast && exPr.ast) {
        stmts = dynamic_cast<Stmts *>(stmtsPr.ast);
        ex = dynamic_cast<Expr *>(exPr.ast);
    }

    pr.ast = new LetExpr(stmts, ex);
    return pr;
}

/*! \brief Parses the negation of a boolen expression

    @return ParseResult 
*/ 
ParseResult Parser::parseNotExpr () {
    ParseResult pr ;
    Expr *ex;

    match ( notOp ) ;
    ParseResult exPr = parseExpr( 0 ); 
    if (exPr.ast) {
        ex = dynamic_cast<Expr *>(exPr.ast);
    }

    pr.ast = new NotExpr(ex);
    return pr ;

}

/*! \brief Parses an addition operation of two expressions

    @return ParseResult 
*/ 
ParseResult Parser::parseAddition ( ParseResult prLeft ) {
    // parser has already matched left expression 
    Expr* left = dynamic_cast<Expr *>(prLeft.ast);
    ParseResult pr ;
    match ( plusSign ) ;
    string op = prevToken->lexeme;
    ParseResult exPr = parseExpr( prevToken->lbp() ); 
    Expr* right = dynamic_cast<Expr *>(exPr.ast);
    pr.ast = new BinOpExpr(left, op, right);
    return pr ;
}

/*! \brief Parses a multiplication operation of two expressions

    @return ParseResult 
*/ 
ParseResult Parser::parseMultiplication ( ParseResult prLeft ) {
    // parser has already matched left expression 
    Expr* left = dynamic_cast<Expr *>(prLeft.ast);
    ParseResult pr ;
    match ( star ) ;
    string op = prevToken->lexeme;
    ParseResult exPr = parseExpr( prevToken->lbp() ); 
    Expr* right = dynamic_cast<Expr *>(exPr.ast);
    pr.ast = new BinOpExpr(left, op, right);
    return pr ;
}

/*! \brief Parses a substraction operation of two expressions

    @return ParseResult 
*/ 
ParseResult Parser::parseSubtraction ( ParseResult prLeft ) {
    // parser has already matched left expression 
    Expr* left = dynamic_cast<Expr *>(prLeft.ast);
    ParseResult pr ;
    match ( dash ) ;
    string op = prevToken->lexeme;
    ParseResult exPr = parseExpr( prevToken->lbp() ); 
    Expr* right = dynamic_cast<Expr *>(exPr.ast);
    pr.ast = new BinOpExpr(left, op, right);
    return pr ;
}

/*! \brief Parses a division operation of two expressions

    @return ParseResult 
*/ 
ParseResult Parser::parseDivision ( ParseResult prLeft ) {
    // parser has already matched left expression 
    Expr* left = dynamic_cast<Expr *>(prLeft.ast);
    ParseResult pr ;
    match ( forwardSlash ) ;
    string op = prevToken->lexeme;
    ParseResult exPr = parseExpr( prevToken->lbp() ); 
    Expr* right = dynamic_cast<Expr *>(exPr.ast);
    pr.ast = new BinOpExpr(left, op, right);
    return pr ;
}


// Expr ::= Expr equalEquals Expr
// Expr ::= Expr lessThanEquals Expr
// Expr ::= Expr greaterThanEquals Expr
// Expr ::= Expr notEquals Expr
// Expr ::= Expr leftAngle Expr
// Expr ::= Expr rightAngle Expr
/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
 */

/*! \brief Parses two expressions being related to one another

    This is most often used in if-statments and while-loops.
    Common examples include "&&", ">", "<=", etc.

    @return ParseResult 
*/ 
ParseResult Parser::parseRelationalExpr ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;

    Expr* left = dynamic_cast<Expr *>(prLeft.ast);
    nextToken( ) ;
    // just advance token, since examining it in parseExpr caused
    // this method being called.
    string op = prevToken->lexeme ;
    ParseResult exPr = parseExpr( prevToken->lbp() ); 
    Expr* right = dynamic_cast<Expr *>(exPr.ast);
    pr.ast = new BinOpExpr(left, op, right);
    return pr ;
}


/*! \brief Matches next token

    Used when in order to be syntatically correct,
    the next token MUST equal something. For example,
    there must be a '=' in an assignment. If the 
    particular type is not found, an error is thrown

    @param tt Type that must be matched by next token
    @return void 
*/ 
void Parser::match (tokenType tt) {
    if (! attemptMatch(tt)) {
        throw ( makeErrorMsgExpected ( tt ) ) ;
    }
}

/*! \brief Attempts to match next token

    Used to check if the next token equals something,
    For example, checking whether there is an 'else'
    in an if-statement or not.

    @param tt Type next token might match
    @return void 
*/ 
bool Parser::attemptMatch (tokenType tt) {
    if (currToken->terminal == tt) { 
        nextToken() ;
        return true ;
    }
    return false ;
}

/*! \brief Determines if next token is of a certain type

    @param tt Type next token might match
    @return bool 
*/ 
bool Parser::nextIs (tokenType tt) {
    return currToken->terminal == tt ;
}

/*! \brief Moves to next token in linked-list 

    Used to explicitly move to the next token
    without checking anything.

    @return void 
*/ 
void Parser::nextToken () {
    if ( currToken == NULL ) 
        throw ( string("Internal Error: should not call nextToken in unitialized state"));
    else 
    if (currToken->terminal == endOfFile && currToken->next == NULL) {
        prevToken = currToken ;
    } else if (currToken->terminal != endOfFile && currToken->next == NULL) {
        throw ( makeErrorMsg ( "Error: tokens end with endOfFile" ) ) ;
    } else {
        prevToken = currToken ;
        currToken = currToken->next ;
    }
}

/*! \brief Retrieves the description for a tokenType

    @param terminal tokenType of which we want to know the description
    @return string 
*/ 
string Parser::terminalDescription ( tokenType terminal ) {
    Token *dummyToken = new Token ("",terminal, NULL) ;
    ExtToken *dummyExtToken = extendToken (this, dummyToken) ;
    string s = dummyExtToken->description() ;
    delete dummyToken ;
    delete dummyExtToken ;
    return s ;
}

/*! \brief Creates an expected error message

    Used to specify that the program expected a value,
    yet received something else. It then is able to 
    show the user where the program is going wrong.

    @param terminal Type expected
    @return string 
*/ 
string Parser::makeErrorMsgExpected ( tokenType terminal ) {
    string s = (string) "Expected " + terminalDescription (terminal) +
        " but found " + currToken->description() ;
    return s ;
}

/*! \brief Makes error message based on tokenType

    Used to show user that an unexpected symbol was found
    while parsing.

    @param terminal Type next token might match
    @return string 
*/ 
string Parser::makeErrorMsg ( tokenType terminal ) {
    string s = "Unexpected symbol " + terminalDescription (terminal) ;
    return s ;
}

/*! \brief Makes programmer-defined error message

    Based on text input, creates an error message that
    is that message.

    @param msg Error message
    @return string 
*/ 
string Parser::makeErrorMsg ( const char *msg ) {
    return msg ;
}

