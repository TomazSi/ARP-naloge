#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
int Minimum;
bool Branje_Stevil(vector<int> &vec, const char s[]) {
	ifstream input(s);
	int st;

	if (!input.is_open()) {
		return false;
	}

	while (!input.eof()) {
		input >> st;
		vec.push_back(st);
		while (isspace(input.peek())) input.get();
	}
	input.close();
	return true;
}

void Izpis_Stevil(int* polje, unsigned int velikost) {
	ofstream output("out.txt");

	for (int i = 0; i<velikost; i++)
		output << polje[i] << ' ';
}

void PodporaNegativnih(vector<int> &A) {
    int minVal = A[0];
    for (int i = 1; i < A.size(); i++) {
        if (A[i] < minVal) {
            minVal = A[i];
        }
    }

    if (minVal < 0) {
        for (int i = 0; i < A.size(); i++) {
            A[i] -= minVal;
        }
    } else {
        minVal = 0;
    }

    Minimum = minVal;
}

void RePodporaNegativnih(vector<int> &A) {
    for (int i = 0; i < A.size(); i++) {
        A[i] += Minimum;
    }
}
void CountingSort(vector<int>& A) {
    int maxVal = *max_element(A.begin(), A.end());
    vector<int> C(maxVal + 1, 0);
    vector<int> B(A.size());

    for (int i = 0; i < A.size(); i++) {
        C[A[i]]++;
    }

    for (int i = 1; i <= maxVal; i++) {
        C[i] += C[i - 1];
    }

    for (int i = A.size() - 1; i >= 0; i--) {
        B[C[A[i]] - 1] = A[i];
        C[A[i]]--;
    }

    A = B;
}

void RomanSort(vector<int>& A) {
    int maxVal = *max_element(A.begin(), A.end());
    vector<int> C(maxVal + 1, 0);
    vector<int> B(A.size());

    for (int i = 0; i < A.size(); i++) {
        C[A[i]]++;
    }

    int idx = 0;
    for (int i = 0; i <= maxVal; i++) {
        for (int j = 0; j < C[i]; j++) {
            B[idx++] = i;
        }
    }

    A = B;
}


int main(int argc, const char* argv[]) {
	vector<int> A;

	if (argc < 3) return 0;
	if (!Branje_Stevil(A, argv[2])) return 0;
    PodporaNegativnih(A);
	if (argv[1][0] == '0') {
		//counting sort
        CountingSort(A);
	}
	else {
		//Roman sort
        RomanSort(A);
	}
    RePodporaNegativnih(A);
	Izpis_Stevil(&A[0],A.size());

	return 0;
}
