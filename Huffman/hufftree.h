#ifndef HUFFTREE_H
#define HUFFTREE_H

class HuffNode {
public:
    int freq;
    char contain;
    bool isLeaf;
    HuffNode *lc;
    HuffNode *rc;
};

#endif // HUFFTREE_H
