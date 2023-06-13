# Branch And Bound C++
## An heuristic to solve "load balancing scheduling with even sums" NP problem.  
**<ins> Problem :</ins>** Given N positive integers (also known as "jobs") and M machines, find a schedule for these jobs on the machines such that the makepsan will be minimal.  
**<ins> Algorithm :</ins>** Branch and Bound in C++  
**<ins> Initialization :</ins>** decide if you want to handle the input in the same order it was inserted or rather sort it in increasing or decreasing order.  
**<ins> Run :</ins>** preferable to use DFS order instead of BFS.  
The algorithm works recursively - create a node, calculate lower and upper bound and then decide if you want to create another sons.  
Lower bound of a node meaning - what is the best solution we might find in it's substree.  
Upper bound of a node meaning - some solution we already found for the node, the calculation of the solution is being calculted by some LPT or LS algorithms.  
while creating the nodes as sons of some node, we assign the job in index height(parent)+1 to each of the machines and caculate the bounds for it.  
Logically what the algorithm does is :  
calculate bounds - if lower bound is too high or upper bound is too small (can't yield good solution in the subtree), then don't create sons (reduce run time).  
otherwise, create M sons - meaning of the sons is "place the next job in order in every machine 1,2,...,M and check if we can find a good solution"  

Since this algorithm works much slower and has to construct a tree, the algorithm complexity might be as high as O(mn*(m^n))
Although the algorithm works so slow, an optimal schedule will be found for any input in the world.
