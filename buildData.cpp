#include "buildData.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stack>

using namespace std;
/***************************************************************
*
*	[Read Data]
*	Input: 	Container for vertices, input file name
*	Output:	Container filled with dynamically allocated 
*			vertices, returns the number data read
*
***************************************************************/
int readData(vector<v*> &V, string filename) {			// return number of data read

	int n = 0;											// number of data 
	string line;
	ifstream inputFile;
	inputFile.open(filename);		
	if (!inputFile)
	{	cout << "Error opening input file. " << endl; }
	else
	{
		while (!inputFile.eof())
		{		
			/* load line */
			getline(inputFile,line);

			if (line.length() > 0){
				
				/* set data */
				v* new_v = new v; 
				new_v->adjacent.resize(1);					// adjacent[0] is a pointer pointing to parent. default is pointing to NULL;
				new_v->adjacent[0] == NULL;
				if (new_v->adjacent.size() != 1) { cout << "ERROR!! Check struct construction process in readData()!" << endl; }
				new_v->key = numeric_limits<int>::max();	// set key value to infinity
				new_v->_isInQueue = false;					// this is not in Queue yet
				new_v->_isInMST = false;					// this is not in MST yet
				stringstream ss(line);						// Streaming method reference: https://stackoverflow.com/questions/16987461/extraction-of-integers-from-strings 
				
				/* reading fileds of each line into data */
				int arg = 0;								
				for (int i = 0; ss >> i; ) {
					switch (arg) {
						case 0:	new_v->id = i;				// first value is city id
								arg = 1;
								break; 
						case 1:	new_v->x = i;				// second value is coord-x
								arg = 2;
								break;
						case 2: new_v->y = i;				// third value is coord-y
								arg = 0;
								break;
						default: cout << "WARNING!! Something is off with reading data! Check switch statement." << endl;
								break;
					}
				}

				/* store to container V */
				V.push_back(new_v);
				n++;						// count input

				/*******************************************************
				*
				*	[dynamic resize]
				*	This part is for speed optimization. WIP.
				*
				*******************************************************/

				// if (n + 1 == V.capacity())
				// {
				// 	_vectorResize(V, n);
				// }
			}
		}
		/* close file */
		inputFile.clear();
		inputFile.close();
	}

	if (n < 1) 
		cout << "ERROR: readData() read no data." << endl;
	else 
		cout << "Read from \"" << filename << "\" | # of cities: [" << n << "]" << endl;
	
	// _vectorTrim(V, n);					// part of dynamic resizing, WIP
	return n;								// return the number of cities
}

/***************************************************************
*
*	[Build Matrix]
*	Input: 	Container of verticies, number of verticies
*	Output:	Returns a 2D int matrix filled with distances 
*			between cities
*
***************************************************************/
int** buildMatrix(vector<v*> V, int n)
{
	/* Dynamic 2D int array created */
	int ** D = new int* [n];

	/* Initialize */
	for (int r = 0; r < n; r++)
	{
		D[r] = NULL;
		D[r] = new int [n];
		for (int c = 0; c < n; c++)
		{
			D[r][c] = 0;
		}
	}

	cout << "/* Constructed D Matrix */" << endl;

	/* Fill with calculation data */
	for (int i = 0; i < n; i++)			// from each point i
	{							
		for (int j = 0; j < n; j++)		// to each point j
		{
			if (j == i)					
			{
				D[i][j] = 0;			// to and from the same point has no distance
			}
			else if (j < i)				// this has been previously calculated
			{
				D[i][j] = D [j][i];		// fetch the prevoius
			}
			else 						// this is a new case, calculate this one
			{		
				float x1 = (float)V[i]->x;
				float x2 = (float)V[j]->x;
				float y1 = (float)V[i]->y;
				float y2 = (float)V[j]->y;
				D[i][j] = round(sqrt(pow((x1 - x2),2) + pow((y1 - y2),2)));
			}
		}
	}

	cout << "/* Filled D Matrix */" << endl;

	return D;
}

/************************************************************************************
*
*	[Build MST]
*	Input: 	Container of verticies, 2D matrix of distances, number of verticies
*	Output:	Vertices in the container now has adjacent list filled.
*			Element [0] in each adjacent list is the parent vertex 
*			(where the spanning tree traverse from).
*			Key value of each vertex is also updated.
*			The value is the weight of the edge from its parent to itself.
*
************************************************************************************/
void buildMST(vector<v*> &V, int ** D, int n)
{
	/* Setup */
	int root = 0; 					// starting from city #0
	V[root]-> key = 0;				// zero distance starting point

	v* current = NULL;				// from this vertex
	v* target = NULL;				// to this vertex
	vector<v*> Q; Q.clear();		// queue for unhandled verticies, sorted from greatest to smallest by key value 

	int d = 0;						// holder variable for distance
	int count = 0;					// count how many verticies (cities) have been computed by MST

	/* Starting City */
	current = V[root];				// set the current pointer to the starting city
	current-> _isInMST = true;		// this city is now in the MST

	while (count != n)				// loop until every city is handled once
	{
		int i = current->id;				// current vertex i in V
		for (int j = 0; j < n; j++)			// all verticies j in V
		{
			target = V[j];							// points to this target j
			if (i != j)								// if this target is not the current city itself, then computer distance comparison
			{
				d = D[i][j];								// d holds distance from current i to target j
				if (target->_isInMST == false) 				// if this vertex hasn't been computed
				{
					if (d < target->key)						// if this is a shorter distance than target's current option,
					{
						target->key = d;							 //update the key in target because this is a shorter distance
						
						if (target->adjacent[0] != NULL)								// if there is some prevoius vertex pointing to
					 	{ _removeLink(target->adjacent[0]->adjacent, target); }			// remove target from previous' adjacent list
						target->adjacent[0] = current;									// set target adjacent[0] as current
						current->adjacent.push_back(target);							// add target to current adjacent list
					}
				}

				/* queue management */
				if (target->_isInQueue == false && target->_isInMST == false)			// if this target is explored the first time,
				{
					_addToQueue(Q, target);							// add target to the queue Q in sorted order
					//_printQ(Q);									// unblock to see the content of Q each time adding new vertex
				}
			}	
		}

		/* Prepare for next vertex */
		current->_isInMST = true;
		count ++;
		current = _getMinKey(Q);
		if (current == NULL && count != n)
		{ cout << "ERROR!! cannot get the next in queue! Check buildMST()" << endl; }

	}

	/* error checking */
	if (Q.size() != 0 || current != NULL)
	{ cout << "ERROR!! Something wrong with buildMST(), there is remaining vertices unhandled" << endl; }

	cout << "/* Constructed MST */" << endl;
}

/***************************************************************************
 [Perfect Match]
  Description: The prefectMatching portion of our modified Christofide's
  algorithm will take the odd vertices from the MST and build a perfect
  matching graph PM. This graph will be combined with MST to create an
  Eulerian circuit. In order increase the speed of our program
  this perfect matching algorithm will use a greedy approach.
***************************************************************************/
void perfectMatching(vector<v*> &V, int** D) {
//make odds list
	vector<v*> odds = _makeOddsList(V);
	//while oddsList is not empty
	while (!odds.empty()) {
		int distance = INT32_MAX;    //for comparing distances
		int closest; //for saving closest
		int i; //index of next vertex
		//loop through oddsList finding min distance
		for (i = 1; i < odds.size(); ++i){
			if (distance > D[odds.front()->id][odds[i]->id]){
				distance = D[odds.front()->id][odds[i]->id];
				closest = i;
			}
		}
		/*error checking */
		if(closest == NULL){
			cout << "ERROR!! perfectMatching function broken, no match could be found" << endl;
		}
		//update adj lists, update oddsList
		odds.front()->adjacent.push_back(odds[closest]);
		odds[closest]->adjacent.push_back(odds.front());
		odds.erase(odds.begin() + closest);
		odds.erase(odds.begin());
	}
	cout << "/* Constructed perfect match graph */" << endl;
}

/***************************************************************************
  [Get odd vertices]
  Description: The makeOddsList function will take the MST and return a
  list of all vertices of odd degree.
***************************************************************************/
vector<v*> _makeOddsList(vector<v *> &V) {

	vector<v*> odds;
	//loop through each city
	for (int i = 0; i < V.size(); ++i) {
		//for first city
		if (i == 0) {
			int vDegree = V[i]->adjacent.size() - 1;
			if (vDegree % 2 != 0) {
				odds.push_back(V[i]);
			}
		}else{
			//if odd add to vector
			if (V[i]->adjacent.size() % 2 != 0) {
				odds.push_back(V[i]);
			}
		}
	}
	return odds;
}
/************************************************************************************
*
*	[Print the full Vertex container]
*	Prints every vertex with full detail
*
************************************************************************************/
void _printV(vector<v*> V)
{
	cout << "\n- Printing " << V.size() << " elements ------------------- " << endl;
	for (int i = 0; i < V.size(); i++)
	{
		_printThisV(V[i]);
	}
	cout << "-------------------------------------------- " << endl;
}

/************************************************************************************
*
*	[Print one Vertex]
*	Prints one vertex with full detail
*
************************************************************************************/
void _printThisV(v* thisV)
{
	cout << "> City : " << thisV->id << "\tx : " << thisV-> x << "\ty : " << thisV-> y << "\tkey : " << thisV-> key << "\tIs in Queue : " << thisV->_isInQueue << "\tIs in MST : " << thisV->_isInMST << endl;
	cout << "\t\tAdjacent (" << thisV->adjacent.size() << ") : ";
	for (int j = 0; j < thisV->adjacent.size(); j++)
	{	
		if (thisV->adjacent[j])
			cout << " > " << thisV->adjacent[j]->id;	
		else 
			cout << " > NULL";

	}
	cout << endl;
}


/************************************************************************************
*
*	[Print Queue]
*	Prints all the city ID in the Queue
*
************************************************************************************/
void _printQ(vector<v*> Q)
{
	cout << "Q : ";
	for (int i = 0; i < Q.size(); i++)
	{
		cout << Q[i]->id << "\t";
	}
	cout << endl;
}


/************************************************************************************
*
*	[Print Distance Matrix to Screen]
*	Prints the 2D int Matrix of distances, must be n x n dimension
*
************************************************************************************/
void _printDistanceMatrix(int ** D, int n)
{
	cout << "\n- Printing Matrix -------------------- " << endl;
	
	for (int m = 0; m < n; m++)
	{
		cout << "\t" << m ; 
	}
	cout << endl;
	for (int i = 0; i < n; i++)
	{
		cout << i << "\t";
		for (int j = 0; j < n; j++)
		{
			cout << D[i][j] << "\t";
		}
		cout << endl;
	}
}

/************************************************************************************
*
*	[Print Distance Matrix to File]
*	Prints the 2D int Matrix of distances, must be n x n dimension
*
************************************************************************************/
void _printDistanceMatrix(int ** D, int n, string filename)
{
	/* Write output */
	ofstream outFile;
	outFile.open(filename);	
	outFile << setw(10);
	outFile << "\n- Printing Matrix -------------------- " << endl;

	outFile << setw(6) << "-";
	for (int m = 0; m < n; m++)
	{
		outFile << setw(6) << "\t" << m ; 
	}
	outFile << endl;
	for (int i = 0; i < n; i++)
	{
		outFile << setw(6) << i << "\t";
		for (int j = 0; j < n; j++)
		{
			outFile << setw(6) << D[i][j] << "\t";
		}
		outFile << endl;
	}

	outFile << "-------------------------------------------- " << endl;

	outFile.clear();
	outFile.close();	
}


/************************************************************************************
*
*	[Remove Link]
*	Removes the target vertex from the target's current parent's adjacent list
*
************************************************************************************/
void _removeLink(vector<v*> &parent_ADJ, v* target)
{
	for (int i = 1; i < parent_ADJ.size(); i++)
	{
		if (parent_ADJ[i]->id == target->id)
		{
			parent_ADJ.erase(parent_ADJ.begin()+i);
			return;
		} 
	}
}

/************************************************************************************
*
*	[Add a vertex to queue]
*	Takes a pointer to a vertex and adds to the queue.
*	Queue is *SORTED* from max to min.  
*
************************************************************************************/
void _addToQueue(vector<v*> &Q, v* target)						
{
	target->_isInQueue = true;	
	for (int i = 0; i < Q.size(); i++)
	{
		if (target->key >= Q[i]->key)
		{
			Q.insert(Q.begin() + i, target);
			return;
		}
	}
	Q.push_back(target);
}


/************************************************************************************
*
*	[Get Minimum Key from queue]
*	Takes the Queue of verticies and pops the minimum, returns the pointer (resizes Q)
*
************************************************************************************/
v* _getMinKey(vector<v*> &Q)
{
	if (Q.size() == 0)
	{
		return NULL;
	}
	else
	{
		v* min = Q[Q.size()-1];
		Q.pop_back();
		return min;
	}
}

/************************************************************************************
*
*	[Print MST in Link List format to Screen]  
*
************************************************************************************/
void printMSTLinklist(vector<v*> V, int n)
{
	cout << "\n- Printing MST in List -------------------- " << endl;
	for(int i = 0; i < n; i++)
	{
		cout << V[i]->id << " : ";
		for (int j = 0; j < V[i]->adjacent.size(); j++)
		{
			if (j == 0) { 
				if (V[i]->adjacent[0] != NULL) { cout << "[" << V[i]->adjacent[0]->id << "] > "; }
				else { cout << "... > "; }
			}
			else {
				cout << V[i]->adjacent[j]->id << " > ";
			}			
		}
		cout << endl;
	}
	cout << "-------------------------------------------- " << endl;
}

/************************************************************************************
*
*	[Print MST in Matrix format to Screen]  
*
************************************************************************************/
void printMSTMatrix(vector<v*> V, int n)
{
	cout << "\n- Printing MST in Matrix -------------------- " << endl;
	
	for (int m = 0; m < n; m++)
	{
		cout << "\t" << m ; 
	}
	cout<<endl;

	for (int i = 0; i < n; i++)
	{
		cout << i << "\t";
		for (int j = 0; j < n; j++)
		{
			bool foundIt = false;
			for (int k = 1; k < V[i]->adjacent.size(); k++)
			{
				
				v* findThis = V[i]->adjacent[k];
				if (findThis == NULL)
				{
					// STarting point
				}
				else if (findThis->id == V[j]->id)
				{
					cout << findThis->key << "\t";
					foundIt = true;
					break;
				}

			}
			if (!foundIt) {	cout << "\t"; }
		}
		cout << endl;
	}
	cout << "-------------------------------------------- " << endl;

}


/************************************************************************************
*
*	[Print MST in Matrix format to File]  
*
************************************************************************************/
void printMSTMatrix(vector<v*> V, int n, string filename)
{

	/* Write output */
	ofstream outFile;
	outFile.open(filename);	
	outFile << setw(10);
	outFile << "\n- Printing MST in Matrix -------------------- " << endl;
	outFile << setw(6) << "-";
	for (int m = 0; m < n; m++)
	{
		outFile << setw(6) << "\t" << m ; 
	}
	outFile<<endl;

	for (int i = 0; i < n; i++)
	{
		outFile << setw(6) << i << "\t";
		for (int j = 0; j < n; j++)
		{
			bool foundIt = false;
			for (int k = 0; k < V[i]->adjacent.size(); k++)
			{
				
				v* findThis = V[i]->adjacent[k];
				if (findThis == NULL)
				{
					if ( i == j )
					{
						outFile << setw(6) << "X\t";
						foundIt = true;
					}
				}
				else if (findThis->id == V[j]->id)
				{
					if (k == 0)
					{
						outFile << setw(6) << V[i]->key <<"\t";
					}
					else
					{
						outFile << setw(6) << findThis->key << "\t";
					}
					foundIt = true;
					break;
				}
			}
			if (!foundIt) {	outFile << setw(6) << "--\t"; }
		}
		outFile << endl;
	}

	outFile << "-------------------------------------------- " << endl;

	outFile.clear();
	outFile.close();
}

/************************************************************************************
*
*	[Cleanup Functions] 
*
************************************************************************************/

void clean_D(int ** &D, int n)
{
	if (D != NULL)
	{
		for (int i = 0; i < n; i++)
		{
			if (D[i] != NULL)
				delete[] D[i];
				D[i] = NULL;
		}
		delete[] D;
		D = NULL;
	}
}

void clean_V(vector<v*> &V)
{
	for (int i = 0 ; i < V.size(); i++)
	{
		if (V[i] != NULL)
		{
			delete V[i];
			V[i] = NULL;
		}
	}
}


/************************************************************************************
*
*	Vector Resize : Currently not used.
*
***********************************************************************************
void _vectorResize(vector<v*> &V, int n)
{
	V.resize(V.capacity() * 2);				// double up the capacity. do not use 2nd argument for faster performance
	cout << "[!] V resized, it now has capacity [" << V.capacity() << "], containing [" << n <<"] elements." << endl;
}

void _vectorTrim(vector<v*> &V , int n)
{
	V.resize(n);							// trim off the empty slots
	cout << "[!] V trimmed, capacity is now [" << V.capacity() << "], containing [" << n << "] elements." << endl;
}

*/

//form Eulerian circuit from connected multigraph
// params: pos is starting vertex id, tour is current tour being processed 
vector<int> _euler(vector<v*> V, int pos, vector<int> &tour)
{
	// make copy of adjacenylist
	vector<v*> temp;

	for (int i = 0; i < V.size(); i++)
	{
		v *thisV = V[i];
		temp.push_back(V[i]);

		for (int j = 0; j < thisV->adjacent.size(); j++)
		{
			temp[i]->adjacent[j] = thisV->adjacent[j];
		}
	}

	//start with empty stack and empty circuit (tour vector)
	std::stack<int> stk;

	//repeat until current vertex has no neighbors(temp) and stack empty
	while (!stk.empty() || temp[pos]->adjacent.size() > 0)
	{
		//if has neighbors...
		if (temp[pos]->adjacent.size() > 0)
		{
			//add vertex to stack
			stk.push(pos);

			//take neighbor
			v* neighbor = temp[pos]->adjacent.back();
			int neighpos = neighbor->id;

			//remove neighbor edge to current vertex
			temp[pos]->adjacent.pop_back();

			for (unsigned int i = 0; i < temp[neighpos]->adjacent.size(); i++)
			{
				if (pos == temp[neighpos]->adjacent[i]->id) {//TODO deal with first city which has no parent and will segfault here: adjacent[i]=NULL
					temp[neighpos]->adjacent.erase(temp[neighpos]->adjacent.begin()+i);
					break;
				}
				//set neighbor to current vertex
				pos = neighpos;
			}

		}
		//if doesn't have neighbors...
		else {
			//add vertex to circuit
			tour.push_back(pos);
			//remove last vertex from stack & set as current
			int last = stk.top();
			stk.pop();
			pos = last;
		}
	}
	//add current pos to end of tour
	tour.push_back(pos);
	return tour;
}

//make Eulerian circuit into hamiltonian circuit
void _make_hamilton(std::vector<int> &tour, int &path_dist, int ** D)
{
	//set up indicators for visited, total dist = 0, current & next
	const int vSize = tour.size();
	vector<bool> visited(vSize);

	path_dist = 0;
	int root = tour.front();
	vector<int>::iterator curr = tour.begin();
	vector<int>::iterator next = tour.begin() + 1;
	visited[root] = true;

	//while end of tour isn't reached yet
	while (next != tour.end())
	{
		//if haven't visited next city, then go
		if (!visited[*next])
		{
			//increase total distance by distance between current and next city
			path_dist += D[*curr][*next];
			curr = next;
			visited[*curr] = true;
			next = curr + 1;
		}
		else
		{
			//remove next city from tour
			next = tour.erase(next);
		}
	}
	//add distance from current and next to total distance
	path_dist += D[*curr][*next];
}

int find_tour(vector<v*> V, int pos, int ** D)
{
	int pathLength;
	vector<int> circuit;
	//euler circuit
	 _euler(V, pos, circuit);

	_make_hamilton(circuit, pathLength, D);

	return pathLength;
}
