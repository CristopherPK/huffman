#ifndef HUFFTREE_H
#define HUFFTREE_H


class HuffNode {
public:
    int freq;
    char contain;
    int sizeList;
    HuffNode *lc;
    HuffNode *rc;
    HuffNode * NodeList[255];
};

#endif // HUFFTREE_H
