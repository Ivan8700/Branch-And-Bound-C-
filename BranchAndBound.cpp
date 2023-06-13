#include <iostream>
#include "RunAlgorithm.h"
#include <time.h>
#include <chrono>
using namespace std::chrono;
const static int fromMicroToSec = 1000000;
int main()
{
	int print,order_of_calculation,get_input;
	cout << "Would you like to print all moves or only the last output?\n 1 - yes , 0 - no : ";
	cin >> print;
	cout << "How do you want to get input ?\n 0 - input from .txt file, 1 - manual input, 2 - randomization : ";
	cin >> get_input;
	cout << "Would you like to calculate the nodes in a specific order ?\n 0 - same order as input, 1 - increasing order, 2 - decreasing order : ";
	cin >> order_of_calculation;
	/* create txt file for the output */
	//std::ofstream out_file; out_file.open("C:/Users/Ivan/Desktop/output.txt");
	//std::streambuf* cout_buffer = cout.rdbuf();  
	//cout.rdbuf(out_file.rdbuf()); 
	auto start = high_resolution_clock::now();
	RunAlgorithm r(get_input, order_of_calculation, print);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << endl << " Time taken by function : " << float(duration.count()) / fromMicroToSec << " seconds" << endl;
	//Close the file if was opened
	//out_file.close();
	//cout.rdbuf(cout_buffer);
	exit(0);

}