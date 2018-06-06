#include <string>
#include <iostream>
#include <vector>
using namespace std;

struct v {
	int id;
	int x;				
	int y;
	vector<v*> adjacent;	// adjacent nodes, the first element is a pointer to the parent. 
	int key;				// distance from prevoius (cost) 	
	bool _isInQueue;		
	bool _isInMST;
};

/*Public Variables*/
vector<int>circuit;
int pathLength;

/* Main Procedures */
int readData(vector<v*> &, string);					// Fill container with dynamically allocated vertices, store data, and return number of data read
int** buildMatrix(vector<v*>, int);					// Create and return a 2D int matrix filled with distances between cities
void buildMST(vector<v*> &, int ** , int);			// Create an MST by pointing verticies in the "adjacent" list of each vertex
void perfectMatching(vector<v*> &, int**);			// Create an perfectMatching graph and add it to the MST by add

/* Public Functions */
void printMSTLinklist(vector<v*>, int);				// Display MST in Link list format. The ID in [ ] is the parent. (Each vertex only has one parent)
void printMSTMatrix(vector<v*>, int);				// Print MST in Matrix format to screen
void printMSTMatrix(vector<v*>, int, string);		// Print MST in Matrix format to file 				(currently in use)
void clean_D(int ** &, int);						// Clean up Matrix 
void clean_V(vector<v*> &);							// Clean up Vertex struct

/* Private Utility Functions */
void _removeLink(vector<v*> &, v*);					// Remove certain child from the parent adjacent list, for updating min key occasions
v* _getMinKey(vector<v*> &);						// Pop the min key from queue, queue must be sorted.
void _addToQueue(vector<v*> &, v*);					// Add a vertex to queue, this keeps queue in sorted order, Max to Min
vector<v*> _makeOddsList(vector<v*> &);			// create list of vertex with odd degree (used by perfectMatching)

/* Helper Functions */
void _printV(vector<v*>);							// Print the whole vertex container in full detail
void _printThisV(v*);								// Print one vertex in full detail
void _printQ(vector<v*>);							// Print the vertex ID in Queue
void _printDistanceMatrix(int **, int);				// Print 2D int Matrix of Distance data
void _printDistanceMatrix(int **, int, string);		// Print 2D int Matrix of Distance data to file 	(currently in use)

/*Part 5 & 6 Functions*/
vector<int> euler(vector<v*>, int, vector<int>&);
void make_hamilton(vector<int> &, int &, int ** D);
int find_tour(vector<v*> V, int pos, int ** D);

/* Unused functions */
//void _vectorResize(vector<v*> &, int);
//void _vectorTrim(vector<v*> &, int);
