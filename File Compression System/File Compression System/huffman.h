#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <stack>
#include <vector>
#include <bitset>
using namespace std;

class Huffman{
public:
    struct Node {
        char ch;
        int freq;
        Node* left;
        Node* right;

        Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
        // Operator overload for priority queue that builds the Huffman tree
        bool operator>(const Node& other) const {
            return this->freq > other.freq;
        }
    };
	unordered_map<char, int> frequencies(const string& filename);
    Node* buildHuffmanTree(const unordered_map<char, int>& frequency);
    void generateCodes(Node* root, const string& str, unordered_map<char, string>& huffmanCode);
    void compressFile(const string& filename, const unordered_map<char, string>& huffmanCode);
    void decompressFile(Node* root, const string& filename);
    void clearTree(Node* root);
private:
};

