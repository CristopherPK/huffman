#ifndef HUFFTREE_H
#define HUFFTREE_H

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QBitArray>
#include <math.h>

class HuffNode {
public:
    int freq;
    bool isLeaf;
    bool visited;
    unsigned char contain;
    QString code;
    HuffNode *lc;
    HuffNode *rc;
    HuffNode *prev;
};

#endif // HUFFTREE_H
