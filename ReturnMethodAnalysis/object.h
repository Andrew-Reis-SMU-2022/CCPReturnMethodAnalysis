#ifndef OBJECT_H
#define OBJECT_H

using namespace std;
#include <iostream>

template <class T>
class object
{
private:
	T* data;
public:
	object() {
		cout << "Default Constructor Called\n";
		this->data = nullptr;
	}

	object(const T& data) {
		cout << "Overloaded Constructor Called\n";
		this->data = new T;
		*(this->data) = data;
	}

	object(const object& rhs) {
		cout << "Copy Constructor Called\n";
		this->data = new T;
		*(this->data) = *(rhs.data);
	}

	object& operator=(const object& rhs) {
		cout << "Copy Assignment Operator Called\n";
		if (this != &rhs) {
            if (rhs.data != nullptr) {
				if (this->data == nullptr) {
					this->data = new T;
				}
				*(this->data) = *(rhs.data);
			}
		}
		return *this;
	}

	object(object&& rhs) {
		cout << "Move Constructor Called\n";
		this->data = rhs.data;
		rhs.data = nullptr;
	}

	object& operator=(object&& rhs) {
		cout << "Move Assignment Operator Called";
		if (this != &rhs) {
			if (this->data != nullptr) {
				delete this->data;
			}
			this->data = rhs.data;
			rhs.data = nullptr;
		}
		return *this;
	}

	~object() {
		cout << "Destructor Called\n";
		if (this->data != nullptr) {
			delete this->data;
		}
	}

	//interesting that operator+ and operator- are returned by value
	object operator+(const object& rhs) {
		T newData = *(this->data) + *(rhs.data);
		return object(newData);
	}

	object operator-(const object& rhs) {
		T newData = *(this->data) - *(rhs.data);
		return object(newData);
	}

	object& operator+=(const object& rhs) {
		if (this->data != nullptr && rhs.data != nullptr) {
			*(this->data) = *(this->data) + *(rhs.data);
		}
		else if (this->data == nullptr && rhs.data != nullptr) {
			this->data = new T;
			*(this->data) = *(rhs.data);
		}
		return *this;
	}

	object& operator-=(const object& rhs) {
		if (this->data != nullptr && rhs.data != nullptr) {
			*(this->data) = *(this->data) - *(rhs.data);
		}
		return *this;
	}

	friend ostream& operator<<(ostream& os, const object& rhs) {
		os << *(rhs.data);
		return os;
	}

	friend istream& operator>>(istream& is, const object& rhs) {
		is >> *(rhs.data);
		return is;
	}

	bool operator<(const object& rhs) {
		return *(this->data) < *(rhs.data);
	}

	bool operator<=(const object& rhs) {
		return *(this->data) <= *(rhs.data);
	}

	bool operator>(const object& rhs) {
		return *(this->data) > *(rhs.data);
	}

	bool operator>=(const object& rhs) {
		return *(this->data) >= *(rhs.data);
	}

	bool operator==(const object& rhs) {
		return *(this->data) == *(rhs.data);
	}

	bool operator!=(const object& rhs) {
		return *(this->data) != 8(rhs.data);
	}

};

#endif
