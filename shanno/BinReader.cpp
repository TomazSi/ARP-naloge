#include <fstream>
#include <string>
#include <vector>

class BinReader {
private:
    std::ifstream input;
    unsigned char buffer;
    int bufferPos;

    void fillBuffer() {
        buffer = input.get();
        bufferPos = 0;
    }

public:
    BinReader(std::string path) : buffer(0), bufferPos(8) {
        input.open(path, std::ios::binary);
        if (!input.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
    }

    bool readBit() {
        if (bufferPos == 8) {
            fillBuffer();
            if (input.eof()) {  // Preverite, ali ste na koncu datoteke
                throw std::ios_base::failure("End of file reached");
            }
        }
        bool bit = (buffer >> (7 - bufferPos)) & 1;
        bufferPos++;
        return bit;
    }

    char readByte() {
        char byte;
        input.read(&byte, 1);
        return byte;
    }

    int readInt() {
        int value;
        input.read(reinterpret_cast<char*>(&value), sizeof(value));
        return value;
    }

    float readFloat() {
        float value;
        input.read(reinterpret_cast<char*>(&value), sizeof(value));
        return value;
    }

    ~BinReader() {
        if (input.is_open()) {
            input.close();
        }
    }
};
