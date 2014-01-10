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
    char contain;
    bool isLeaf;
    QString code;
    HuffNode *lc;
    HuffNode *rc;
    HuffNode *prev;
};

#endif // HUFFTREE_H
