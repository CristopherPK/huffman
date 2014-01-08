#include "encoding.h"

void Encoding::calcFreqChar(QFile * src){

    if(!src->open(QIODevice::ReadOnly))
    //Inicializer array of char frequency
    for(int i=0; i<=255; i++){
        freqList[i] = 0;
    }

    int fsz = 0;

    //Counting char frequency
    while(!src->atEnd()){
        src->getChar(&ch);

        int asc = 0;
        asc = (int) ch;
        if(asc<0){
            asc += 256;
        }

        freqList[asc]++;

        fsz++;
    }

    qDebug() << fsz << "file size";

    qDebug() << "Frequecy finished.";
}

void Encoding::buildNodeList(){

    //Size of list initialized.
    sizeList = 0;

    //Filling list.
    for(int i=0; i<=255; i++){
        //
        if(freqList[i] != 0){
            NodeList[sizeList] = new HuffNode;
            NodeList[sizeList]->contain = (char) i;
            NodeList[sizeList]->freq = freqList[i];
            NodeList[sizeList]->lc = NULL;
            NodeList[sizeList]->rc = NULL;
            NodeList[sizeList]->isLeaf = 1;
            sizeList++;
        }
    }
}

void Encoding::sortNodeList(){

    HuffNode * aux = new HuffNode;
    for(int i=0; i < sizeList-1 ; i++ ){
        for(int j=0; j < sizeList-1; j++){
            if(NodeList[j]->freq >= NodeList[j+1]->freq && NodeList[j]->freq != 0){
                aux = NodeList[j+1];
                NodeList[j+1] = NodeList[j];
                NodeList[j] = aux;
                aux = NULL;
            }
        }
    }/*
    for(int i=0; i<sizeList; i++){
        qDebug() << (int) NodeList[i]->contain << NodeList[i]->freq << i;
    }*/
}

void Encoding::buildHuffTree(){

    for(int i=0; i<sizeList-1; i++){

        qDebug() << "Iteragindo.";

        HuffNode * head = new HuffNode;
        HuffNode * aux = new HuffNode;

        head->freq = NodeList[i]->freq + NodeList[i+1]->freq;
        head->isLeaf = 0;
        head->lc = aux->lc = NodeList[i];
        head->rc = aux->rc = NodeList[i+1];

        NodeList[i+1] = head;
        NodeList[i]->freq = 0;

        sortNodeList();

//        for(int j=0; j<sizeList; j++){
//            qDebug() << "List" << (int)NodeList[j]->contain << NodeList[j]->freq;
//        }

    }
}
void Encoding::writeHuffTree(){

}
