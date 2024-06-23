#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
vector<int> calculateKMPNext(const string& pattern) {
    int m = pattern.length();
    vector<int> kmpNext(m);

    kmpNext[0] = -1;
    kmpNext[1] = 0;
    int i = 2;
    int j = 0;

    while (i < m) {
        if (pattern[i - 1] == pattern[j]) {
            j++;
            kmpNext[i] = j;
            i++;
        } else if (j > 0) {
            j = kmpNext[j];
        } else {
            kmpNext[i] = 0;
            i++;
        }
    }
    return kmpNext;
}
void searchWithKMP(const string& pattern, const string& text, ofstream& outFile) {
    vector<int> kmpNext = calculateKMPNext(pattern);
    int m = pattern.length();
    int n = text.length();
    int i = 0;
    int j = 0;

    while (j <= n - m) {
        if (pattern[i] == text[j + i]) {
            i++;
            if (i == m) {
                outFile << j << " ";
                j += m;
                i = 0;
            }
        } else {
            j += i - kmpNext[i];
            if (i > 0) {
                i = kmpNext[i];
            }
        }
    }
}
vector<int> calculateBCH(const string& pattern) {
    int m = pattern.length();
    vector<int> bch(256, m + 1);

    for (int i = 0; i < m; i++) {
        bch[pattern[i]] = m - i;
    }
    return bch;
}
void searchWithSunday(const string& pattern, const string& text, ofstream& outFile) {
    vector<int> bch = calculateBCH(pattern);
    int m = pattern.length();
    int n = text.length();
    int i, j=0;

    while(j <= n - m) {
        for (i = 0; i < m; i++) {
            if (pattern[i] != text[j + i]) {
                break;
            }
        }
        if (i == m) {
            outFile << j << " ";
        }
        if (j < n - m) {
            j += bch[text[j + m]];
        } else {
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Uporaba: " << argv[0] << " <opcija> <iskani niz> <datoteka besedila>" << endl;
        return 1;
    }

    int option = stoi(argv[1]);
    string pattern = argv[2];
    string filename = argv[3];

    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Napaka pri odpiranju datoteke " << filename << endl;
        return 1;
    }

    string text((istreambuf_iterator<char>(inFile)), (istreambuf_iterator<char>()));

    ofstream outFile("out.txt");
    if (!outFile) {
        cerr << "Napaka pri odpiranju datoteke out.txt" << endl;
        return 1;
    }

    if (option == 0) {
        searchWithKMP(pattern, text, outFile);
    } else if (option == 1) {
        searchWithSunday(pattern, text, outFile);
    } else {
        cerr << "Neveljavna opcija" << endl;
        return 1;
    }
    return 0;
}
