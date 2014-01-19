#ifndef DECODING_H
#define DECODING_H

#include "hufftree.h"

class Decoding
{
public:

    int TreeSize;

    int TrashSize;

    HuffNode * TreeRoot;

    QByteArray HuffCode;

    QByteArray convertDecToBin(int entry);

    int convertBinToDec(QByteArray entry);

    void calcSizeTree(QFile * src);

    QByteArray getFileName(QFile * src);

    void buildHuffTree(QFile * src);

    void buildHuffCode(QFile * src);

    void writeDecodeFile(QFile * out);

    void freeHuffTree(HuffNode * TreeRoot);

    void decodeFile(QString inFileName, QString outPath);

};

#endif // DECODING_H
