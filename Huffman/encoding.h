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
    int freqList[256];
    int fileSize;
    HuffNode * curr;
    void calcFreqChar(QString entry);
    void buildNodeList();
    void sortNodeList();

};

#endif // ENCODING_H
