#ifndef DECODING_H
#define DECODING_H

#include "hufftree.h"

class Decoding
{
public:

    QString convertDecToBin(int entry);

    QString convertBinToDec(QString entry);

    void calcSizeTree(int TreeSize, int TrashSize);

    QString getFileName(QFile * src, int TreeSize);

    void buildHuffTree(QFile * src, HuffNode *TreeRoot, int treeSize, int markerIndice);

    void writeDecodeFile();

    void freeHuffTree(HuffNode * TreeRoot);

    void decodeFile(QString inFileName, QString outPath);

};

#endif // DECODING_H
