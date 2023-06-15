#include <fstream>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <chrono>
#include <map>

// Function prototypes for the functions defined in this file
std::vector<int> readNumFile(std::string);
std::vector<std::vector<int>> createNumListContainer(int numberOfFiles, std::string pathNameStem, std::string fileType = ".txt");
std::map<int, double> calcSortTime(std::vector<std::vector<int>> &, bool);
void selectionSort(std::vector<int> &a); // Adapted from the course lab files
// printSortTimes prints the sort times for both sorting functions to the terminal
void printSortTimes(const std::map<int, double> &timesMap, std::string sortType);

/*std:: notation has been used instead of importing the entire std namespace because this is best practice to avoid
naming collisions in different header files or libraries. */
int main()
{
    /* Declare two vectors, each of which hold vectors for elements of type int. numberContainer1 and numberContainer2 will
    each contain 10 vectors, with each of these 10 vectors storing the numbers in one of the 10 files to be sorted.*/
    std::vector<std::vector<int>> numberContainer1, numberContainer2;

    std::cout << "Loading the 10 text files into memory\n\n";
    numberContainer1 = createNumListContainer(10, "Randomfiles/Random");
    numberContainer2 = createNumListContainer(10, "Randomfiles/Random");

    // Sort using the STL sort function
    std::cout << "Sorting the 10 text files with STL sort:\n";
    /* The object returned by calcSortTime is a map object containing 10 integer keys, one for each sorted file. The values in this map
    are the time in microseconds as an integer for the sorting of the given file, using either the STL sort function or a programmer-defined
    selection sort. */
    const std::map<int, double> stlSortTimes = calcSortTime(numberContainer1, true);
    // The printSortTimes function prints the sorting times for a given algorithm to the terminal
    printSortTimes(stlSortTimes, "STL sort");

    // Sort using selection sort
    std::cout << "Sorting the 10 text files with selection sort:\n";
    const std::map<int, double> selectionSortTimes = calcSortTime(numberContainer2, false);
    printSortTimes(selectionSortTimes, "selection sort");

    return 0;
}

std::vector<int> readNumFile(std::string fileName)
{
    /* This function opens a file with the path provided to the fileName parameter and
    adds every line-spaced integer in the file to a vector that is returned by the function. */
    std::vector<int> numbers;
    // Initialise an ifstream object that is linked to the file path specified in the fileName parameter.
    std::ifstream inFile(fileName);

    int extractedNum;
    while (inFile >> extractedNum)
    {
        numbers.push_back(extractedNum); // Push extractedNum to the numbers vector
    }

    // Close the file once finished with it, per best practice
    inFile.close();
    // Return the vector object
    return numbers;
}

std::vector<std::vector<int>> createNumListContainer(int numberOfFiles, std::string pathNameStem, std::string fileType)
{
    // Declare a vector named numListContainer. This vector will store 10 vectors, each of which stores every integer in a given file
    std::vector<std::vector<int>> numListContainer;

    for (int i = 0; i < numberOfFiles; i++)
    {
        // Dynamically create the target file path string
        const std::string fileName = pathNameStem + std::to_string(i + 1) + fileType;
        // Call the readNumFile function to create a vector that contains every integer in a given file
        std::vector<int> numList = readNumFile(fileName);
        // Add the vector of integers to the numListContainer container that stores all 10 lists of integers
        numListContainer.push_back(numList);
    }
    // Return the container
    return numListContainer;
}

std::map<int, double> calcSortTime(std::vector<std::vector<int>> &numListContainer, bool isStlSort)
{
    /* This function executes a given sorting algorithm (STL sort or selection sort) and times the sorting process. The time measurements
    are added to a map object. The map object contains 10 keys, one for each of the 10 sets of numbers. The corresponding values are the time
    in microseconds as an integer for the sorting.*/

    // Use type double for values to avoid overflow errors - important if the sort time is long and using a small unit like microseconds
    std::map<int, double> runTimes;

    // Iterate through the 10 vectors contained within numListContainer
    for (int i = 0; i < static_cast<int>(numListContainer.size()); i++)
    {
        if (isStlSort)
        {
            /*These timing-related statements need to be in each branch of the if-else block to avoid the timing including the execution of the if-else statement.
            The timing of evaluating the condition of the if-else block would be a minor difference but this minor adjustment increases accuracy of the timing
            measurements. */
            auto startTime = std::chrono::high_resolution_clock::now();
            std::sort(numListContainer[i].begin(), numListContainer[i].end());
            // stop count time
            auto endTime = std::chrono::high_resolution_clock::now();
            // the time in ms as a double
            std::chrono::duration<double, std::micro> time_us = endTime - startTime;
            // cast the time from double to int
            auto intTimeMicroSec = std::chrono::duration_cast<std::chrono::microseconds>(time_us);

            /* Call the .count() method with returns the duration of the sorting as an integer. Add a key i representing the set of sorted numbers
            and the duration as a value. */
            runTimes[i] = intTimeMicroSec.count();
        }
        else
        { // If isStlSort is false, selection sort will be used to sort the 10 sets of numbers
            // i+1 below to avoid outputting zero-indexing to the terminal
            std::cout << "Undertaking selection sort on file " << i + 1 << ". This may take a while." << std::endl;

            // Initialise an object named startTime by calling the now() function of std::chrono::high_resolution_clock
            auto startTime = std::chrono::high_resolution_clock::now();
            // Call the selectionSort function to sort the ith vector in numListContainer
            selectionSort(numListContainer[i]);
            // Create an object named endTime using the std::chrono::high_resolution_clock now() function
            auto endTime = std::chrono::high_resolution_clock::now();

            /* Create an std::chrono::duration object named time_us that stores time ticks in microseconds. The time_us
            variable indicates how long, in microseconds, the sorting algorithm took to execute. */
            std::chrono::duration<double, std::micro> time_us = endTime - startTime;
            // Convert the stored duration to an integer
            auto intTimeMicroSec = std::chrono::duration_cast<std::chrono::microseconds>(time_us);

            runTimes[i] = intTimeMicroSec.count();
        }
    }
    if (!isStlSort)
        std::cout << std::endl; // To aid with formatting the output

    return runTimes;
}

void selectionSort(std::vector<int> &a)
{
    // An implementation of selection sort
    for (int i = 0; i < static_cast<int>(a.size() - 1); i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < static_cast<int>(a.size()); j++)
        {
            if (a.at(minIndex) > a.at(j))
                minIndex = j;
        }
        int temp = a.at(i);
        a.at(i) = a.at(minIndex);
        a.at(minIndex) = temp;
    }
    return;
}

void printSortTimes(const std::map<int, double> &timesMap, std::string sortType)
{
    // This function outputs the sorting time for the 10 files, using either STL sort or selection sort.

    /*Initialise a variable of type int to track the total time elapsed during the sorting of all
    10 files with a given sorting algorithm. */
    
    double totalTime = 0; // Use double to prevent overflow error
    std::string pathNameStem = "Randomfiles/Random";
    std::string fileType = ".txt";

    for (auto iter = timesMap.cbegin(); iter != timesMap.cend(); iter++)
    {
        // Print a given file's sorting time
        printf("Sort time for file %d (%s): %.0f microseconds\n", iter->first + 1, sortType.c_str(), iter->second);
        totalTime += iter->second;
    }
    std::cout << std::endl; // Add spacing after displaying one container's sort times

    double averageSortTime = totalTime / timesMap.size();
    printf("Total algorithm run time for all files: %.0f microseconds\n", totalTime);
    printf("Average sort time for the %s: %.0f microseconds\n", sortType.c_str(), averageSortTime);

    std::cout
        << std::endl
        << std::endl; // Add spacing after displaying one container's sort times
    return;
}
