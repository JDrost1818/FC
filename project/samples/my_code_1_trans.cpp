#include <iostream>
#include "Matrix.h"
#include <math.h>
using namespace std;

main () {
  Matrix x1 = Matrix::readMatrix("../samples/my_code_1.data");
  Matrix x2 = Matrix::readMatrix("../samples/my_code_2.data");
  Matrix x3(5,5);
  for (int i = 0; i < 5; i++ ) {
    for (int j = 0; j < 5; j++ ) {
      *(x3.access(i, j)) = i * j;
    }
  }
  int i;
  int j;
  int k;
  int total;
  for(i=0; i <= 4; i ++){
    for(j=0; j <= 4; j ++){
      total = 0;
      for(k=0; k <= 4; k ++){
	total = total + (*(x1.access(k,i)) * *(x2.access(j,k)));
      }
      *(x3.access(i, j)) = total;
    }
  }
  cout << x3;
}
