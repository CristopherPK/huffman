#ifndef DECODING_H
#define DECODING_H

#include "hufftree.h"

class Decoding
{
public:


    int TreeSize;

    int TrashSize;

    QByteArray HuffCode;

    QByteArray convertDecToBin(int entry);

    int convertBinToDec(QByteArray entry);

    void calcSizeTree(QFile * src);

    QByteArray getFileName(QFile * src);

    void buildHuffTree(QFile * src, HuffNode *TreeRoot);

    void buildHuffCode(QFile * src);

    void writeDecodeFile(QFile * out, HuffNode *TreeRoot);

    void freeHuffTree(HuffNode * TreeRoot);

    void decodeFile(QString inFileName, QString outPath);

};

#endif // DECODING_H
