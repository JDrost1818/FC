// Root
main(){
	// StandardDecl and StmtStmts
	Int x;

	// Matrix Advanced Decl, AnyConst. LetExpr, and MatrixAssignStmt
	Matrix long_matrix [10,10] mi, mj =
  let x = 7 ;
      Matrix n [10,10] ni, ni = mi + mj ;
  in  n [mi, mj] * 2 end ;

	// Matrix Decl
	Matrix short_matrix = 1;

	// IfStmt, StmtBlock, and PrintStmt
	if (True) {
		print("Hello World");
	}

	// IfElseStmt, BinOpExpr, VarName, StandardAssignStmt, and MatrixRefExpr
	if (1 < 2) {
		print("Duh");
	} else {
		//x = short_matrix[0, 0];
	}


  // WhileStmt, NotExpr, and ParensExpr
  while (!(i > 10)) {
	  print("This is an infinite loop!!!");
  }

	// ForStmt, 
	for ( x = 1 : 10 ) {
    print ("I Did It Ma!");
  }

  // EmptyStmts
}
