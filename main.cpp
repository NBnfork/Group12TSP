#include "buildData.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int main(int argc, char* argv[]) {

	string filename = argv[1];					// Filename of input 
	vector<v*> Vertices;						// Container for cities, v is a struct (see buildData.hpp)
	int n = 0;									// Number of cities
	int ** Distances = NULL;					// Matrix for distances between cities

	cout << "\n- Reading Data --------------------------- " << endl;
	
		n = readData(Vertices, filename);
 
	cout << "\n- Building Data --------------------------- " << endl;
	
		Distances = buildMatrix(Vertices, n);
		buildMST(Vertices, Distances, n);
		perfectMatching(Vertices, Distances);

/**//* Optional -------------------------------------------------------------------------------------- */
/**/
/**/	cout << "\n- Output Data ----------------------------- " << endl;
/**/	
/**/		_printDistanceMatrix(Distances, n, "outputDistanceMatrix.txt");			// take away the string argument to print to screen							
/**/		printMSTLinklist(Vertices, n);
/**/		printMSTMatrix(Vertices, n, "outputMST.txt");							// take away the string argument to print to screen
/**/
/**//*--------------------------------------------------------------------------------------------------*/

	cout << "\n- Clean up Data --------------------------- " << endl;

		clean_D(Distances, n);
		clean_V(Vertices);



	return 0;
}
