#include "Matrix.h"
#include <sstream>
#include "../src/readInput.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stdlib.h>


using namespace std;

Matrix::Matrix(int i, int j){
	rows = i;
	cols = j;
    data = (float*) malloc(sizeof(float) * i * j);
	if(!data){
		cout << "Error: malloc didn't do it's job correctly.";
	}
}


Matrix::Matrix(const Matrix& m){
	rows = m.rows;
	cols = m.cols;
    data = m.data;
}
int Matrix::numRows(){
	return rows;
}


int Matrix::numCols(){
	return cols;
}


float *Matrix::access(const int i, const int j) const {
	return data + ((i*cols) + j);
}


std::ostream& operator<<(std::ostream &os, Matrix &m){
	os << m.numRows() << " " << m.numCols();
	for(int i = 0; i < m.numRows(); i++){
		os << "\n";
		for(int j = 0; j < m.numCols(); j++){
			os << *(m.access(i, j)) << "  ";
		}
	}
	os << "\n";
	return os;
}


/*Format of readMatrix
 read in the rows and columns
 */
Matrix Matrix::readMatrix(std::string filename){
int count = 0, data;
	std::string line;
    std::vector<int> matrix;
    ifstream file(filename.c_str());
    
    if (!file.is_open()) {
		cout << "Error reading file" << endl;
	}
    while (!file.eof()) {
        std::getline(file, line);
        std::stringstream ss(line);
        while (ss >> data) {
            matrix.push_back(data);
		}
    }
    file.close();
    
    Matrix* m = new Matrix( matrix[0], matrix[1] );
    matrix.erase( matrix.begin(), matrix.begin()+2 );
	for (int i = 0; i < m->numRows(); i++){
		for (int j = 0; j < m->numCols(); j++){
			*(m->access(i,j))= (float)(matrix.operator[](count));
			count++;
		}
	}
    
	return *m;
}



