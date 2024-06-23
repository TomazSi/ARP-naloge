#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int SIZE = 16;


vector<vector<char>> prepareKeyMatrix(string key) {
    vector<vector<char>> matrix(SIZE, vector<char>(SIZE, 0));
    string uniqueChars;

    //odstranim z kluca duple
    for (char c : key) {
        if (uniqueChars.find(c) == string::npos)
            uniqueChars += c;
    }

    //kluc v matrix
    int index = 0;
    for (char c : uniqueChars) {
        matrix[index / SIZE][index % SIZE] = c;
        index++;
    }

    //napolnit ostali matrix
    char ch = 32;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matrix[i][j] == 0) {
                matrix[i][j] = ch++;
            }
        }
    }

    return matrix;
}

string preprocessText(string text) {
    string processedText;
    for (size_t i = 0; i < text.size(); i += 2) {
        if (text[i] == text[i + 1]) {
            processedText += text[i];
            processedText += 'X';
            i--;
        } else {
            processedText += text.substr(i, 2);
        }
    }

    if (processedText.size() % 2 != 0) {
        processedText += char(3);
    }

    return processedText;
}


string processPair(const vector<vector<char>>& keyMatrix, char a, char b, bool encrypt) {
    int rowA, colA, rowB, colB;

    //nastavlanje a pa b
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (keyMatrix[i][j] == a) {
                rowA = i;
                colA = j;
            }
            if (keyMatrix[i][j] == b) {
                rowB = i;
                colB = j;
            }
        }
    }

    string result;
    if (rowA != rowB && colA != colB) {
        //ce sta a pa b razlicna
        result += keyMatrix[rowA][colB];
        result += keyMatrix[rowB][colA];
    } else if (rowA == rowB) {
        //ce sta v isti vrsti
        result += keyMatrix[rowA][(colA + (encrypt ? 1 : -1) + SIZE) % SIZE];
        result += keyMatrix[rowB][(colB + (encrypt ? 1 : -1) + SIZE) % SIZE];
    } else if (colA == colB) {
        //ce sta v istem stolpu
        result += keyMatrix[(rowA + (encrypt ? 1 : -1) + SIZE) % SIZE][colA];
        result += keyMatrix[(rowB + (encrypt ? 1 : -1) + SIZE) % SIZE][colB];
    }

    return result;
}

string processText(const vector<vector<char>>& keyMatrix, const string& text, bool encrypt) {
    string result="";
    for (size_t i = 0; i < text.size(); i += 2) {
        result += processPair(keyMatrix, text[i], text[i + 1], encrypt);
    }
    if(result[result.size()-1]==char(3)){
        result.pop_back();
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " <e or d> <key> <input_file>\n";
        return 1;
    }

    char mode = argv[1][0];
    string key = argv[2];
    string inputFileName = argv[3];

    //branje datoteke
    ifstream inputFile(inputFileName);
    if (!inputFile) {
        cout << "Error: Unable to open input file.\n";
        return 1;
    }

    string text((istreambuf_iterator<char>(inputFile)), (istreambuf_iterator<char>()));
    inputFile.close();

    //preprocesiranje testa s klucom
    text = preprocessText(text);
    vector<vector<char>> keyMatrix = prepareKeyMatrix(key);

    string outputText;
    if (mode == 'e') {
        outputText = processText(keyMatrix, text, true);
    } else if (mode == 'd') {
        outputText = processText(keyMatrix, text, false);
    } else {
        cout << "Invalid mode. Please use 'e' for encryption or 'd' for decryption.\n";
        return 1;
    }

    //izpisanje
    ofstream outputFile((mode == 'e' ? "out_e.txt" : "out_d.txt"));
    if (!outputFile) {
        cout << "Error: Unable to open output file.\n";
        return 1;
    }

    outputFile << outputText;
    outputFile.close();

    cout << "Operation completed successfully.\n";
    return 0;
}
