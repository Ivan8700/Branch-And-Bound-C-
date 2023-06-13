#include "RunAlgorithm.h"

bool comparePairs(const pair<int, int>& a, const pair<int, int>& b);
void heapify(int arr[], int n, int i);
void heapify_decreasing(int arr[], int n, int i);
RunAlgorithm::RunAlgorithm(const int& option_to_read_input, const int& option_to_sort_array, const int& print) : sum_of_jobs(0), counter_of_nodes_created(0), counter_of_nodes_used(0)
{
	int cnt_odd = 0;
	if (option_to_read_input == 0) //Read input_list from .txt file
	{
		std::ifstream infile;
		infile.open("inputs/input1.txt");
		infile >> machine_number;
		cout << "Enter number of machines : ";
		cout << machine_number << "\n"; 
		infile >> number_of_jobs;
		cout << "Enter Amount of numbers : ";
		cout << number_of_jobs << "\n"; 
		input_arr = new int[number_of_jobs];
		if (infile)
		{
			int i = 0;
			while (infile >> input_arr[i] && i < number_of_jobs)
			{
				sum_of_jobs += input_arr[i];
				if (input_arr[i] % 2 == 1)
				{
					cnt_odd++;
				}
				i++;
			}

			infile.close();
		}
		else
			cout << "File was not found" << endl;
	}
	else
	{
		if (option_to_read_input == 1) //Manual insertion input_list
		{
			cout << "Enter amount of machines : "; cin >> machine_number;
			cout << "\n Enter amount of jobs : "; cin >> number_of_jobs; cout << "\n";
			input_arr = new int[number_of_jobs];
			for (int i = 0; i < number_of_jobs; i++)
			{
				cout << "Enter a job : ";
				cin >> input_arr[i]; cout << "/n";
				this->sum_of_jobs += input_arr[i];
				if (input_arr[i] % 2 == 1)
					cnt_odd++;
			}
		}
		else //Random input_list
		{
			srand((unsigned int)time(NULL));
			cout << "Enter amount of machines : "; cin >> machine_number;
			cout << "\n Enter amount of jobs : "; cin >> number_of_jobs; cout << "\n";
			input_arr = new int[number_of_jobs];
			for (int i = 0; i < number_of_jobs; i++)
			{
				input_arr[i] = (rand() % 40) + 21; // Size of jobs [21,60]
				this->sum_of_jobs += input_arr[i];
				if (input_arr[i] % 2 == 1)
					cnt_odd++;
			}
		}
	}
	if (cnt_odd % 2 == 1)
	{
		for (int i = 0; i < number_of_jobs; i++) //if there is odd number of odd jobs, change some even job to odd.
		{
			if (input_arr[i] % 2 == 0)
			{
				--input_arr[i];
				--sum_of_jobs;
				break;
			}
		}
	}
	opt = ceil(double(sum_of_jobs) / machine_number);
	opt = (opt % 2 == 1) ? ++opt : opt;
	chosen_solution_value = sum_of_jobs;
	chosen_solution_schedule = new list<pair<int, int>>[machine_number];
	chosen_solution_load = new int[machine_number];
	printArray();
	if (option_to_sort_array == 1)
	{
		//insertionSortIncreasingOrder(input_arr);
		heapSort(input_arr, number_of_jobs,0);
	}
	else
	{
		if (option_to_sort_array == 2)
		{
			//insertionSortDecreasingOrder(input_arr, number_of_jobs);
			heapSort(input_arr, number_of_jobs, 1);
		}
	}
	cout << "The chosen order for the input is : \n";
	printArray();
	cout << "Sum of jobs is : " << sum_of_jobs << "\n";
	cout << "OPT is bound by : " << opt << "\n";
	startRun(print);
}
RunAlgorithm::~RunAlgorithm()
{
	delete[] this->input_arr;
	delete[] chosen_solution_load;
	delete[] sorting_decreasing_of_input_arr;
	delete[] index_array_of_the_sorted_decreasing_input_arr;
	for (int i = 0; i < machine_number; i++)
	{
		chosen_solution_schedule[i].clear();
	}
	delete[] chosen_solution_schedule;
}
/*Prints the array of input_list*/
void RunAlgorithm::printArray()
{
	for (int i = 0; i < number_of_jobs; i++)
		cout << "(" << input_arr[i] << "," << i + 1 << ")" << "\t";
	cout << "\n";
}
/*Prints the schedule of the node*/
void RunAlgorithm::printSchedule(list<pair<int, int>> s[], int sum[])
{
	list<pair<int, int>>::iterator it;
	int makespan = 0;
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ \n";
	cout << "Building Node for the Tree : \n";
	cout << "The output will be in the form of (number, index) :\n";
	for (int i = 0; i < machine_number; i++)
	{
		cout << "Machine number " << (i + 1) << "\n";
		cout << "{";
		for (it = s[i].begin(); it != s[i].end(); ++it)
		{
			cout << "(" << (*it).first << "," << (*it).second << ") ";
		}
		cout << "}" << " \n -> Sum of The Machine is : " << sum[i] << "\n \n";
		if (sum[i] > makespan)
			makespan = sum[i];
	}
	cout << "The makespan is  " << makespan << endl;
}
/*Prints the final schedule (best node)*/
void RunAlgorithm::printFinalSchedule()
{
	cout << "\n \n";
	int makespan = 0;
	list<pair<int, int>>::iterator it;
	cout << "----------------------------------------------------------------------------- \n";
	cout << "The output will be in the form of (number, index) :\n";
	for (int i = 0; i < machine_number; i++)
	{
		cout << "Machine number " << (i + 1) << "\n";
		cout << "{";
		for (it = chosen_solution_schedule[i].begin(); it != chosen_solution_schedule[i].end(); ++it)
		{
			cout << "(" << (*it).first << "," << (*it).second << ") ";
		}
		cout << "}" << " \n -> Sum of The Machine is : " << chosen_solution_load[i] << "\n \n";
		if (chosen_solution_load[i] > makespan)
			makespan = chosen_solution_load[i];
	}
	cout << "The makespan in this node  " << makespan << endl;
	cout << "The opt is bound by " << opt << endl;
	cout << "Amount of nodes created is :  " << counter_of_nodes_created;
	cout << "\nAmount of nodes used (built) :  " << counter_of_nodes_used;

}
/* Sort the input_list in O(nlogn)*/
void RunAlgorithm::heapSort(int arr[], const int& length,const int& method_of_sort)
{
	// Build max heap
	if (method_of_sort == 0)
	{
		for (int i = length / 2 - 1; i >= 0; i--)
			heapify(arr, length, i);
	}
	else
	{
		for (int i = length / 2 - 1; i >= 0; i--)
			heapify_decreasing(arr, length, i);
	}

	// Heap sort
	for (int i = length - 1; i >= 0; i--) {
		std::swap(arr[0], arr[i]);

		// Heapify root element to get highest element at root again
		if (method_of_sort == 0)
		{
			heapify(arr, i, 0);
		}
		else
		{
			heapify_decreasing(arr, i, 0);
		}
	}
}
/*Check if we need to update the chosen solution*/
int RunAlgorithm::updateChosenSolution(list<pair<int, int>>* schedule, int* load_on_each_machine)
{
	int makespan = 0;
	for (int i = 0; i < machine_number; i++)
	{
		if (load_on_each_machine[i] > makespan)
			makespan = load_on_each_machine[i];
	}
	if (makespan < chosen_solution_value)
	{
		chosen_solution_value = makespan;
		for (int i = 0; i < machine_number; i++)
		{
			chosen_solution_schedule[i] = schedule[i];
			chosen_solution_load[i] = load_on_each_machine[i];
		}
	}
	return makespan;
}
/*Calculate upper bound of the node using the LS algorithm on the jobs which can be scheduled.
LS - traverse the jobs, if the job is even - schedule it in the machine with the minimal load
if the job is odd - look for a machine with minimal load and odd summation, if such exists - schedule the job there, if not act as if the job is even.*/
int RunAlgorithm::LS(list<pair<int, int>> schedule[], int sumArray[], int arr[], const int& height)
{
	list<pair<int, int>>* s = new list<pair<int, int>>[machine_number];
	int* sumS = new int[machine_number];
	for (int i = 0; i < machine_number; i++)
	{
		s[i] = schedule[i];
		sumS[i] = sumArray[i];
	}
	int cnt_odd_machines = 0, cnt_amount_of_odd_jobs = 0;
	for (int i = 0; i < machine_number; i++)
	{
		if (sumS[i] % 2 == 1)
		{
			cnt_odd_machines++;
		}
	}
	for (int i = height; i < number_of_jobs; i++)
	{
		if (arr[i] % 2 == 1)
		{
			cnt_amount_of_odd_jobs++;
		}
	}
	/*Check if the node can be fixed, if the node can't be fixed return '-3' so the algorithm will know not to create sons for this node
	since no valid solution can be found.
	node with non-valid solution avaiable is a node, which has less odd jobs left to schedule than the amount of machines with odd summation
	or more odd jobs left to schedule, but (amount of odd jobs - amount of odd summation machines) is odd.*/
	if (cnt_amount_of_odd_jobs < cnt_odd_machines || (cnt_amount_of_odd_jobs - cnt_odd_machines) % 2 == 1) 
	{
		delete[] s;
		delete[] sumS;
		return -3;
	}
	bool foundODD;
	int minimum_makespan_on_odd_machine, index_with_minimum_odd_makespan, minimum_makespan_on_machine, index_with_minimum_makespan;
	//========================= input_list the remaining jobs of this node =============================
	for (int i = height; i < number_of_jobs; i++)
	{
		foundODD = false;
		minimum_makespan_on_odd_machine = INT_MAX; minimum_makespan_on_machine = INT_MAX;
		index_with_minimum_odd_makespan = 0; index_with_minimum_makespan = 0;
		if (arr[i] % 2 == 1)
		{
			for (int j = 0; j < machine_number; j++)
				if (sumS[j] % 2 == 1 && sumS[j] < minimum_makespan_on_odd_machine)
				{
					foundODD = true;
					minimum_makespan_on_odd_machine = sumS[j];
					index_with_minimum_odd_makespan = j;
				}
			if (foundODD)
			{
				s[index_with_minimum_odd_makespan].push_front(std::make_pair(arr[i], i + 1));
				sumS[index_with_minimum_odd_makespan] += arr[i];
				continue;
			}
		}
		/*Schedule even job on minimal load machine, or odd job on minimal load machine (when no odd summation machine exists)*/
		for (int j = 0; j < machine_number; j++)
		{
			if (sumS[j] < minimum_makespan_on_machine)
			{
				minimum_makespan_on_machine = sumS[j];
				index_with_minimum_makespan = j;
			}
		}
		s[index_with_minimum_makespan].push_front(std::make_pair(arr[i], i + 1));
		sumS[index_with_minimum_makespan] += arr[i];
	}

	//printSchedule(s, sumS); //choose if you want to print the calculation of this node

	int makespan = updateChosenSolution(s, sumS);
	/*int makespan = 0;
	for (int i = 0; i < machine_number; i++)
	{
		if (sumS[i] > makespan)
			makespan = sumS[i];
	}
	if (makespan < chosen_solution_value)
	{
		chosen_solution_value = makespan;
		for (int i = 0; i < machine_number; i++)
		{
			chosen_solution_schedule[i] = s[i];
			chosen_solution_load[i] = sumS[i];
		}
	}*/
	delete[] s, delete[] sumS;
	if (height == number_of_jobs)
	{
		return -2; //Return that its the last job (left) of the tree, no sons should be created.
	}
	return makespan;
}
/*Calculate upper bound of the node using the LPT algorithm  on the jobs which can be scheduled.
LPT - first, schedule the even jobs on the minimal load of the machines.
then, take the odd jobs and place them in the odd summation machines (if exists).
if no more odd summation machines exist - place the jobs in pairs, biggest and smallest odd machine on the minimal load machine.*/
int RunAlgorithm::LPT(list<pair<int, int>> schedule[], int sumArray[], int indexArr[], int arr[], const int& height)
{
	list<pair<int, int>>* s = new list<pair<int, int>>[machine_number];
	int* sumS = new int[machine_number];
	int cnt_odd_jobs = 0;
	for (int i = 0; i < machine_number; i++)
	{
		s[i] = schedule[i];
		sumS[i] = sumArray[i];
	}
	list<pair<int, int>> input_list;
	//Traverse the input jobs and place the jobs that can be scheduled in a list.
	if (height != number_of_jobs)
	{
		for (int i = 0; i < number_of_jobs; i++)
		{
			if (indexArr[i] > height)
			{
				input_list.push_back(std::make_pair(arr[i], indexArr[i]));
				if (arr[i] % 2 == 1)
				{
					cnt_odd_jobs++;
				}
			}
		}
	}
	int indexMin = 0, indexMinOdd = 0, makespanAtIndexMinODD, makespanAtIndexMin, cntOddMachine = 0;
	for (int i = 0; i < machine_number; i++)
	{
		if (sumS[i] % 2 == 1)
			cntOddMachine++;
		if (sumS[i] < sumS[indexMin])
			indexMin = i;
	}
	if (cnt_odd_jobs < cntOddMachine || (cnt_odd_jobs - cntOddMachine) % 2 == 1) //check if possible to even fix, same reasoning as in LS
	{
		delete[] s, delete[] sumS;
		return -3;
	}
	list<pair<int, int>>::iterator it, t;
	bool ok;
	//================== Assign even jobs ===========================================================
	for (it = input_list.begin(); it != input_list.end();)
	{
		if ((*it).first % 2 == 0) //place even job in the minimal load machine
		{
			sumS[indexMin] += (*it).first;
			s[indexMin].push_back(*it);
			for (int i = 0; i < machine_number; i++)
				if (sumS[i] < sumS[indexMin])
					indexMin = i;
			t = (++it); input_list.erase(--it); it = t;
		}
		else
		{
			++it;
		}
	}
	//================== Assign odd jobs ============================================================
	for (it = input_list.begin(); it != input_list.end(); ++it)
	{
		ok = true;
		indexMinOdd = 0, indexMin = 0, makespanAtIndexMinODD = INT_MAX, makespanAtIndexMin = INT_MAX;
		for (int i = 0; i < machine_number; i++)
		{
			if (sumS[i] % 2 == 1 && sumS[i] < makespanAtIndexMinODD)
			{
				ok = false;
				makespanAtIndexMinODD = sumS[i];
				indexMinOdd = i;
			}
			if (sumS[i] % 2 == 0 && sumS[i] < makespanAtIndexMin)
			{
				makespanAtIndexMin = sumS[i];
				indexMin = i;
			}
		}
		if (!ok) //if exists a machine with odd summation, place the odd jobs there.
		{
			s[indexMinOdd].push_front(*it);
			sumS[indexMinOdd] += (*it).first;
		}
		else //if there is no machine with odd summation, place biggest and smallest odd jobs in the minimal load machine.
		{
			s[indexMin].push_front(*it);
			sumS[indexMin] += (*it).first;
			s[indexMin].push_front(input_list.back());
			sumS[indexMin] += (input_list.back().first);
			input_list.pop_back();
		}
	}

	//printSchedule(s, sumS); //choose if you want to print the calculation of this node

	int makespan = updateChosenSolution(s,sumS);
	/*for (int i = 0; i < machine_number; i++)
	{
		if (sumS[i] > makespan)
			makespan = sumS[i];
	}
	if (makespan < chosen_solution_value)
	{
		chosen_solution_value = makespan;
		for (int i = 0; i < machine_number; i++)
		{
			chosen_solution_schedule[i] = s[i];
			chosen_solution_load[i] = sumS[i];
		}
	}*/
	delete[] s, delete[] sumS;
	if (height == number_of_jobs)
	{
		return -2; //if the node is a leaf, return no sons should be created.
	}
	return makespan;
}
/*Calculate lower bound of the node using 4 options :
1) maximal load on machine
2) opt bound (average)
3) minimal load machine + average on left to schedule jobs
4) minimal load machine + biggest and smallest job left to schedule*/
int RunAlgorithm::lowerBound(int sumArray[], const int& opt, int arr[], list<pair<int, int>> schedule[], const int& height)
{
	int maximal_machine_load = 0; int minimal_machine_load = INT_MAX;
	for (int i = 0; i < machine_number; i++) //Find minimal and maximal load on the node
	{
		if (sumArray[i] > maximal_machine_load)
			maximal_machine_load = sumArray[i];
		if (sumArray[i] < minimal_machine_load)
			minimal_machine_load = sumArray[i];
	}
	int sum_on_left_jobs = 0, smallest_odd_job = INT_MAX, biggest_odd_job = 0, cntODD = 0;
	for (int i = height; i < number_of_jobs; i++) //sum all the remaining unscheduled jobs, find biggest and smallest odd job
	{
		sum_on_left_jobs += arr[i];
		if (arr[i] % 2 == 1)
		{
			cntODD++;
			if (arr[i] < smallest_odd_job)
				smallest_odd_job = arr[i];
			if (arr[i] > biggest_odd_job)
				biggest_odd_job = arr[i];
		}
	}
	sum_on_left_jobs = ceil(((double)sum_on_left_jobs) / this->machine_number);
	//Return maximal value that we calculated
	int* calculate = new int[4];
	calculate[0] = maximal_machine_load;
	calculate[1] = opt;
	calculate[2] = (minimal_machine_load + sum_on_left_jobs);
	if (cntODD != 0)
		calculate[3] = (minimal_machine_load + biggest_odd_job + smallest_odd_job);
	else
		calculate[3] = minimal_machine_load;
	heapSort(calculate, 4, 1);
	smallest_odd_job = calculate[0];
	delete[] calculate;
	return smallest_odd_job;
}
/*Main Algorithm :
traversal order : DFS
take node - calculate upper and lower bound
if lower bound is still smaller than chosen solution (perhaps better solution can be found in the node's subtree) and upper bound > lower bound (can still find a better solution)
then create 'machine_number' sons which means - (all the options for the next job to be scheduled - on first machine or second or .... last machine)*/
void RunAlgorithm::startRun(const int& print)
{
	sorting_decreasing_of_input_arr = new int[number_of_jobs];
	index_array_of_the_sorted_decreasing_input_arr = new int[number_of_jobs];
	list<pair<int, int>> temp_list;
	//================================ sort the input for LPT usage and save indecies ======================
	for (int i = 0; i < number_of_jobs; i++)
	{
		temp_list.push_front(std::make_pair((size_t)input_arr[i], (size_t)(i + 1)));
	}
	temp_list.sort(comparePairs);
	list<pair<int, int>>::iterator it = temp_list.begin();
	for (int i = 0; i < number_of_jobs; i++)
	{
		sorting_decreasing_of_input_arr[i] = (size_t)it->first;
		index_array_of_the_sorted_decreasing_input_arr[i] = (size_t)it->second;
		++it;
	}
	//================================ Building the root and startup of the algorithm ======================
	Node* root = new Node(machine_number,opt);
	counter_of_nodes_created++;
	list<Node*>* order_of_scan = new list<Node*>; //Using list so we can change between DFS and BFS easily.
	order_of_scan->push_front(root);
	Node* node_temp;
	list<Node*>::iterator it_to_delete;
	//Algorithm startup
	int solution_for_LPT;
	//================================ Starting the DFS ======================
	while (order_of_scan->size() != 0)
	{
		node_temp = order_of_scan->front();
		it_to_delete = order_of_scan->begin();
		if (counter_of_nodes_created != 1) // if its not root, then schedule the job associated with height and child fields.
			node_temp->addNodeInShedule(input_arr[node_temp->getH() - 1], node_temp->getH(), (node_temp->getChild() - 1), input_arr);
		if (print == 1)
			printSchedule(node_temp->getSchedule(), node_temp->getSumArray());
		node_temp->setU(LS(node_temp->getSchedule(), node_temp->getSumArray(), input_arr, node_temp->getH()));
		solution_for_LPT = LPT(node_temp->getSchedule(), node_temp->getSumArray(), index_array_of_the_sorted_decreasing_input_arr, sorting_decreasing_of_input_arr, node_temp->getH());
		if (solution_for_LPT < node_temp->getU()) //Calculate upper bounds using LPT and LS algorithms, choose the best solution of the two.
			node_temp->setU(solution_for_LPT);
		node_temp->setL(lowerBound(node_temp->getSumArray(), opt, input_arr, node_temp->getSchedule(), node_temp->getH()));
		if (print == 1)
		{
			cout << "Node with [L,U]=[" << node_temp->getL() << "," << node_temp->getU() << "] \n";
			cout << "Chosen solution till now : " << chosen_solution_value << "\n";
		}
		counter_of_nodes_used++;
		if (chosen_solution_value == opt) //if found best solution, exit to economize run time.
			break;
		/*if the node is not a leaf, the node can be fixed (if it has odd summation machines), better solution than the chosen solution ca be found, then create sons.*/
		if ((node_temp->getL() != node_temp->getU()) && chosen_solution_value > node_temp->getL() && node_temp->getU() != -2 && node_temp->getU() != -3)
		{
			if (counter_of_nodes_created != 1) //create 'machine_number' sons if its not the root
			{
				for (int i = 0; i < machine_number; i++)
				{
					Node* k = new Node(machine_number,-1, -1, node_temp->getH() + 1, node_temp->getSchedule(), node_temp->getSumArray(),(machine_number - i));
					order_of_scan->push_front(k);
				}
				counter_of_nodes_created += machine_number;
			}
			else //if its the root, create only 1 son to reduce run time (due to symmetric cases)
			{
				Node* k = new Node(machine_number,-1, -1, node_temp->getH() + 1, node_temp->getSchedule(), node_temp->getSumArray(),1);
				order_of_scan->push_front(k);
				counter_of_nodes_created++;
			}
		}
		else //Printing reasons for not creating sons
		{
			if (print == 1)
			{
				if (chosen_solution_value <= node_temp->getL())
					cout << "We do not create sons for this node because v<=" << node_temp->getL() << "\n";
				else
					if (node_temp->getU() == -2 || node_temp->getU() == -3)
					{
						if (node_temp->getU() == -2)
							cout << "We do not create sons for this node because its a leaf \n";
						else
							cout << "We do not create sons for this node because there are too many machines with odd sum that the remaining input_list can't fix. \n";
					}
					else
						cout << "We do not create sons for this node because L=U \n";
			}
		}
		delete* it_to_delete;
		order_of_scan->erase(it_to_delete);
	}
	/*Found best solution, going ahead to print and free memory*/
	printFinalSchedule();
	for (list<Node*>::iterator q = order_of_scan->begin(); q != order_of_scan->end(); ++q)
		delete* q;
	order_of_scan->clear();
	delete order_of_scan;
}
/*Used for sorting list of pairs in decreasing order*/
bool comparePairs(const std::pair<int, int>& a, const std::pair<int, int>& b)
{
	return a.first > b.first;
}
void heapify(int arr[], int n, int i) 
{
	// Find largest among root, left child and right child
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && arr[left] > arr[largest])
		largest = left;

	if (right < n && arr[right] > arr[largest])
		largest = right;

	// Swap and continue heapifying if root is not largest
	if (largest != i) {
		std::swap(arr[i], arr[largest]);
		heapify(arr, n, largest);
	}
}
void heapify_decreasing(int arr[], int n, int i) 
{
	int smallest = i; // Initialize smallest as root
	int l = 2 * i + 1; // left = 2*i + 1
	int r = 2 * i + 2; // right = 2*i + 2

	// If left child is smaller than root
	if (l < n && arr[l] < arr[smallest])
		smallest = l;

	// If right child is smaller than smallest so far
	if (r < n && arr[r] < arr[smallest])
		smallest = r;

	// If smallest is not root
	if (smallest != i) {
		std::swap(arr[i], arr[smallest]);

		// Recursively heapify the affected sub-tree
		heapify_decreasing(arr, n, smallest);
	}
}