#include <iostream>
#include "object.h"
#include "DSVector.h"
#include <chrono>
#include <string>
#include <fstream>
#include <cmath>
#include "DSVectorNoMS.h"

const int NUM_ITERATIONS = 100;
const int MAX_POWER = 7;

using namespace std;

DSVector<int> returnRVO(const int& numcpys, chrono::high_resolution_clock::time_point& t) {
    return DSVector<int>(1, numcpys, t);
}

DSVector<int> returnNRVO(const int& numcpys, chrono::high_resolution_clock::time_point& t) {
    DSVector<int> a(1, numcpys);
    t = chrono::high_resolution_clock::now();
    return a;
}

DSVector<int>& returnReference(DSVector<int>& a, chrono::high_resolution_clock::time_point& t) {
    t = chrono::high_resolution_clock::now();
    return a;
}

DSVectorNoMS<int> returnCpy(const int& numcpys, chrono::high_resolution_clock::time_point& t) {
    return move(DSVectorNoMS<int>(1, numcpys, t)); //calling move without MS implemented prevents copy elision
}

DSVector<int> returnMS(const int& numcpys, chrono::high_resolution_clock::time_point& t) {
    return move(DSVector<int>(1, numcpys, t));
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        cout << "Invalid number of command line arguments...\n";
        cout << argc << " arguments passed.\n";
        return -1;
    }
    string mode = argv[1];
    if (mode != "rvo" && mode != "nrvo" && mode != "ref" && mode != "cpy" && mode != "ms") {
        cout << "Invalid mode entered: " << mode << '\n';
        cout << "Acceptable modes are: rvo, nrvo, ref, cpy\n";
        return -1;
    }

    ofstream fout(mode + " output.csv");
    fout << "DSVectorSize,responseTime_ns\n";
    //fout << fixed;

    if (mode == "rvo") {
        for (int i = 0; i <= MAX_POWER; i++) {
            for (int j = 0; j < NUM_ITERATIONS; ++j) {
                chrono::high_resolution_clock::time_point t;
                DSVector<int> a = returnRVO(static_cast<int>(pow(10, i)), t);
                auto diff = chrono::high_resolution_clock::now() - t;
                fout << static_cast<int>(pow(10, i)) << ',' << chrono::duration<int, nano>(diff).count() << '\n';
            }
        }
    }

    if (mode == "nrvo") {
        for (int i = 0; i <= MAX_POWER; i++) {
            for (int j = 0; j < NUM_ITERATIONS; ++j) {
                chrono::high_resolution_clock::time_point t;
                DSVector<int> a = returnNRVO(static_cast<int>(pow(10, i)), t);
                auto diff = chrono::high_resolution_clock::now() - t;
                fout << static_cast<int>(pow(10, i)) << ',' << chrono::duration<int, nano>(diff).count() << '\n';
            }
        }
    }

    if (mode == "ref") {
        for (int i = 0; i <= MAX_POWER; i++) {
            for (int j = 0; j < NUM_ITERATIONS; ++j) {
                chrono::high_resolution_clock::time_point t;
                DSVector<int> a(1, static_cast<int>(pow(10, i)));
                DSVector<int>& b = returnReference(a, t);
                auto diff = chrono::high_resolution_clock::now() - t;
                fout << static_cast<int>(pow(10, i)) << ',' << chrono::duration<int, nano>(diff).count() << '\n';
            }
        }
    }

    if (mode == "cpy") {
        for (int i = 0; i <= MAX_POWER; i++) {
            for (int j = 0; j < NUM_ITERATIONS; ++j) {
                chrono::high_resolution_clock::time_point t;
                DSVectorNoMS<int> a = returnCpy(static_cast<int>(pow(10, i)), t);
                auto diff = chrono::high_resolution_clock::now() - t;
                fout << static_cast<int>(pow(10, i)) << ',' << chrono::duration<int, nano>(diff).count() << '\n';
            }
        }
    }

    if (mode == "ms") {
        for (int i = 0; i <= MAX_POWER; i++) {
            for (int j = 0; j < NUM_ITERATIONS; ++j) {
                chrono::high_resolution_clock::time_point t;
                DSVector<int> a = returnMS(1, t);
                auto diff = chrono::high_resolution_clock::now() - t;
                fout << static_cast<int>(pow(10, i)) << ',' << chrono::duration<int, nano>(diff).count() << '\n';
            }
        }
    }

    fout.close();
    return 0;
}
