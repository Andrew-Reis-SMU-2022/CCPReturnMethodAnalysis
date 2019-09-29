#ifndef UTILITY_H
#define UTILITY_H



template <class T>
T* memcpy(T* cpyData, int size, int cap) {
	T* newData = new T[cap];
	for (int i = 0; i < size; i++) {
		newData[i] = cpyData[i];
	}
	return newData;
}

template <class T>
void insertionSort(T* arr, int arrSize) {
	//assumes operator has been overriden for the object
	for (int i = 1; i < arrSize; i++) {
		int j = i - 1;
		while (j >= 0 && arr[j + 1] < arr[j]) {
			T temp = arr[j + 1];
			arr[j + 1] = arr[j];
			arr[j] = temp;
			j--;
		}
	}
}

template <class T>
int binarySearch(T* arr, int lowerBound, int upperBound, T elem){
    if (lowerBound < upperBound) {
        int mid = (lowerBound + upperBound) / 2;

        if (arr[mid] == elem) {
            return mid; //base case
        }
        else if (elem < arr[mid]) {
            return binarySearch(arr, lowerBound, mid - 1, elem);
        }
        else {
            return binarySearch(arr, mid + 1, upperBound, elem);
        }
    }
    else if (upperBound == lowerBound) {
        if (arr[upperBound] == elem) {
            return upperBound; //base case
        }
    }
    return -1; //if elem was not found
}

template<class T>
int checkLower(T* arr, int index, T elem) { //utility function for findNextHighest
    for (int i = index - 1; i >= 0; i--) {
        if (arr[i] < elem) {
            return i + 1;
        }
    }
    return 0; //the lowest value in the range is still larger than the passed elem
}

template <class T>
int findNextHighest(T* arr, int lowerBound, int upperBound, T elem) { //implemented using a sudo recursive Binary Search
    //check edge cases first
    if (elem < arr[lowerBound] || elem == arr[lowerBound]) { //don't want to assume <= has been overloaded for T
        return lowerBound; //going to prepend elem
    }
    else if (arr[upperBound] < elem) {
        return upperBound + 1; //going to append elem
    }

    if (lowerBound < upperBound) {
        int mid = (lowerBound + upperBound) / 2;

        if (arr[mid] == elem) {
            return checkLower(arr, mid, elem); //base
        }
        else if (elem < arr[mid]) {
            return findNextHighest(arr, lowerBound, mid - 1, elem);
        }
        else {
            return findNextHighest(arr, mid + 1, upperBound, elem);
        }
    }
    else if (lowerBound == upperBound) {
        return checkLower(arr, lowerBound, elem);
    }

}

template <class T>
int partition(T* arr, int lowerBound, int upperBound) {
    T pivot = arr[upperBound];
    int i = lowerBound - 1; //low side index

    for (int j = lowerBound; j < upperBound; j++) {
        if (arr[j] < pivot) {
            i++;
            T temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    i++;
    T temp = arr[i];
    arr[i] = arr[upperBound];
    arr[upperBound] = temp;
    return i;
}

template <class T>
void quickSort(T* arr, int lowerBound, int upperBound) {
    if (lowerBound< upperBound) {
       int partitionIndex = partition(arr, lowerBound, upperBound);
       quickSort(arr, lowerBound, partitionIndex - 1);
       quickSort(arr, partitionIndex + 1, upperBound);
    }
}

#endif // !UTILITY

