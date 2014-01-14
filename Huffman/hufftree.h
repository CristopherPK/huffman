#ifndef HUFFTREE_H
#define HUFFTREE_H

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QBitArray>

class HuffNode {
public:
    int freq;
    bool isLeaf;
    unsigned char contain;
    QBitArray code;
    HuffNode *lc;
    HuffNode *rc;
    HuffNode *prev;
};

#endif // HUFFTREE_H
