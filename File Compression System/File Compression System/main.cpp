/* HUFFMAN ENCODING AND DECODING OF A TEXT FILE
* Developed by: Akaash Suresh
* September 2024
*/
#include "huffman.h"

int main() {
    Huffman huffman;
    string filename = "randomtext.txt";
	//cout << "Enter the name of the file:" << endl;
	//cin >> filename;

    // Count character frequencies
    unordered_map<char, int> frequency = huffman.frequencies(filename);
    if (frequency.empty()) {
        cerr << "Error: Unable to open or read from the file " << filename << endl;
        return 1; // Return an error code
    }

    // Build the Huffman tree
    Huffman::Node* root = huffman.buildHuffmanTree(frequency);
    if (!root) {
        cerr << "Error: Huffman tree could not be built." << endl;
        return 1;
    }

    // Generate Huffman codes
    unordered_map<char, string> huffmanCode;
    huffman.generateCodes(root, "", huffmanCode);

    // Print Huffman Codes(for debugging)
    //cout << "Huffman Codes: " << endl;
    //for (const auto& pair : huffmanCode) {
        //cout << pair.first << ": " << pair.second << endl;
    //}

    // Compress the file
    huffman.compressFile(filename, huffmanCode);
    cout << "File compressed to 'compressed.bin'." << endl;

    // Decompress the file
    huffman.decompressFile(root, "compressed.bin");
    cout << "File decompressed to 'decompressed.txt'." << endl;

    // Clean up the Huffman tree
    huffman.clearTree(root);

    return 0;
}
