#include "buildData.hpp"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {

	string filename = argv[1];
	vector<v*> Vertices;

	cout << "\n- Reading Data --------------------------- " << endl;

	int n = 0;
	n = readData(Vertices, filename);
	if (n < 1) 
		cout << "ERROR: readData() read no data." << endl;
	else 
		cout << "Read from \"" << filename << "\" | # of cities: [" << n << "]" << endl;
 
	cout << "\n- Building Data --------------------------- " << endl;
	int ** Distances = buildMatrix(Vertices, n);
	_printDistanceMatrix(Distances, n);
	buildMST(Vertices, Distances, n);

	cout << "\n- Output Data ----------------------------- " << endl;

	printMSTLinklist(Vertices, n);
	printMSTMatrix(Vertices, n, "outputMST.txt");							// take away the string argument to print to screen

	cout << "\n------------------------------------------- " << endl;





	/* clean up remark
		D is a 2D dynamic array
		V is a dynamic array
	*/
	return 0;
}
