#include <string>
#include <iostream>
#include <vector>
using namespace std;

struct v {
	int id;
	int x;				
	int y;
	vector<v*> adjacent;	// adjacent nodes, the first element is a pointer to the parent. 
	int key;				// distance from previous (cost)
	bool _isInQueue;		
	bool _isInMST;
	bool _isInPM;
};

int readData(vector<v*> &, string);					// return number of data read
int** buildMatrix(vector<v*>, int);
void buildMST(vector<v*> &, int ** , int);
void _removeLink(vector<v*> &, v*);
void _vectorResize(vector<v*> &, int);
void _vectorTrim(vector<v*> &, int);
v* _getMinKey(vector<v*> &);
void _addToQueue(vector<v*> &, v*);
void _printV(vector<v*>);
void _printThisV(v*);
void _printQ(vector<v*>);
void _printDistanceMatrix(int **, int);
void printMSTLinklist(vector<v*>, int);
void printMSTMatrix(vector<v*>, int);
void printMSTMatrix(vector<v*>, int, string);
