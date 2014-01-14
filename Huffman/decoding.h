#ifndef DECODING_H
#define DECODING_H

#include "hufftree.h"

class decoding
{
public:
    decoding();
    void buildHuffTree();
    void writeDecodeFile();
    void freeHuffTree(HuffNode * TreeRoot);
    void decodeFile();
};

#endif // DECODING_H
