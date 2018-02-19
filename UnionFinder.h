#ifndef UNIONFINDER_H_
#define UNIONFINDER_H_

#include <vector>
using namespace std;

class pairInd {
public:
  int sInd, tInd;
};

class Group {
public:
	int size;
	int* id;
  Group() {size=0;}
	Group(int* ind, int size) {
		this->size = size;
		id = new int[size];
		for (int i=0; i<size; ++i) id[i] = ind[i];
	}
	Group(int ind) {
		size = 1;
		id = new int[1];
		id[0] = ind;
	}
	void add(int ind) {
		size++;
		int* temp = new int[size];
    int i;
		for (i=0; i<size-1; ++i) temp[i] = id[i];
		id = new int[size];
		for (i=0; i<size-1; ++i) id[i] = temp[i];
		id[size-1] = ind;
	}
};

class UnionFinder {
  public: int n;
    int* ancestor;
    int* id;
    int* sz;
    int* groupID;
	  int groupNum;
	  Group* gps;
  UnionFinder(int* Ind, int size) {
    n = size;
	  id = new int[n];
    sz = new int[n];
	  ancestor = new int[n];
    groupID = new int[n];
    for(int i=0; i<n; ++i) {
	    ancestor[i] = i;
	    id[i] = Ind[i];
      sz[i] = 1;
    }
	  groupNum = n;
  }
  // path compression during find
  int find(int i) { 
    while (i!=ancestor[i]) {
      ancestor[i] = ancestor[ancestor[i]];
      i = ancestor[i];
    }
    return i;
  }
  void join(int u, int v) { 
    if (ancestor[u] == ancestor[v]) return;
    int i;
    if (sz[u]<sz[v]) {ancestor[find(u)] = find(v); sz[v]+=sz[u];}
    else             {ancestor[find(v)] = find(u); sz[u]+=sz[v];}
    for (i=0; i<n; ++i) find(i);
    for (i=0; i<n; ++i) printf("%d ",ancestor[i]); printf("\n");
    groupNum--;
  }
  void genGroups() {
    int i,j;
    for (i=0; i<n; ++i) find(i);
    //for (i=0; i<n; ++i) printf("%d ",ancestor[i]); printf("\n");
    //printf("There are %d groups\n", groupNum);
	  gps = new Group[groupNum];
    int currentGp = 0;
    gps[0] = Group(id[0]);
    groupID[0] = 0;
    for (i=1; i<n; ++i) {
      bool isNewGroup = true;
      for (j=0; j<i; ++j) {
        if (ancestor[i]==ancestor[j]) {
          isNewGroup = false;
          groupID[i] = groupID[j];
          gps[groupID[i]].add(id[i]);
          break;
        }
      }
      if (isNewGroup) {
        currentGp++;
        gps[currentGp] = Group(id[i]);
        groupID[i] = currentGp;
      }
    }
  }
};

#endif /*UNIONFINDER_H_*/