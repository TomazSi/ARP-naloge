#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "BinReader.cpp"
#include "BinWriter.cpp"

long getFileSize(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file to get size");
    }
    return file.tellg();
}

struct Node {
    char symbol;
    int frequency;
    std::string code;
};

bool compareNodes(Node a, Node b) {
    return a.frequency > b.frequency;
}

void generateCodes(std::vector<Node>& nodes, int start, int end, std::string prefix) {
    if (start == end) {
        nodes[start].code = prefix;
        return;
    }

    int total = 0;
    for (int i = start; i <= end; ++i) {
        total += nodes[i].frequency;
    }

    int halfTotal = total / 2;
    int sum = 0;
    int mid = start;
    for (int i = start; i <= end; ++i) {
        sum += nodes[i].frequency;
        if (sum >= halfTotal) {
            mid = i;
            break;
        }
    }

    for (int i = start; i <= mid; ++i) {
        nodes[i].code = prefix + "0";
    }
    for (int i = mid + 1; i <= end; ++i) {
        nodes[i].code = prefix + "1";
    }

    generateCodes(nodes, start, mid, prefix + "0");
    generateCodes(nodes, mid + 1, end, prefix + "1");
}

void compressFile(std::string inputPath, std::string outputPath) {
    std::ifstream input(inputPath, std::ios::binary);
    if (!input.is_open()) {
        throw std::runtime_error("Failed to open input file");
    }

    std::unordered_map<char, int> frequencyMap;
    char ch;
    while (input.get(ch)) {
        frequencyMap[ch]++;
    }
    input.clear();
    input.seekg(0, std::ios::beg);

    std::vector<Node> nodes;
    for (auto& pair : frequencyMap) {
        nodes.push_back(Node{pair.first, pair.second, ""});
    }
    std::sort(nodes.begin(), nodes.end(), compareNodes);

    generateCodes(nodes, 0, nodes.size() - 1, "");

    std::unordered_map<char, std::string> codeMap;
    for (const auto& node : nodes) {
        codeMap[node.symbol] = node.code;
    }

    BinWriter writer(outputPath);
    writer.writeInt(nodes.size());
    for (const auto& node : nodes) {
        writer.writeByte(node.symbol);
        writer.writeInt(node.frequency);
    }

    while (input.get(ch)) {
        std::string code = codeMap[ch];
        for (char bit : code) {
            writer.writeBit(bit == '1');
        }
    }

    input.close();
    writer.~BinWriter();

    long originalSize = getFileSize(inputPath);
    long compressedSize = getFileSize(outputPath);
    double compressionRatio = (compressedSize == 0) ? 0 : (double)originalSize / compressedSize;

    std::cout << "Original size: " << originalSize << " bytes" << std::endl;
    std::cout << "Compressed size: " << compressedSize << " bytes" << std::endl;
    std::cout << "Compression ratio: " << compressionRatio << std::endl;
}

void decompressFile(std::string inputPath, std::string outputPath) {
    BinReader reader(inputPath);
    int nodeCount = reader.readInt();
    std::vector<Node> nodes(nodeCount);
    for (int i = 0; i < nodeCount; ++i) {
        nodes[i].symbol = reader.readByte();
        nodes[i].frequency = reader.readInt();
    }

    generateCodes(nodes, 0, nodeCount - 1, "");

    std::unordered_map<std::string, char> codeMap;
    for (const auto& node : nodes) {
        codeMap[node.code] = node.symbol;
    }

    std::ofstream output(outputPath, std::ios::binary);
    if (!output.is_open()) {
        throw std::runtime_error("Failed to open output file");
    }

    std::string currentCode;
    while (true) {
        try {
            bool bit = reader.readBit();
            currentCode += bit ? "1" : "0";
            if (codeMap.find(currentCode) != codeMap.end()) {
                output.put(codeMap[currentCode]);
                currentCode.clear();
            }
        } catch (std::ios_base::failure&) {
            break;
        }
    }

    output.close();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <c|d> <file>" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string filePath = argv[2];

    try {
        if (mode == "c") {
            compressFile(filePath, "out.bin");
            std::cout << "Compression completed." << std::endl;
        } else if (mode == "d") {
            decompressFile("out.bin", "out_d.bin");
            std::cout << "Decompression completed." << std::endl;
        } else {
            std::cerr << "Invalid mode. Use 'c' for compression and 'd' for decompression." << std::endl;
            return 1;
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
