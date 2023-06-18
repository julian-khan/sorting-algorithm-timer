# Sorting Algorithm Timer

In this project, I created a C++ program that reads 10 files that each contain 25000 integers, adds the numbers in each file to a corresponding vector container and then sorts each vector with the built-in Standard Template Library (STL) Sort function and a programmer-defined implementation of selection sort. The run-time for each algorithm is recorded using the std::chrono library and the user is provided with the average run times in the terminal's output.

The purpose of this project was to compare the time efficiency and thus run time of the STL sort and selection sort algorithms. 

As expected, STL sort, which is an implementation of introsort with a worst-case time complexity of O(nlogn) has a lower run-time than selection sort, which has a time complexity of O(n^2). For a small number of items to sort, the run time difference between the two algorithms may not be as observable. However, as the number of items to be sorted increases, the STL sort with its linearithmic time complexity will have far less run time than the selection sort with its inefficient quadratic time complexity.
