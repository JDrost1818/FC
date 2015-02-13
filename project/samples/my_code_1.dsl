/* This program is just a copy of sample_4.dsl. You should write your
   solution for my_code_1 in this file. */

main () { 
  Matrix x1 = readMatrix ( "../samples/my_code_1.data" ) ;
  Matrix x2 = readMatrix ( "../samples/my_code_2.data" ) ;
  Matrix x3 [ 5, 5] i, j = i * j ;
  Int i ;
  Int j ;
  Int k ;
  Int total ;
  for (i = 0 : 4 ) {
    for ( j = 0 : 4 ) {
      total = 0 ;
      for ( k = 0 : 4 ) {
        total = total + (x1[k,i] * x2[j,k]) ;
	      
      }
      x3[i,j] = total;
    }
  }
  print(x3) ;
}
  




