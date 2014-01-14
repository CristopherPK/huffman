#ifndef ENCODING_H
#define ENCODING_H


#include "hufftree.h"

class Encoding
{
public:    
    int sizeList;
    QString HuffCode;
    void calcFreqChar(QFile * src, int freqList[]);
    void buildNodeList(HuffNode *NodeList[], int freqList[]);
    void sortNodeList(HuffNode *NodeList[]);
    void buildHuffTree(HuffNode *NodeList[]);
    void buildCopyList(HuffNode *NodeList[], HuffNode *ListCopy[]);
    void writeHuffTree(QFile * out, HuffNode *TreeRoot, HuffNode * CodeList[]);
    void writeHuffCode(QFile * src, QFile *out, HuffNode * CodeList[]);
    void freeHuffTree(HuffNode *NodeList[]);
    void encodeFile(QString inFileName, QString outFileName);
};

#endif // ENCODING_H
