#include "huffman.h"

unordered_map<char, int> Huffman::frequencies(const string& filename) {
    ifstream file(filename, ios::in);
    if (!file.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
        return {};
    }
    unordered_map<char, int> frequency;
    char ch;

    while (file.get(ch)) {
        frequency[ch]++;
    }

    file.close();
    return frequency;
}

Huffman::Node* Huffman::buildHuffmanTree(const unordered_map<char, int>& frequency) {
    // Custom comparator for priority queue that uses Node's comparison method
    auto compare = [](Node* l, Node* r) {
        return *l > *r; // Use Node's operator>
        };

    priority_queue<Node*, vector<Node*>, decltype(compare)> pq(compare);

    // Creates a leaf node for each character and adds the to priority queue
    for (const auto& pair : frequency) {
        pq.push(new Node(pair.first, pair.second));
    }

    // If the frequency map is empty
    if (pq.empty()) return nullptr;

    // If there is only one character in the input file
    if (pq.size() == 1) {
        Node* singleNode = pq.top();
        pq.pop();
        return new Node(singleNode->ch, singleNode->freq);
    }

    // Build the Huffman tree
    while (pq.size() != 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* newNode = new Node('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }

    return pq.top(); // Returns the root of the tree
}

// Generates Huffman codes recursively
void Huffman::generateCodes(Node* root, const string& str, unordered_map<char, string>& huffmanCode) {
    if (root == nullptr) {
        return;
    }

    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    generateCodes(root->left, str + "0", huffmanCode);
    generateCodes(root->right, str + "1", huffmanCode);
}

void Huffman::compressFile(const string& filename, const unordered_map<char, string>& huffmanCode) {
    ifstream inFile(filename, ios::binary);
    ofstream outFile("compressed.bin", ios::binary);

    string encodedString = "";
    char ch;

    // Convert file contents to binary string using Huffman codes
    while (inFile.get(ch)) {
        encodedString += huffmanCode.at(ch);
    }

    // Write the encoded string as bytes
    size_t length = encodedString.length();
    for (size_t i = 0; i < length; i += 8) {
        string byteString = encodedString.substr(i, 8);
        // Pad the last byte if it's less than 8 bits
        if (byteString.size() < 8) {
            byteString.append(8 - byteString.size(), '0');
        }
        bitset<8> byte(byteString);
        outFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }

    inFile.close();
    outFile.close();
}

void Huffman::decompressFile(Node* root, const string& filename) {
    ifstream inFile(filename, ios::binary);
    ofstream outFile("decompressed.txt");

    Node* currentNode = root;
    char byte;

    // Read binary file and decode using Huffman tree
    while (inFile.get(byte)) {
        bitset<8> bits(byte);
        for (size_t i = 0; i < bits.size(); i++) {
            currentNode = bits[i] ? currentNode->right : currentNode->left;
            if (!currentNode->left && !currentNode->right) {
                outFile.put(currentNode->ch);
                currentNode = root;
            }
        }
    }

    inFile.close();
    outFile.close();
}

// Frees up the Huffman tree memory
void Huffman::clearTree(Node* root) {
    if (!root) return;
    clearTree(root->left);
    clearTree(root->right);
    delete root;
}
