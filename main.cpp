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


	//timing total
	clock_t mainStart = clock();

	//timing
	clock_t start = clock();
	Distances = buildMatrix(Vertices, n); //insert function here
	clock_t end = clock();
	double time = (double) (end - start) / CLOCKS_PER_SEC;
	std::cout << "Time to build DMaxtrix: " << time << " secs" << std::endl;

	//timing
	start = clock();
	buildMST(Vertices, Distances, n);
	end = clock();
	time = (double) (end - start) / CLOCKS_PER_SEC;
	std::cout << "Time to build MST" << time << " secs" << std::endl;

	//timing
	start = clock();
	perfectMatching(Vertices, Distances);
	end = clock();
	time = (double) (end - start) / CLOCKS_PER_SEC;
	std::cout << "Time to add Perfect Matching: " << time << " secs" << std::endl;

	clock_t mainEnd = clock();
	time = (double) (mainEnd - mainStart) / CLOCKS_PER_SEC;
	std::cout << "\nTotal running: " << time << " secs" << std::endl;

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
