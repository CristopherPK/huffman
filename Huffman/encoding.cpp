#include "encoding.h"

void Encoding::calcFreqChar(QString entry){

    QFile file(entry);

    //Inicializer array of char frequency
    for(int i=0; i<=256; i++){
        freqList[i] = 0;
    }

    //Opening file
    if(!file.open(QIODevice::ReadOnly))
        qDebug() << "failed.";

    //Counting char frequency
    while(!file.atEnd()){
        file.getChar(&ch);
        freqList[(int) ch]++;

    }
}

void Encoding::buildNodeList(){
    curr = new HuffNode;
    int numChar = 0;
    for(int i=0; i<=256; i++){
        if(freqList[i] != 0){
            curr->NodeList[numChar] = new HuffNode;
            curr->NodeList[numChar]->contain = (char) i;
            curr->NodeList[numChar]->freq = freqList[i];
            curr->NodeList[numChar]->lc = NULL;
            curr->NodeList[numChar]->rc = NULL;

            numChar++;
        }
        curr->sizeList = numChar;
    }
}

void Encoding::sortNodeList(){
    HuffNode * aux = new HuffNode;

    for(int i=0; i < curr->sizeList ; i++ ){
        for(int j=curr->sizeList-1; j>0; j--){
            if(curr->NodeList[j-1]->freq >= curr->NodeList[j]->freq){
                aux = curr->NodeList[j-1];
                curr->NodeList[j-1] = curr->NodeList[j];
                curr->NodeList[j] = aux;
                aux = NULL;
            }
        }
    }

    for(int i=0; i<curr->sizeList; i++){
        qDebug() << curr->NodeList[i]->contain << curr->NodeList[i]->freq;
    }
}
