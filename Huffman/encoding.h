#ifndef ENCODING_H
#define ENCODING_H


#include "hufftree.h"

class Encoding
{
public:

    QString HuffTree;
    QString HuffCode;

    int tsize;
    int sizeList;

    //Encoding Functions.
    void initHuffList(HuffNode * NodeList[], HuffNode * ListCopy[],int freqList[]);

    void calcFreqChar(QFile * src, int freqList[]);

    void buildNodeList(HuffNode *NodeList[], HuffNode *ListCopy[], int freqList[]);

    void sortNodeList(HuffNode *NodeList[]);

    void buildHuffTree(HuffNode *NodeList[]);

    void writeHuffTree(HuffNode *TreeRoot, HuffNode * CodeList[]);

    void writeHuffCode(QFile * src, HuffNode * CodeList[]);

    QString convertDecToBin(int entry);

    QString convertBinToDec(QString entry);

    void freeHuffTree(HuffNode *NodeList[], HuffNode *ListCopy[]);


    //Global Function.
    void encodeFile(QString inFileName, QString outFileName);
};

#endif // ENCODING_H
