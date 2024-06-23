#include <fstream>
#include <string>

class BinWriter {
private:
    std::ofstream output;
    unsigned char buffer;
    int bufferPos;

    void flushBuffer() {
        if (bufferPos > 0) {
            output.put(buffer);
            buffer = 0;
            bufferPos = 0;
        }
    }

public:
    BinWriter(std::string path) : buffer(0), bufferPos(0) {
        output.open(path, std::ios::binary);
        if (!output.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
    }

    void writeBit(bool bit) {
        if (bufferPos == 8) {
            flushBuffer();
        }
        if (bit) {
            buffer |= (1 << (7 - bufferPos));
        }
        bufferPos++;
    }

    void writeByte(char byte) {
        flushBuffer();
        output.put(byte);
    }

    void writeInt(int value) {
        flushBuffer();
        output.write(reinterpret_cast<char*>(&value), sizeof(value));
    }

    void writeFloat(float value) {
        flushBuffer();
        output.write(reinterpret_cast<char*>(&value), sizeof(value));
    }

    ~BinWriter() {
        flushBuffer();
        if (output.is_open()) {
            output.close();
        }
    }
};
