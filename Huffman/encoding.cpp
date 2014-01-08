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
            if(NodeList[j]->freq >= NodeList[j+1]->freq){
                aux = NodeList[j+1];
                NodeList[j+1] = NodeList[j];
                NodeList[j] = aux;
                aux = NULL;
            }
        }
    }
    for(int i=0; i<sizeList; i++){
        qDebug() << (int) NodeList[i]->contain << NodeList[i]->freq << i;
    }
}

void Encoding::buildHuffTree(){

    head = new HuffNode;

    head->contain = 0;
    head->freq = 0;
    head->isLeaf = 0;

    for(int i=0; i<sizeList-1; i++){

        if(head->freq == 0){
            head->freq = NodeList[i]->freq + NodeList[i+1]->freq;
            head->lc = NodeList[i];
            head->rc = NodeList[i+1];

        } else {

            if(NodeList[i+1]->freq < NodeList[i+1]->freq + head->freq){
                head->rc = head;
                head->freq = NodeList[i+1]->freq + head->freq;
                head->lc = NodeList[i+1];
            }

            if(NodeList[i+1]->freq >= NodeList[i+1]->freq + head->freq) {
                head->lc = head;
                head->freq = NodeList[i+1]->freq + head->freq;
                head->rc = NodeList[i+1];

            }
        }
    }
}

void Encoding::writeHuffTree(){

}
