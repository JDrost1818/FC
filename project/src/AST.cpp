#include "AST.h"

using namespace std ;

string Root::unparse() {
	return varName->unparse() + " () {\n" + stmts->unparse() + "}\n";
}

string Root::cppCode() {
	string s = "#include <iostream>\n#include \"Matrix.h\"\n#include <math.h>\nusing namespace std;\n\n";
	s += varName->cppCode() + " () {\n" + stmts->cppCode() + "}\n";
	return s;
}
////////////////////////////////////////////////
//
//	STATEMENTS CLASS AND DERIVATES
//
////////////////////////////////////////////////

string EmptyStmts::unparse() {
	return "";
}

string EmptyStmts::cppCode() {
	return "";
}

string StmtStmts::unparse() {
	return stmt->unparse() + stmts->unparse();
}

string StmtStmts::cppCode() {
	return stmt->cppCode() + stmts->cppCode();
}

////////////////////////////////////////////////
//
//	STATEMENT CLASS AND DERIVATES
//
////////////////////////////////////////////////

string StandardDecl::unparse() {
	return typeKeyword + " " + varName->unparse() + ";\n";
}

string StandardDecl::cppCode() {
	// It would be easier if there were an easy method
	// to lowercase a string, but this is simple enought
	// and also handles Str -> string. So oh well.
	if (typeKeyword == "Int") {
		return "int " + varName->cppCode() + ";\n";
	} else if (typeKeyword == "Float") {
		return "float " + varName->cppCode() + ";\n";
	} else if (typeKeyword == "Str") {
		return "string " + varName->cppCode() + ";\n";
	} else if (typeKeyword == "Bool") {
		return "bool " + varName->cppCode() + ";\n";
	} else {
		return "Unsupported Type";
	}
}

string MatrixAdvDecl::unparse() {
	string s = "";
	s += "Matrix " + varName1->unparse() + " [ " + expr1->unparse() + "," + expr2->unparse() + "] ";
	s += varName2->unparse() + " , " + varName3->unparse() + " = " + expr3->unparse() + ";\n";
	return s;
}

string MatrixAdvDecl::cppCode() {
	string returnString = "Matrix " + varName1->cppCode() + "(" + expr1->cppCode() + "," + expr2->cppCode() + ");\n";
	returnString += "for (int " + varName2->cppCode() + " = 0; " + varName2->cppCode() + " < " + expr1->cppCode() + "; " + varName2->cppCode() + "++ ) {\n";
    returnString += "\tfor (int " + varName3->cppCode() + " = 0; " + varName3->cppCode() + " < " + expr2->cppCode() + "; " + varName3->cppCode() + "++ ) {\n";
    returnString += "\t\t*(" + varName1->cppCode() + ".access(" + varName2->cppCode() + ", " + varName3->cppCode() + ")) = " + expr3->cppCode() + ";\n";
    returnString += "\t}\n";
    returnString += "}\n";
    return returnString;
}

string MatrixDecl::unparse() {
	return "Matrix " + varName->unparse() + " = " + expr->unparse() + ";\n";
}

string MatrixDecl::cppCode() {
	// Need to create a matrix class before this can be done
	return "Matrix " + varName->cppCode() + " = Matrix::"  + expr->cppCode() + ";\n"; 
}

string StmtBlock::unparse() {
	return "{\n" + stmts->unparse() + "}";
}

string StmtBlock::cppCode() {
	return "{\n" + stmts->cppCode() + "}";
}

string IfStmt::unparse() {
	return "if ( " + expr->unparse() + " ) " + stmt->unparse();
}

string IfStmt::cppCode() {
	return "if ( " + expr->cppCode() + " ) " + stmt->cppCode();
}

string IfElseStmt::unparse() {
	string s = "";
	s+= "if (" + expr->unparse() + ") " + stmt1->unparse();
	s+= " else " + stmt2->unparse() + "\n";
	return s;
}

string IfElseStmt::cppCode() {
	string s = "";
	s+= "if (" + expr->cppCode() + ") " + stmt1->cppCode();
	s+= " else " + stmt2->cppCode() + "\n";
	return s;
}

string StandardAssignStmt::unparse() {
	return varName->unparse() + " = " + expr->unparse() + ";\n";
}

string StandardAssignStmt::cppCode() {
	return varName->cppCode() + " = " + expr->cppCode() + ";\n";
}

string MatrixAssignStmt::unparse() {
	return varName->unparse() + "[" + expr1->unparse() + "," + expr2->unparse() + "] = " + expr3->unparse() + ";\n" ;
}

string MatrixAssignStmt::cppCode() {
	return "*(" + varName->cppCode() + ".access(" + expr1->cppCode() + ", " + expr2->cppCode() + ")) = " + expr3->cppCode() + ";\n";
}

string PrintStmt::unparse() {
	return "print(" + expr->unparse() + ");\n";
}

string PrintStmt::cppCode() {
	return "cout << " + expr->cppCode() + ";\n";
}

string ForStmt::unparse() {
	return "for(" + varName->unparse() + "=" + expr1->unparse() + " : " + expr2->unparse() + ")" + stmt->unparse() + "\n";
}

string ForStmt::cppCode() {
	string returnString = "for(" + varName->cppCode() + "=" + expr1->cppCode() + "; " + varName->cppCode();
	returnString += " <= " + expr2->cppCode() + "; " + varName->cppCode() + " ++)" + stmt->cppCode() + "\n";
	return returnString;
}

string WhileStmt::unparse() {
	return "while(" + expr->unparse() + ")" + stmt->unparse() + "\n";
}

string WhileStmt::cppCode() {
	return "while(" + expr->cppCode() + ")" + stmt->cppCode() + "\n";
}

////////////////////////////////////////////////
//
//	EXPR CLASS AND DERIVATES
//
////////////////////////////////////////////////

string VarName::unparse() { 
	return lexeme;
}

string VarName::cppCode() { 
	return lexeme;
}

string AnyConst::unparse() {
	return constString;
}

string AnyConst::cppCode() {
	return constString;
}

string BinOpExpr::unparse() {
	return left->unparse() + " " + op + " " + right->unparse();
}

string BinOpExpr::cppCode() {
	return left->cppCode() + " " + op + " " + right->cppCode();
}
	
string MatrixRefExpr::unparse() {
	return varName->unparse() + "[" + expr1->unparse() + "," + expr2->unparse() + "]";
}

string MatrixRefExpr::cppCode() {
	return "*(" + varName->cppCode() + ".access(" + expr1->cppCode() + "," + expr2->cppCode() + "))";
}

string FunctionCall::unparse() {
	return varName->unparse() + "(" + expr->unparse() + ")";
}

string FunctionCall::cppCode() {
	string args = expr->cppCode();
	if (args == "data") {
		return expr->cppCode() + "." + varName->cppCode() + "()";
	} else {
		return varName->cppCode() + "(" + expr->cppCode() + ")";
	}
}

string ParensExpr::unparse() {
	return "(" + expr->unparse() + ")";
}

string ParensExpr::cppCode() {
	return "(" + expr->cppCode() + ")";
}

string LetExpr::unparse() {
	return "let " + stmts->unparse() + " in " + expr->unparse() + " end ";
}

string LetExpr::cppCode() {
	return "({ " + stmts->cppCode() + expr->cppCode() + ";\n})";
}

string IfExpr::unparse() {
	return "if " + expr1->unparse() + " then " + expr2->unparse() + " else " + expr3->unparse();
}

string IfExpr::cppCode() {
	// C++ doesn't have if then else, so just using IfElseStmt::cppCode() 
	return "(" + expr1->cppCode() + ") ? " + expr2->cppCode() + " : " + expr3->cppCode() + "\n";
}

string NotExpr::unparse() {
	return "!" + expr->unparse();
}

string NotExpr::cppCode() {
	return "!" + expr->cppCode();
}
