#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <vector>
using namespace std;

namespace llm {
class SparseMatrixElement
{
public:
	int index;
	float value;
	SparseMatrixElement() {index=0; value=0;}
	SparseMatrixElement(int ind, float val) {index=ind; value=val;}
};

class SparseMat {
public:
	vector< vector<SparseMatrixElement> > s;
  int m;        // number of rows
  int n;        // number of cols
	int numNonzeros;
	SparseMat() {numNonzeros=0;}
	SparseMat(int m, int n):m(m),n(n),numNonzeros(0){s.resize(m);}
	void insert(float& val, Subscript i, Subscript j) {
		SparseMatrixElement SME(j,val);
		s[i].push_back(SME);
	}
	void increase(float val, int i, int j) {
		//first search if (i,j) exists in the current sparse matrix
		for (int jj=0; jj<s[i].size(); ++jj) 
			//if exists, increase the value
			if (s[i][jj].index==j) {
				s[i][jj].value+=val;
				return;
			}
		//if not exists, insert it
		insert(val, i, j);
	}
	float get(int i, int j) {
		for (int jj=0; jj<s[i].size();++jj)
			if (s[i][jj].index==j) return s[i][jj].value;
		return 0;
	}
};
};

#endif