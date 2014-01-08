#ifndef ENCODING_H
#define ENCODING_H

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include "hufftree.h"

class Encoding
{
public:    
    char ch;
    int freqList[255];
    int fileSize;
    int sizeList;
    HuffNode * NodeList[255];
    void calcFreqChar(QFile * src);
    void buildNodeList();
    void sortNodeList();
    void buildHuffTree();
    void writeHuffTree();
};

#endif // ENCODING_H
