#ifndef DSVECTORNOMS
#define DSVECTORNOMS

#include "Utility.h"
#include <stdexcept>
#include <initializer_list>
#include <cmath>
#include <chrono>
#include <iostream>

using namespace std;

template <class T>
class DSVectorNoMS
{
private:
	int cap;
	int _size;
	T* data;

public:
    DSVectorNoMS() {
        cout << "Default Constructor Called\n";
		this->cap = 10;
		this->_size = 0;
		this->data = new T[cap];
	}

    DSVectorNoMS(const T& elem, int numcpys) : DSVectorNoMS<T>(numcpys) {
        cout << "Overloaded Constructor Called\n";
        for (int i = 0; i < numcpys; i++) {
            DSVectorNoMS::append(elem);
        }
    }

    DSVectorNoMS(const T& elem, int numcpys, chrono::high_resolution_clock::time_point& t) : DSVectorNoMS<T>(numcpys) {
        cout << "Overloaded Constructor Called\n";
        for (int i = 0; i < numcpys; i++) {
            DSVectorNoMS::append(elem);
        }
        t = chrono::high_resolution_clock::now();
    }

    DSVectorNoMS(int cap) {
		this->_size = 0;
        this->cap = cap;
		this->data = new T[cap];
	}

    DSVectorNoMS(const initializer_list<T>& iList) {
		this->_size = iList.size();
        this->cap = this->_size + 1;
		this->data = new T[this->cap];
		int i = 0;
		for (T elem : iList) {
			this->data[i] = elem;
			i++;
		}
	}

    DSVectorNoMS(const DSVectorNoMS<T>& rhs) {
        cout << "Copy Constructor Called\n";
		this->cap = rhs.cap;
		this->_size = rhs._size;
		this->data = memcpy(rhs.data, rhs._size, rhs.cap);
	}

//No Move Semantics
//    DSVectorNoMS(DSVectorNoMS<T>&& rhs) {
//        cout << "Move Constructor Called\n";
//        this->cap = rhs.cap;
//        this->_size = rhs._size;
//        rhs._size = 0;
//        //steal rhs data
//        this->data = rhs.data;
//        rhs.data = nullptr;
//    }

    DSVectorNoMS<T>& operator=(const DSVectorNoMS<T>& rhs) {
        cout << "Copy Assignment Operator Called\n";
		if (this != &rhs) { //avoid self-assignment
			if (this->data != nullptr) {
				delete[] this->data;
			}
			this->cap = rhs.cap;
			this->_size = rhs._size;
			this->data = memcpy(rhs.data, rhs._size, rhs.cap);
		}
		return *this;
	}

//No Move Semantics
//    DSVectorNoMS<T>& operator=(DSVectorNoMS<T>&& rhs) {
//        cout << "Move Assignment Operator Called\n";
//        if(this != &rhs) { //avoid self-assignment
//            if (this->data != nullptr) {
//                delete[] this->data;
//            }
//            this->cap = rhs.cap;
//            this->_size = rhs._size;
//            //steal rhs data
//            rhs._size = 0;
//            this->data = rhs.data;
//            rhs.data = nullptr;
//        }
//        return *this;
//    }

    DSVectorNoMS<T>& operator=(const initializer_list<T>& iList) {
		this->_size = iList.size();
        this->cap = this->size + 1;
		this->data = new T[this->cap];
		int i = 0;
		for (T elem : iList) {
			this->data[i] = elem;
			i++;
		}
		return *this;
	}

    DSVectorNoMS<T>& operator+=(const DSVectorNoMS<T>& rhs) {
		int oldSize = this->_size;
		this->_size = oldSize + rhs._size;
		this->cap += rhs.cap;
		if (this->data != nullptr && rhs.data != nullptr) { //both vectors have data
            T* oldData = this->data;
			this->data = memcpy(oldData, oldSize, this->cap); //use generic copy to populate the first section of data
			for (int i = 0; i < rhs._size; i++) {
				this->data[i + oldSize] = rhs.data[i];
			}
			delete[] oldData;
		}
		else if (this->data == nullptr && rhs.data != nullptr) { //lhs vector is empty
			this->data = memcpy(rhs.data, this->_size, this->cap);
		}
		else if (this->data == nullptr && rhs.data == nullptr) { //both vectors are empty
			this->data = nullptr;
		}
		// do nothing if the rhs vector is empty
		return *this;
	}

    DSVectorNoMS<T> operator+(const DSVectorNoMS<T>& rhs) const{
        DSVectorNoMS returnVec(*this);
		returnVec += rhs; //utilize functionality in operator+=
		return returnVec;
	}

    T& at(int index) const {
		if (index >= this->_size || index < -1 * this->_size) {
            throw out_of_range("DSVectorNoMS::at Index Out of Bounds...");
		}
		if (index >= 0) {
			return this->data[index];
		}
		else {
			return this->data[this->_size + index];
		}
	}

    T& operator[](int index) const{
        return DSVectorNoMS::at(index); // call at() instead of rewriting the same function
	}

    bool operator==(const DSVectorNoMS<T>& rhs) const{
        if (this->_size != rhs._size) {
			return false;
		}
		for (int i = 0; i < this->_size; i++) {
            if (!(this->data[i] == rhs.data[i])) { //don't want to assume != has been overloaded for T
				return false;
			}
		}
		return true; // if all conditional statements have been cleared, the vectors are identical
	}

	void resize(int newCap) {
		if (newCap < 0) {
            throw out_of_range("DSVectorNoMS::resize Cannot allocate a vector of negative capactiy...");
		}
        this->cap = newCap;
		//create a new data arr and only copy up to _size or newCap-1, whichever comes first, from the old one
        this->_size = this->_size < newCap ? newCap - 1 : this->_size;
		T* oldData = this->data;
        this->data = memcpy(oldData, this->_size, this->cap);
        delete[] oldData;
	}

    int size() const {
		return this->_size;
	}

    int capacity() const {
		return this->cap;
	}

	void sortASC() {
        quickSort(this->data, 0, this->_size - 1);
	}

	void reverse() {
		int lowerBound = 0;
		int upperBound = this->size - 1;
		while (lowerBound < upperBound) {
			T temp = this->data[lowerBound];
			this->data[lowerBound] = this->data[upperBound];
			this->data[upperBound] = temp;
			lowerBound++;
			upperBound--;
		}
	}

	void sortDESC() {
        DSVectorNoMS::sortASC();
        DSVectorNoMS::reverse();
	}

    void prepend(const T& elem) {
		this->_size++;
        this->cap *= this->_size >= this->cap ? 2 : 1; //double capacity if new size exceeds old capacity
		T* oldData = this->data;
		this->data = new T[this->cap];
		this->data[0] = elem;
		for (int i = 1; i < this->_size; i++) {
			this->data[i] = oldData[i - 1];
		}
		if (oldData != nullptr) {
			delete[] oldData; //could be prepending to an empty vector
		}
	}

    void append(const T& elem) {
		this->_size++;
        if (this->_size >= this->cap) {
            this->cap *= 2; //double capacity
            T* oldData = this->data;
            this->data = memcpy(oldData, this->_size - 1, this->cap);
            this->data[this->_size - 1] = elem;
            if (oldData != nullptr) {
                delete[] oldData; //could be appending to an empty vector
            }
        }
        else {
            this->data[this->_size - 1] = elem;
        }

	}

    int find(const T& elem) const { //not going to assume vector has been sorted
		for (int i = 0; i < this->_size; i++) {
			if (this->data[i] == elem) {
				return i;
			}
		}
        return -1; //to indicate that the element was not found
	}

    int findBS(const T& elem) const { //binary search, assuming that the vector has been sorted
        //assuming T has implemented < and ==
        return binarySearch(this->data, 0, this->_size - 1, elem);
    }

    DSVectorNoMS<T> slice(int lowerBound, int upperBound) const {
		if (upperBound >= this->_size || upperBound < -1 * this->_size) {
            throw out_of_range("DSVectorNoMS::slice upper bound out of range");
		}
		if (lowerBound < 0) { //standardize lowerBound for negative indexing
			lowerBound = this->_size + lowerBound;
		}
		if (upperBound < 0) { //standardize upperBound for negative indexing
			upperBound = this->_size + upperBound;
		}
		if (lowerBound > upperBound) {
            throw out_of_range("DSVectorNoMS::slice lowerbound > upperbound");
		}
		
        DSVectorNoMS<T> returnVec(upperBound - lowerBound + 1);
		for (int i = lowerBound; i <= upperBound; i++) {
			returnVec.append(this->data[i]);
		}
		return returnVec;
	}

	void remove(int index) {
		if (index >= this->_size || index < -1 * this->_size) {
            throw out_of_range("DSVectorNoMS::Remove index out of bounds");
		}
		if (index < 0) {
            index = this->_size + index; //standardize index from negative indexing
		}

		T* oldData = this->data;
		this->data = new T[this->cap];
		for (int i = 0; i < index; i++) {
			this->data[i] = oldData[i];
		}
		for (int i = index + 1; i < this->_size; i++) {
			this->data[i - 1] = oldData[i];
		}
		this->_size--;
		delete[] oldData;
	}

    void insert(T elem, int index) {
        if (index > this->_size || index < -1 * this->_size) {
            throw out_of_range("DSVectorNoMS::insert index out of bounds");
        }
        if (index < 0) {
            index = this->_size + index; //standardized negative indexing
        }

        if (index == 0) { //prepending
            DSVectorNoMS::prepend(elem);
            return;
        }
        else if (index == this->_size) { //appending
            DSVectorNoMS::append(elem);
            return;
        }

        this->_size++;
        this->cap *= this->_size >= this->cap ? 2 : 1; //double capacity if new size exceeds old capacity
        T* oldData = this->data;
        this->data = new T[this->cap];
        for (int i = 0; i < index; i++) {
           this->data[i] = oldData[i];
        }
        this->data[index] = elem;
        for (int i = index + 1; i < this->_size; i++) {
            this->data[i] = oldData[i - 1];
        }
        delete[] oldData;
    }

    void insertInOrder(T elem) {
        int insertionIndex = findNextHighest(this->data, 0, this->_size - 1, elem);
        DSVectorNoMS::insert(elem, insertionIndex);
    }

    ~DSVectorNoMS() {
        cout << "Destructor Called\n";
        if (this->data != nullptr) {
            delete[] data;
        }
	}
};

#endif
