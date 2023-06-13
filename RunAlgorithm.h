#pragma once
#include <list>;
#include <fstream>;
#include <time.h>
#include "Node.h";
#include <limits>;
using std::list; using std::pair;
class RunAlgorithm
{
private:
	int machine_number;
	int number_of_jobs;
	int sum_of_jobs;
	int opt;
	int counter_of_nodes_created;
	int counter_of_nodes_used;
	int chosen_solution_value;
	int* input_arr;
	int* sorting_decreasing_of_input_arr; // Used for LPT algorithm
	int* index_array_of_the_sorted_decreasing_input_arr; //Being used to know which job we can still allocate in every node
	list<pair<int, int>>* chosen_solution_schedule;
	int* chosen_solution_load;

public:
	RunAlgorithm(const int& option_to_read_input, const int& option_to_sort_array, const int& print);
	~RunAlgorithm();
	void heapSort(int arr[], const int& length,const int& method_of_sorting);
	int updateChosenSolution(list<pair<int, int>>* schedule, int* load_on_each_machine);
	void printArray();
	void printSchedule(list <pair<int, int>> s[], int sum[]);
	void printFinalSchedule();
	void startRun(const int& print);
	int LS(list<pair<int, int>> schedule[], int sumArray[], int arr[], const int& height);
	int LPT(list<pair<int, int>> schedule[], int sumArray[], int indexArr[], int arr[], const int& height);
	int lowerBound(int sumArray[], const int& opt, int arr[], list<pair<int, int>> schedule[], const int& height);
};

