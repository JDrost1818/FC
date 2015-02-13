#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include <stdio.h>
using namespace std ;

class ScannerTestSuite : public CxxTest::TestSuite 
{
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    Scanner *s ;
    void test_setup_code ( ) {
        s = new Scanner() ;
    }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You may need to write several tests to adequately test the
       functions that are called from "scan".  Once you are confident
       that the components used by "scan" work properly, then you can
       run tests on the "scan" method itself.
    */

    // Here you should place tests for these other functions.


    /* Below is one of the tests for these components in the project
       solution created by your instructor..  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly ( ) {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{" ) ;
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */

    void tokenMaker_tester(const char* sentence, tokenType assert_type, string assert_value){
        /* Since regex has it's own set of tests, we don't need to test 
           its functionality here, just the token part.  
        */
        Token *newToken = s->matchNextToken(sentence) ;
        TS_ASSERT_EQUALS(newToken->terminal, assert_type) ;
		TS_ASSERT_EQUALS(newToken->lexeme, assert_value) ;
    }

    void test_TokenMaker_intKwd(){
        tokenMaker_tester("Int ", intKwd, "Int") ;
    }
    void test_TokenMaker_floatKwd(){
        tokenMaker_tester("Float ", floatKwd, "Float") ;
    }
    void test_TokenMaker_boolKwd(){
        tokenMaker_tester("Bool ", boolKwd, "Bool") ;
    }
    void test_TokenMaker_trueKwd(){
        tokenMaker_tester("True ", trueKwd, "True") ;
    }
    void test_TokenMaker_falseKwd(){
        tokenMaker_tester("False ", falseKwd, "False") ;
    }
    void test_TokenMaker_stringKwd(){
        tokenMaker_tester("Str ", stringKwd, "Str") ;
    }
    void test_TokenMaker_matrixKwd(){
        tokenMaker_tester("Matrix ", matrixKwd, "Matrix") ;
    }
    void test_TokenMaker_letKwd(){
        tokenMaker_tester("let ", letKwd, "let") ;
    }
    void test_TokenMaker_inKwd(){
        tokenMaker_tester("in ", inKwd, "in") ;
    }
    void test_TokenMaker_endKwd(){
        tokenMaker_tester("end ", endKwd, "end") ;
    }
    void test_TokenMaker_ifKwd(){
        tokenMaker_tester("if ", ifKwd, "if") ;
    }
    void test_TokenMaker_thenKwd(){
        tokenMaker_tester("then ", thenKwd, "then") ;
    }
    void test_TokenMaker_elseKwd(){
        tokenMaker_tester("else ", elseKwd, "else") ;
    }
    void test_TokenMaker_forKwd(){
        tokenMaker_tester("for ", forKwd, "for") ;
    }
    void test_TokenMaker_whileKwd(){
        tokenMaker_tester("while ", whileKwd, "while") ;
    }
    void test_TokenMaker_printKwd(){
        tokenMaker_tester("print ", printKwd, "print") ;
    }
    void test_TokenMaker_intConst(){
        tokenMaker_tester("123 ", intConst, "123") ;
    }
    void test_TokenMaker_floatConst(){
        tokenMaker_tester("123.123 ", floatConst, "123.123") ;
    }
    void test_TokenMaker_stringConst(){
        tokenMaker_tester("\"string\" ", stringConst, "\"string\"") ;
    }
    void test_TokenMaker_variableName(){
        tokenMaker_tester("variable_name ", variableName, "variable_name") ;
    }
    void test_TokenMaker_leftParen(){
        tokenMaker_tester("( ", leftParen, "(") ;
    }
    void test_TokenMaker_rightParen(){
        tokenMaker_tester(") ", rightParen, ")") ;
    }
    void test_TokenMaker_leftCurly(){
        tokenMaker_tester("{ ", leftCurly, "{") ;
    }
    void test_TokenMaker_rightCurly(){
        tokenMaker_tester("} ", rightCurly, "}") ;
    }
    void test_TokenMaker_leftSquare(){
        tokenMaker_tester("[ ", leftSquare, "[") ;
    }
    void test_TokenMaker_rightSquare(){
        tokenMaker_tester("] ", rightSquare, "]") ;
    }
    void test_TokenMaker_comma(){
        tokenMaker_tester(", ", comma, ",") ;
    }
    void test_TokenMaker_semiColon(){
        tokenMaker_tester("; ", semiColon, ";") ;
    }
    void test_TokenMaker_colon(){
        tokenMaker_tester(": ", colon, ":") ;
    }
    void test_TokenMaker_assign(){
        tokenMaker_tester("= ", assign, "=") ;
    }
    void test_TokenMaker_plusSign(){
        tokenMaker_tester("+ ", plusSign, "+") ;
    }
    void test_TokenMaker_star(){
        tokenMaker_tester("* ", star, "*") ;
    }
    void test_TokenMaker_dash(){
        tokenMaker_tester("- ", dash, "-") ;
    }
    void test_TokenMaker_forwardSlash(){
        tokenMaker_tester("/ ", forwardSlash, "/") ;
    }
    void test_TokenMaker_lessThan(){
        tokenMaker_tester("< ", lessThan, "<") ;
    }
    void test_TokenMaker_lessThanEqual(){
        tokenMaker_tester("<= ", lessThanEqual, "<=") ;
    }
    void test_TokenMaker_greaterThan(){
        tokenMaker_tester("> ", greaterThan, ">") ;
    }
    void test_TokenMaker_greaterThanEqual(){
        tokenMaker_tester(">= ", greaterThanEqual, ">=") ;
    }
    void test_TokenMaker_equalsEquals(){
        tokenMaker_tester("== ", equalsEquals, "==") ;
    }
    void test_TokenMaker_notEquals(){
        tokenMaker_tester("!= ", notEquals, "!=") ;
    }
    void test_TokenMaker_andOp(){
        tokenMaker_tester("&& ", andOp, "&&") ;
    }
    void test_TokenMaker_orOp(){
        tokenMaker_tester("|| ", orOp, "||") ;
    }
    void test_TokenMaker_notOp(){
        tokenMaker_tester("! ", notOp, "!") ;
    }
    void test_TokenMaker_lexicalError(){
        tokenMaker_tester("@ ", lexicalError, "@") ;
    }
    


    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the 
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (Token *tks) {
        Token *currentToken = tks ;
        while (currentToken != NULL) {
            if (currentToken->terminal == lexicalError) {
                printf("problem: %s\n",currentToken->lexeme.c_str());
				return false ;
            }
            else {
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned. 
    */
    void scanFileNoLexicalErrors ( const char* filename ) {
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT (noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
    bool sameTerminals (Token *tks, int numTerms, tokenType *ts) {
        Token *currentToken = tks ;
        int termIndex = 0 ;
        while (currentToken != NULL && termIndex < numTerms ) {
            if (currentToken->terminal != ts[termIndex]) {
            printf("%i: %i should be %i\n",termIndex,currentToken->terminal , ts[termIndex]);
			fflush(stdout);
                return false ;
            }
            else {
                ++ termIndex ;
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty ( ) {
        Token *tks = s->scan ("  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    void test_scan_empty_comment ( ) {
        Token *tks = s->scan (" /* a comment */ ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    // When a lexical error occurs, the scanner creates a token with a 
    // single-character lexeme and lexicalError as the terminal.
   void test_scan_lexicalErrors ( ) {
        Token *tks = s->scan ("$&1  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "$");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "&");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, intConst) ;
        TS_ASSERT_EQUALS (tks->lexeme, "1");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }


    // A test for scanning numbers and a variable.
    void test_scan_nums_vars ( ) {
        Token *tks = s->scan (" 123 x 12.34 ") ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, endOfFile } ;
        TS_ASSERT ( sameTerminals ( tks, 4, ts ) ) ;
    }



    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexeme
       are correct.
     */
    
    void test_scan_bad_syntax_good_tokens ( ) {
        const char *filename = "../samples/bad_syntax_good_tokens.dsl" ;
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { 
            intConst, intConst, intConst, intConst, 

            stringConst, stringConst, stringConst,

            floatConst, floatConst, floatConst,

                   matrixKwd,



            semiColon, comma, colon,
            leftCurly, leftParen, rightCurly, rightParen,

            plusSign, star, dash, forwardSlash, 

            equalsEquals, lessThanEqual, 
            greaterThanEqual, notEquals, 
            assign,

            variableName, variableName, variableName, variableName, 
            variableName, variableName, variableName,


            intKwd, floatKwd,  stringKwd,  whileKwd,

            endOfFile
       } ;
        int count = 39; 
        TS_ASSERT ( sameTerminals ( tks, count, ts ) ) ;
    }

    void test_scan_sample_forestLoss ( ) {
        scanFileNoLexicalErrors ("../samples/forest_loss_v2.dsl") ;
    }

      

} ;

