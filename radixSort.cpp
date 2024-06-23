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
void BinaryRadixSort(vector<int>& A){
    const int numBits=8;
    const int numIterations=numBits;
    vector<int> sorted(A.size());
    for(int k=0;k<numIterations;k++){
        vector<int> D(A.size());
        for (int i = 0; i < A.size(); ++i) {
            D[i] = (A[i] >> k) & 1;
        }
        vector<int> C(2, 0);
        for (int i = 0; i < D.size(); ++i) {
            ++C[D[i]];
        }
        for(int i=1;i<C.size();i++){
            C[i]+=C[i-1];
        }
        for(int i=A.size()-1;i>=0;i--){
            sorted[--C[D[i]]]=A[i];
        }
        A=sorted;
    }
}

int main(int argc, const char* argv[]) {
	vector<int> A;

	if (argc < 2) {
        cout << "Usage: dn1 <input_file>" << endl;
        return 0;
    }

    if (!Branje_Stevil(A, argv[1])) {
        cout << "Error reading input file." << endl;
        return 0;
    }
    BinaryRadixSort(A);
	Izpis_Stevil(&A[0],A.size());

	return 0;
}
