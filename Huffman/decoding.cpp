#include "decoding.h"

void buildHuffTree(QFile * src, HuffNode * TreeRoot){

    char ch;
    bool warn;

    while(!src->atEnd()){

        src->getChar(&ch);

        if(ch=='0'){

            warn = true;

        } else if(ch == '('){

            if(warn==true){
                TreeRoot->contain = ch;
                TreeRoot = TreeRoot->prev;
            } else {
                TreeRoot->lc = new HuffNode;
                TreeRoot->lc->prev = TreeRoot;
                TreeRoot = TreeRoot->lc;
            }

        } else if(ch == ')'){

            if(warn==true){
                TreeRoot->contain = ch;
                TreeRoot = TreeRoot->prev;
            } else {
                TreeRoot = TreeRoot->prev;
            }

        } else {
            if(TreeRoot->lc == NULL){
                warn = false;
                TreeRoot->contain = ch;
            } else {
                TreeRoot->rc = new HuffNode;
                TreeRoot->rc->contain = ch;
            }

            TreeRoot = TreeRoot->prev;
        }
    }

}


