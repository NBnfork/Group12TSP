#include "buildData.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <fstream>
#include <sstream>
using namespace std;


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
				new_v->adjacent.resize(1);						// adjacent[0] is a pointer pointing to parent. default is pointing to NULL;
				new_v->adjacent[0] == NULL;
				if (new_v->adjacent.size() != 1) { cout << "ERROR!! Check struct construction process in readData()!" << endl; }
				new_v->key = numeric_limits<int>::max();
				new_v->_isInQueue = false;
				new_v->_isInMST = false;
				stringstream ss(line);							/* https://stackoverflow.com/questions/16987461/extraction-of-integers-from-strings */
				int arg = 0;
				for (int i = 0; ss >> i; ) {
					switch (arg) {
						case 0:	new_v->id = i;	
								arg = 1;
								break; 
						case 1:	new_v->x = i;
								arg = 2;
								break;
						case 2: new_v->y = i;
								arg = 0;
								break;
						default: cout << "WARNING!! Something is off with reading data! Check switch statement." << endl;
								break;
					}
				}

				V.push_back(new_v);
				n++;

				/* dynamic resize */
				// if (n + 1 == V.capacity())
				// {
				// 	_vectorResize(V, n);
				// }
			}
		}
		inputFile.clear();
		inputFile.close();
	}
	// _vectorTrim(V, n);

	_printV(V);
	return n;
}



int** buildMatrix(vector<v*> V, int n)
{
	int ** D = new int* [n];		//intializing matrix
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
				D[i][j] = sqrt(pow((x1 - x2),2) + pow((y1 - y2),2));
			}
		}
	}

	cout << "/* Filled D Matrix */" << endl;

	return D;
}

void buildMST(vector<v*> &V, int ** D, int n)
{

	int root = 0; 				// starting from city #0
	V[root]-> key = 0;			// zero distance starting point

	v* current = NULL;			// from this vertex
	v* target = NULL;			// to this vertex
	vector<v*> Q; Q.clear();	// queue for unhandled verticies, sorted from greatest to smallest by key value 

	int d = 0;					// holder variable for distance
	int count = 0;				// count how many verticies (cities) have been computed by MST

	current = V[root];
	current-> _isInMST = true;

	while (count != n)
	{
		int i = count;									// current vertex i in V
		for (int j = 0; j < n; j++)						// all verticies j in V
		{
			target = V[j];								// points to this target j
			if (i != j)
			{
				d = D[i][j];								// d holds distance from current i to target j
				if (target->_isInMST == false) 				// this vertex hasn't been computed
				{
					if (d < target->key)					// update the key in target because this is a shorter distance
					{
						target->key = d;
						
						if (target->adjacent[0] != NULL)							// some prevoius vertex pointing to
					 	{ _removeLink(target->adjacent[0]->adjacent, target); }		// remove target from previous' adjacent list
						target->adjacent[0] = current;								// set target adjacent[0] as current
						current->adjacent.push_back(target);						// add target to current adjacent list
					}
				}

				if (target->_isInQueue == false && target->_isInMST == false)
				{
					_addToQueue(Q, target);
					//_printQ(Q);													// unblock to see the content of Q each time adding new vertex
				}
			}
	
		}
		// Prepare for next current
		current->_isInMST = true;
		count ++;
		current = _getMinKey(Q);
		if (current == NULL && count != n)
		{ cout << "ERROR!! cannot get the next in queue! Check buildMST()" << endl; }

	}

	if (Q.size() != 0 || current != NULL)
	{ cout << "ERROR!! Something wrong with buildMST(), there is remaining vertices unhandled" << endl; }

}

void _printV(vector<v*> V)
{
	cout << "\n- Printing " << V.size() << " elements ------------------- " << endl;
	for (int i = 0; i < V.size(); i++)
	{
		_printThisV(V[i]);
	}
	cout << "-------------------------------------------- " << endl;
}

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

void _printQ(vector<v*> Q)
{
	cout << "Q : ";
	for (int i = 0; i < Q.size(); i++)
	{
		cout << Q[i]->id << "\t";
	}
	cout << endl;
}
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

void _addToQueue(vector<v*> &Q, v* target)						// Keeps Q sorted
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

void printMSTMatrix(vector<v*> V, int n, string filename)
{
	/* Write output */
	ofstream outFile;
	outFile.open(filename);
	

	outFile << "\n- Printing MST in Matrix -------------------- " << endl;
	
	for (int m = 0; m < n; m++)
	{
		outFile << "\t" << m ; 
	}
	outFile<<endl;

	for (int i = 0; i < n; i++)
	{
		outFile << i << "\t";
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
					outFile << findThis->key << "\t";
					foundIt = true;
					break;
				}

			}

			if (!foundIt) {	outFile << "\t"; }

		}
		outFile << endl;
	}

	outFile << "-------------------------------------------- " << endl;

	outFile.clear();
	outFile.close();

}




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

