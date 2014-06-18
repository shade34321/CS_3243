#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdexcept>

enum SORT_TYPE {
	QUICK_SORT = 0,
	QUICK_SORT3 = 1,
	SELECTION_SORT = 2,
	MERGE_SORT = 3,
	BUBBLE_SORT = 4,
	SHELL_SORT = 5,
	INSERTION_SORT = 6,
	HEAP_SORT = 7,
	BUCKET_SORT = 8,
	COMB_SORT = 9
};

pthread_mutex_t globalLock = PTHREAD_MUTEX_INITIALIZER;

std::vector<long> masterUnsortedList;
std::vector<long> masterSortedList;

std::string fileName;

int m_numOfRunningThreads = 0;

void doQuickSort(std::vector<long> &unsortedList, long begin, long end);
void doSelectionSort(std::vector<long> &unsortedList);
void doMergeSort(std::vector<long> &list);
void doBubbleSort(std::vector<long> &unsortedList);
void doShellSort(std::vector<long> &unsortedList);
void doInsertionSort(std::vector<long> &unsortedList);
void writeContentsToFile(std::vector<long> &values, std::string fileName);

std::vector<long> mergeSortedLists(std::vector<long> &firstList,
		std::vector<long> &secondList);
std::vector<long> mergeUnsortedLists(std::vector<long> &firstList,
		std::vector<long> &secondList);

/*
 * Main execution thread used by a ComplexThread when pthread_create() is called.
 */
void *performSortOnThread(void *complex_thread);

/*
 * struct used to encapsulate a pthread along with its
 * specific data.
 */
struct ComplexThread {
	long m_threadID;
	pthread_t m_thread;
	SORT_TYPE m_sort_type;
	std::vector<long> data_list;

	ComplexThread() {
		m_threadID = -1;
		m_thread = NULL;
		m_sort_type = QUICK_SORT;
	}

	ComplexThread(const std::vector<long> list) :
			data_list(list) {
		m_thread = NULL;
		m_threadID = -1;
		m_sort_type = QUICK_SORT;
	}

	ComplexThread(const std::vector<long> list, SORT_TYPE sort_type) :
			data_list(list), m_sort_type(sort_type) {
		m_thread = NULL;
		m_threadID = -1;
	}

	void createAndExecuteThread() {
		m_threadID = pthread_create(&m_thread, NULL, &performSortOnThread,
				this);

		if (m_threadID == 0) {
			++m_numOfRunningThreads;
			m_thread = pthread_self();
		}
	}
};

/*
 * Callback method used by a thread that has finished its execution.
 * Allows itself to join the main thread and pass its processed data to it.
 */
void onThreadFinished(ComplexThread* thread);
std::vector<ComplexThread*> partitionMasterListForSpecifiedNumOfThreads(
		int numOfThreads);

void trial1();
void trial2();

int main() {
	int user_choice = -1;

	std::cout << "Project 3 - Part 3" << std::endl;
	std::cout << "\t1. Trial 1 (2 threads - Quicksort, Selection Sort" << std::endl;
	std::cout << "\t2. Trial 2 (4 threads - Mergesort, Bubblsort, Shell sort, Insertion sort" << std::endl;
	std::cout << "\n\tNumber choice: ";

	std::cin >> user_choice;

	switch(user_choice) {
	case 1: {
		fileName = "sorted_A.txt";
		trial1();
		break;
	}
	case 2: {
		fileName = "sorted_B.txt";
		trial2();
		break;
	}
	default: {
		std::cout << "Incorrect option." << std::endl;
		exit(0);
	}
	}

	/* Sleeping the main thread for 1 second while background
	 * threads are still processing.
	 */
	while (m_numOfRunningThreads > 0) {
		sleep(1);
	}

	std::cout << "All threads finished!" << std::endl;


	writeContentsToFile(masterSortedList, fileName);

	return 0;
}

std::vector<ComplexThread*> partitionMasterListForSpecifiedNumOfThreads(
		int numOfThreads) {
	std::vector<ComplexThread*> threadList;

	std::ifstream inStream("numbers.txt");
	std::string m_line;
	while (std::getline(inStream, m_line)) {

		//Converting from a string to a long.
		long number = strtol((char*) m_line.c_str(), NULL, 0);

		masterUnsortedList.push_back(number);
	}
	inStream.close();

	/*
	 * Sanity check to ensure we actually have a useful number of theads.
	 * We'll purposely ask the user if we initially do not.
	 */
	if (numOfThreads == NULL || numOfThreads < 1) {
		std::cout << "Number of threads: ";
		std::cin >> numOfThreads;
	}

	int range = masterUnsortedList.size() / numOfThreads;
	long offset = 0;
	long lastIndex = range;
	for (int i = 0; i < numOfThreads; i++) {
		std::vector<long> partitionedData;
		std::vector<long>::iterator it;

		for (int j = offset; j < lastIndex; j++) {
			partitionedData.push_back(masterUnsortedList.at(j));
		}

		offset += range;
		lastIndex += range;

		ComplexThread* thread = new ComplexThread(partitionedData);

		threadList.push_back(thread);
	}

	return threadList;
}

/*
 * Main execution thread used by a ComplexThread when pthread_create() is called.
 */
void *performSortOnThread(void* complex_thread) {
	ComplexThread* working_thread = static_cast<ComplexThread*>(complex_thread);

	switch (working_thread->m_sort_type) {
	case QUICK_SORT: {
		std::cout << "QuickSort" << std::endl;
		doQuickSort(working_thread->data_list, 0,
				(working_thread->data_list.size() - 1));
		break;
	}
	case SELECTION_SORT: {
		std::cout << "SelectionSort" << std::endl;
		doSelectionSort(working_thread->data_list);
		break;
	}
	case MERGE_SORT: {
		std::cout << "MergeSort" << std::endl;
		doMergeSort(working_thread->data_list);
		break;
	}
	case BUBBLE_SORT: {
		std::cout << "BubbleSort" << std::endl;
		doBubbleSort(working_thread->data_list);
		break;
	}
	case SHELL_SORT: {
		std::cout << "ShellSort" << std::endl;
		doShellSort(working_thread->data_list);
		break;
	}
	case INSERTION_SORT: {
		std::cout << "InsertionSort" << std::endl;
		doInsertionSort(working_thread->data_list);
		break;
	}
	}

	onThreadFinished(working_thread);
	return working_thread;
}


void onThreadFinished(ComplexThread* thread) {
	pthread_join(thread->m_thread, (void **) &thread);

	pthread_mutex_lock(&globalLock);
	masterSortedList = mergeSortedLists(thread->data_list, masterSortedList);
	pthread_mutex_unlock(&globalLock);

	--m_numOfRunningThreads;
}

/*
 * Trial 1 is designed for 2 threads.
 */
void trial1() {
	std::vector<ComplexThread*> threadList =
			partitionMasterListForSpecifiedNumOfThreads(2);

	for (int i = 0; i < threadList.size(); i++) {
		if (i == 0) {
			// Using QUICK SORT
			threadList.at(i)->m_sort_type = QUICK_SORT;
			threadList.at(i)->createAndExecuteThread();
		}
		if (i == 1) {
			// Using SELECTION SORT
			threadList.at(i)->m_sort_type = SELECTION_SORT;
			threadList.at(i)->createAndExecuteThread();
		}
	}
}

/*
 * Trial 2 is designed for 4 threads.
 */
void trial2() {
	std::vector<ComplexThread*> threadList =
			partitionMasterListForSpecifiedNumOfThreads(4);

	for (int i = 0; i < threadList.size(); i++) {
		if (i == 0) {
			// Using MERGE SORT
			threadList.at(i)->m_sort_type = MERGE_SORT;
			threadList.at(i)->createAndExecuteThread();
		}
		if (i == 1) {
			// Using BUBBLE SORT
			threadList.at(i)->m_sort_type = BUBBLE_SORT;
			threadList.at(i)->createAndExecuteThread();
		}

		if (i == 2) {
			// Using SHELL SORT
			threadList.at(i)->m_sort_type = SHELL_SORT;
			threadList.at(i)->createAndExecuteThread();
		}

		if (i == 3) {
			// Using INSERTION SORT
			threadList.at(i)->m_sort_type = INSERTION_SORT;
			threadList.at(i)->createAndExecuteThread();
		}
	}
}

void doQuickSort(std::vector<long> &unsortedList, long leftMost,
		long rightMost) {

	long tempVal;
	long i = leftMost;
	long j = rightMost;
	long pivotVal = unsortedList.at((leftMost + rightMost) / 2);

	while (i <= j) {
		while (unsortedList.at(i) < pivotVal)
			i++;
		while (unsortedList.at(j) > pivotVal)
			j--;
		if (i <= j) {
			tempVal = unsortedList.at(i);
			unsortedList.at(i) = unsortedList.at(j);
			unsortedList.at(j) = tempVal;
			i++;
			j--;
		}
	}

	if (leftMost < j) {
		doQuickSort(unsortedList, leftMost, j);
	}
	if (i < rightMost) {
		doQuickSort(unsortedList, i, rightMost);
	}
}

void doSelectionSort(std::vector<long> &unsortedList) {
	long minIndex;
	long tempVal;
	long i;
	long j;

	for (i = 0; i < unsortedList.size() - 1; i++) {
		minIndex = i;

		for (j = i + 1; j < unsortedList.size(); j++) {

			if (unsortedList.at(j) < unsortedList.at(minIndex)) {
				minIndex = j;
			}
		}

		if (minIndex != i) {
			tempVal = unsortedList.at(i);
			unsortedList.at(i) = unsortedList.at(minIndex);
			unsortedList.at(minIndex) = tempVal;
		}
	}
}

void doMergeSort(std::vector<long> &list) {
	std::vector<long> sortedList;
	std::vector<long> mLeft;
	std::vector<long> mRight;

	if (list.size() > 1) {

		long mMiddle = list.size() / 2;

		for (long i = 0; i < mMiddle; i++) {
			mLeft.push_back(list.at(i));
		}

		for (long j = mMiddle; j < list.size(); j++) {
			mRight.push_back(list.at(j));
		}

		doMergeSort(mLeft);
		doMergeSort(mRight);
		list = mergeUnsortedLists(mLeft, mRight);
	}
}

void doBubbleSort(std::vector<long> &unsortedList) {
	long tempVal, n = unsortedList.size();

	do {
		long newN = 0;
		for (long i = 1; i < n; ++i) {
			if (unsortedList.at(i - 1) > unsortedList.at(i)) {
				tempVal = unsortedList.at(i - 1);
				unsortedList.at(i - 1) = unsortedList.at(i);
				unsortedList.at(i) = tempVal;
				newN = i;
			}
		}

		n = newN;
	} while (n > 0);
}

void doShellSort(std::vector<long> &unsortedList) {
	int gaps[] = { 701, 301, 132, 57, 23, 10, 4, 1 };
	int g = sizeof(gaps) / sizeof(int);
	long temp, j;

	for (long k = 0; k < g; ++k) {
		for (long i = gaps[k]; i < unsortedList.size(); ++i) {
			temp = unsortedList.at(i);

			for (j = i; j >= gaps[k] && unsortedList.at(j - gaps[k]) > temp;
					j -= gaps[k]) {
				unsortedList.at(j) = unsortedList.at(j - gaps[k]);
			}

			unsortedList.at(j) = temp;
		}
	}
}

void doInsertionSort(std::vector<long> &unsortedList) {
	long valueToInsert, holePos;

	for (long i = 1; i < unsortedList.size(); ++i) {
		valueToInsert = unsortedList.at(i);
		holePos = i;
		while (holePos > 0 && valueToInsert < unsortedList.at(holePos - 1)) {
			unsortedList.at(holePos) = unsortedList.at(holePos - 1);
			holePos = holePos - 1;
		}
		unsortedList.at(holePos) = valueToInsert;
	}
}

std::vector<long> mergeUnsortedLists(std::vector<long> &firstList,
		std::vector<long> &secondList) {
	std::vector<long> finalSortedList;

	while (firstList.size() > 0 || secondList.size() > 0) {
		if (firstList.size() > 0 && secondList.size() > 0) {
			if (firstList.at(0) <= secondList.at(0)) {
				finalSortedList.push_back(firstList.at(0));
				firstList.erase(firstList.begin());
			} else {
				finalSortedList.push_back(secondList.at(0));
				secondList.erase(secondList.begin());
			}
		} else if (firstList.size() > 0) {
			finalSortedList.push_back(firstList.at(0));
			firstList.erase(firstList.begin());
		} else if (secondList.size() > 0) {
			finalSortedList.push_back(secondList.at(0));
			secondList.erase(secondList.begin());
		}
	}

	return finalSortedList;
}

std::vector<long> mergeSortedLists(std::vector<long> &firstList,
		std::vector<long> &secondList) {
	long firstListSize = firstList.size();
	long secondListSize = secondList.size();

	long totalSize = firstListSize + secondListSize;

	std::vector<long> finalSortedList(totalSize);

	long i = 0;
	long j = 0;
	long k = 0;

	while (i < firstListSize && j < secondListSize) {
		if (firstList.at(i) <= secondList.at(j)) {
			finalSortedList.at(k) = firstList.at(i);
			i++;
		} else {
			finalSortedList.at(k) = secondList.at(j);
			j++;
		}
		k++;
	}
	if (i < firstListSize) {
		for (int p = i; p < firstListSize; p++) {
			finalSortedList.at(k) = firstList.at(p);
			k++;
		}
	} else {
		for (int p = j; p < secondListSize; p++) {
			finalSortedList.at(k) = secondList.at(p);
			k++;
		}
	}

	return finalSortedList;
}

void writeContentsToFile(std::vector<long> &values, std::string fileName) {
	std::cout << "Writing contents to file: " << fileName << std::endl;

	std::ofstream outFile((char*) fileName.c_str());
	std::ostream_iterator<long> out_iterator(outFile, "\n");
	std::copy(values.begin(), values.end(), out_iterator);

	outFile.close();

	std::cout << "Data written to " << fileName << std::endl;
}