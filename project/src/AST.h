#ifndef Node_H
#define Node_H

#include <string>
#include <iostream> 

#include "scanner.h"

// Abstract Classes
class Node ;
class Root ;
class Stmts ;
class Stmt ;
class Expr ;

// Stmts Derivatives
class EmptyStmts ;
class StmtStmts ;

// Stmt Derivatives
class StandardDecl ;
class MatrixAdvancedDecl ;
class MatrixDecl ;
class StmtBlock ;
class IfStmt ;
class IfElseStmt ;
class StandardAssignStmt ;
class MatrixAssignStmt ;
class PrintStmt ;
class ForStmt ;
class WhileStmt ;

// Expr Derivatives
class VarName ;
class AnyConst ;
class BinOpExpr ;
class MatrixRefExpr ;
class FunctionCall ;
class ParensExpr ;
class LetExpr ;
class IfExpr ;
class NotExpr ;

/*! \class Node
	\brief Abstract parent or grandparent for all classes in the Abstract Syntax Tree (AST)
*/
class Node {
public:
	virtual std::string unparse ( ) = 0 ;
	virtual std::string cppCode ( ) = 0 ;
	virtual ~Node() { } ;
} ;

/*! \class Root
 *	\brief Handles initial function declaration.
 *
 *	Model: Root ::= varName '(' ')' '{' Stmts '}' <BR>
 *	Example: main(){}
 */
class Root : public Node {
public:
	/*! Public constructor.
	    @param v - Name of root
	    @param s - Stmts in body of root
	*/
	Root(VarName *v, Stmts *s) : varName(v),stmts(s){};

	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();

	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();
private:
	VarName *varName;
	Stmts *stmts;
};


/*! \class Stmts
 *	\brief Parent for all statements to be parsed
 */
class Stmts : public Node {};

/*! \class EmptyStmts
 	\brief Represents emptiness
 
	Model: Stmts ::= <<empty>> <BR>
	Example: <<emptiness>>
*/
class EmptyStmts : public Stmts {
public:
	/*! Public constructor. 
	*/	
	EmptyStmts(){};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();
};

/*! \class StmtStmts 
    \brief This provides a gateway to stmt objects. Without this, there would only be  <BR>
		   Stmts and no Stmt would be able to be parsed.

	MODEL: Stmts ::= Stmt Stmts <BR>
	Example: x = 1; <<empty>>
*/
class StmtStmts : public Stmts {
public:
	/*! Public constructor.
	    @param _stmt  - Single statement
	    @param _stmts - Further statements
	*/
	StmtStmts(Stmt *_stmt, Stmts *_stmts) : stmt(_stmt), stmts(_stmts) {};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
	Stmt *stmt;
	Stmts *stmts;
};
		

/*! \class Stmt 
    \brief Parent for all statment instances to be parsed <BR>
*/	
class Stmt : public Node {};

/*! \class StandardDecl
    \brief Represents a declaration of a variable <BR>

	Model: Decl ::= Keyword varName ';' <BR>
	Example: Int x;
*/
class StandardDecl : public Stmt {
public:
	/*! Public constructor.
	    @param tKwd - Keyword ie "Int", "Float", etc
	    @param v - Name of variable being declared
	*/
	StandardDecl(std::string tKwd, VarName *v) : typeKeyword(tKwd), varName(v) {}; 
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
	std::string typeKeyword;
	VarName *varName;
};

/*! \class MatrixAdvDecl 
    \brief Represents a declaration of a matrix with initial values. <BR>

	Model: Decl ::= 'Matrix' varName '[' Expr ',' Expr ']' varName ',' varName  '=' Expr ';' <BR>
	Example: Matrix n[10,10] ni, ni = mi + mj;
*/
class MatrixAdvDecl : public Stmt {
public:
	/*! Public constructor. 
	    @param v1 - Name of matrix being constructed
	    @param v2 - First index variable name
	    @param v3 - Second index variable name
	    @param e1 - Row index expression
	    @param e2 - Col index expression
	    @param e3 - Expr for value to  be assigned
	*/
	MatrixAdvDecl(VarName* v1, VarName* v2, VarName* v3, Expr* e1, Expr* e2, Expr* e3) 
	: varName1(v1), varName2(v2), varName3(v3), expr1(e1), expr2(e2), expr3(e3) {}; 
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
	VarName *varName1;
	VarName *varName2;
	VarName *varName3;
	Expr* expr1;
	Expr* expr2;
	Expr* expr3;
};

/*! \class MatrixDecl 
    \brief Represents a standard declaration of a matrix. <BR>

	Model: Decl ::= 'Matrix' varName '=' Expr ';' <BR>
	Example: Matrix new_matrix = 10;
*/
class MatrixDecl : public Stmt {
public:
	/*! Public constructor. 
	    @param v - Name of matrix being constructed
	    @param e - Expr of value being assigned
	*/
	MatrixDecl(VarName* v, Expr* e) : varName(v), expr(e) {}; 
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
	VarName *varName;
	Expr* expr;
};

/*! \class StmtBlock 
    \brief Represents a stmt which contains a block of stmts. <BR>

	Model: Stmt ::= '{' Stmts '}' <BR>
	Example: { int x; <<empty>> }
*/
class StmtBlock: public Stmt{
public:
	/*! Public constructor. 
	    @param v - Name of matrix being constructed
	    @param e - Expr of value being assigned
	*/
    StmtBlock(Stmts* s): stmts(s) {};
    /** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
    std::string unparse();
    /** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
    Stmts* stmts;
} ;

/*! \class IfStmt 
    \brief Represents an if statement. <BR>

	Model: Stmt ::= 'if' '(' Expr ')' Stmt <BR>
	Example: if (True) { x += 1; }
*/
class IfStmt : public Stmt {
public:
	/*! Public constructor. 
	    @param e - Boolean expression that is evaluated by if
	    @param s - Statement to be executed if true
	*/
	IfStmt(Expr* e, Stmt* s) : expr(e), stmt(s) {};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
	Expr* expr;
	Stmt* stmt;
};

/*! \class IfElseStmt 
    \brief Represents an if-else statement. <BR>

	Model: Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt <BR>
	Example: if (True) { x += 1; } else { x -= 1; }
*/
class IfElseStmt : public Stmt {
public:
	/*! Public constructor. 
	    @param e - Boolean expression that is evaluated by if
	    @param s1 - Statement to be executed if true
	    @param s2 - Statement to be executed if false
	*/
	IfElseStmt(Expr* e, Stmt* s1, Stmt* s2) : expr(e), stmt1(s1), stmt2(s2) {};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
	Expr* expr;
	Stmt* stmt1;
	Stmt* stmt2;
};

/*! \class StandardAssignStmt 
    \brief  Represents a standard variable assignment. <BR>

	Model: Stmt ::= varName '=' Expr ';' <BR>
	Example: x = 1;
*/
class StandardAssignStmt : public Stmt {
public:
	/*! Public constructor.
	    @param v - Name of variable being assigned
	    @param e - Expression of the value to be assigned
	*/
	StandardAssignStmt(VarName* v, Expr* e) : varName(v), expr(e) {};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();
private:
	VarName* varName;
	Expr* expr;
};

/*! \class MatrixAssignStmt 
    \brief Represents a matrix assignment. <BR>

	Model: varName '[' Expr ',' Expr ']' '=' Expr ';' <BR>
	Example: matrix[0,0] = 8;
*/
class MatrixAssignStmt : public Stmt {
public:
	/*! Public constructor.
	    @param v - Name of matrix being assigned
	    @param e1 - Expression of the value of the row to be assigned
	    @param e2 - Expression of the value to the col to be assigned
	    @param e3 - Expression of the value to be assigned
	*/
	MatrixAssignStmt(VarName* v, Expr* e1, Expr* e2, Expr* e3) 
	: varName(v), expr1(e1), expr2(e2), expr3(e3) {};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
	VarName* varName;
	Expr* expr1;
	Expr* expr2;
	Expr* expr3;
};

/*! \class PrintStmt 
    \brief Represents a print statement. <BR>

	Model: Stmt ::= 'print' '(' Expr ')' ';' <BR>
	Example: print("Hello World");
*/class PrintStmt : public Stmt{
public:
	/*! Public constructor. 
	    @param e - Expression of what is to be printed
	*/
	PrintStmt(Expr* e) : expr(e) {};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
	Expr* expr;
};

/*! \class ForStmt 
    \brief Represents a for-loop statement. <BR>

	Model: Stmt ::= 'for' '(' varName '=' Expr ':' Expr ')' Stmt <BR>
	Example: for (i = 1: 10) { print(i); }
*/class ForStmt : public Stmt{
public:
	/*! Public constructor. 		
	    @param v  - Variable that will change each time through the loop
	    @param e1 - Original value given to the variable on first iteration
	    @param e2 - Expression of the stopping value of the variable through the iteration
	    @param s  - Expression to be executed for each iteration
	*/
    ForStmt(VarName* v, Expr* e1, Expr* e2, Stmt* s)
    : varName(v), expr1(e1), expr2(e2), stmt(s) {};
    /** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
    std::string unparse();
    /** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
    VarName* varName;
    Expr* expr1;
    Expr* expr2;
    Stmt* stmt;
};

/*! \class WhileStmt 
    \brief Represents a while-loop statement. <BR>

	Model: Stmt ::= 'while' '(' Expr ')' Stmt <BR>
	Example: while (true) { print("This is an infinite loop!!!"); }
*/
class WhileStmt : public Stmt {
public:
	/*! Public constructor. 		
	    @param e - Expression to be evaluated for each iteration
	    @param s - Expression to be executed for each iteration
	*/
	WhileStmt(Expr* e, Stmt* s) : expr(e), stmt(s) {};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
	Expr* expr;
	Stmt* stmt;
};

/*! \class Expr 
    \brief Parent for all expression instances to be parsed  <BR>

	Model: Expr ::= varName <BR>
	Example: x
*/	
class Expr : public Node {};

/*! \class VarName 
   \brief Represents a variable name. <BR>

	Model: Expr ::= varName <BR>
	Example: x
*/	
class VarName : public Expr {
public:
	/*! Public constructor. 
	    @param s - Name of variable
	*/
	VarName(std::string s) : lexeme(s) {};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();
private:
	std::string lexeme;
};

/*! \class AnyConst 
    \brief Represents a constant - of any type (string, int, float, etc...). <BR>

	Model: Expr ::= const <BR>
	Example: 100
*/
class AnyConst : public Expr {
public:
	/*! Public constructor. 
	    @param s - Value of the constant
	*/
	AnyConst(std::string s) :constString(s){};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();
private:
	std::string constString;
};

/*! \class BinOpExpr 
    \brief Represents two expressions separated by an operation. <BR>

	Model: Expr ::= Expr *op* Expr  <BR>
	Example: 1 + 1
*/
class BinOpExpr : public Expr {
public:
	/*! Public constructor. 
	    @param L  - Expression on left-hand side of operation
	    @param op - Operation splitting the left and right expressions
	    @param R  - Expression on right-hand side of operation
	*/
	BinOpExpr(Expr* L, std::string op, Expr* R)
	: left(L), op(op), right(R) {} ;
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();
private:
	Expr *left;
	std::string op;
	Expr *right;
};

/*! \class MatrixRefExpr 
    \brief Represents accessing a matrix. <BR>

	Model: Expr ::= varName '[' Expr ',' Expr ']' <BR>
	Example: matrix[0.0]
*/
class MatrixRefExpr : public Expr {
public:
	/*! Public constructor. 
	    @param v  - Name of matrix being accessed
	    @param e1 - Expression of row being accessed
	    @param e2 - Expression of col being accessed
	*/
	MatrixRefExpr(VarName* v, Expr* e1, Expr* e2) : varName(v), expr1(e1), expr2(e2) {};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
	VarName* varName;
	Expr* expr1;
	Expr* expr2;	
};

/*! \class FunctionCall 
    \brief Represents a function call. <BR>

	Model: Expr ::= varName '(' Expr ')' <BR>
	Example: main(1)
*/
class FunctionCall : public Expr {
public:
	/*! Public constructor. 
	    @param v  - Name of function being called
	    @param e  - Expression of arguments
	*/
	FunctionCall(VarName* v, Expr* e) : varName(v), expr(e) {};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
	VarName* varName;
	Expr* expr;
};

/*! \class ParensExpr 
    \brief Represents an expression surrounded by parenthesis. <BR>

	Model: Expr ::= '(' Expr ')'  <BR>
	Example: (1 + 1)
*/
class ParensExpr : public Expr {
public:
	/*! Public constructor. 
	    @param e  - Expression evaluated inside the parenthesis
	*/
	ParensExpr(Expr* e) : expr(e) {};
	/** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
	std::string unparse();
	/** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
	Expr* expr;
};

/*! \class LetExpr 
    \brief Represents a let expression. <BR>

	Model: Expr ::= 'let' Stmts 'in' Expr 'end' <BR>
	Example: let x = 7;
*/
class LetExpr : public Expr{
public:
	/*! Public constructor. 
		@param s - Statement to be considered
	    @param e - Expression to be evaluated
	*/
    LetExpr(Stmts* s, Expr* e) : stmts(s), expr(e) {};
    /** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
    std::string unparse();
    /** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
    Stmts* stmts;
    Expr* expr;
};

/*! \class IfExpr 
    \brief Represents if-then-else expression <BR>

	Model: Expr ::= 'if' Expr 'then' Expr 'else' Expr <BR>
	Example: if True then print("Hello World"); else print("This will never be read!!!");
*/
class IfExpr: public Expr{
public:
	/*! Public constructor.
		@param e1 - Boolean expression to be considered
	    @param e2 - Expression to be evaluated if true
	    @param e3 - Expression to be evaluated if false
	*/
    IfExpr(Expr* e1, Expr* e2, Expr* e3) : expr1(e1), expr2(e2), expr3(e3) {};
    /** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
    std::string unparse();
    /** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
    Expr* expr1;
    Expr* expr2;
    Expr* expr3;
};

/*! \class NotExpr 
    \brief Represents negation of a boolean <BR>
    
	Model: Expr ::= '!' Expr  <BR>
	Example: !true
*/
class NotExpr: public Expr{
public:
	/*! Public constructor. 
		@param e - Boolean expression being negated 
	*/
    NotExpr(Expr* e) : expr(e) {};
    /** @brief Returns a string representation of the code
	 *         modeled by this class and all its variables.
	 *  @return std::string.
	 */
    std::string unparse();
    /** @brief Returns a string representation of the FCAL
	 *  	   code in C++ code.
	 *	@return std::string.
	 */
	 std::string cppCode();

private:
    Expr* expr;
};

#endif

