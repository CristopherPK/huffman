#include "encoding.h"
#include <math.h>

void Encoding::calcFreqChar(QFile * src, int freqList[255], char ch){

    //Inicializer array of char frequency
    for(int i=0; i<=255; i++){
        freqList[i] = 0;
    }

    //Counting char frequency
    while(!src->atEnd()){
        src->getChar(&ch);

        //ASCII Extended
        int asc = 0;
        asc = (int) ch;
        if(asc<0){
            asc += 256;
        }

        //Counting char frequency
        freqList[asc]++;
    }

//    for(int i=0; i<=255; i++){
//        qDebug() << i << freqList[i];
//    }

}

void Encoding::buildNodeList(HuffNode * NodeList[255], int freqList[255]){

    sizeList = 0;

    //Filling list.
    for(int i=0; i<=255; i++){
        //
        if(freqList[i] > 0){
            NodeList[sizeList] = new HuffNode;
            NodeList[sizeList]->contain = (char) i;
            NodeList[sizeList]->freq = freqList[i];
            NodeList[sizeList]->lc = NULL;
            NodeList[sizeList]->rc = NULL;
            NodeList[sizeList]->isLeaf = 1;
            sizeList++;
        }
    }

    sizeList--;

//    for(int i=0; i<sizeList; i++){
//        qDebug() << (int) NodeList[i]->contain << NodeList[i]->freq;
//    }

}

void Encoding::buildCopyList(HuffNode *NodeList[255], HuffNode *ListCopy[255]){
    for(int i=0; i<=255; i++){
        ListCopy[i] = NodeList[i];
    }
}


void Encoding::sortNodeList(HuffNode * NodeList[255]){

    HuffNode * aux = new HuffNode;
    for(int i=0; i < sizeList; i++){
        for(int j=0; j < sizeList; j++){
            if(NodeList[j]->freq >= NodeList[j+1]->freq && NodeList[j]->freq != 0){
                aux = NodeList[j+1];
                NodeList[j+1] = NodeList[j];
                NodeList[j] = aux;
                aux = NULL;                            
            }
        }
    }

//    for(int i=0; i<sizeList; i++){
//        qDebug() << (int) NodeList[i]->contain << NodeList[i]->freq << i;
//    }
}

void Encoding::buildHuffTree(HuffNode *NodeList[255]){

    for(int i=0; i<sizeList; i++){

        HuffNode * head = new HuffNode;
        HuffNode * aux = new HuffNode;

        head->freq = NodeList[i]->freq + NodeList[i+1]->freq;
        head->isLeaf = 0;
        head->lc = aux->lc = NodeList[i];
        head->rc = aux->rc = NodeList[i+1];

        NodeList[i+1] = head;
        NodeList[i]->freq = 0;

        sortNodeList(NodeList);

//        for(int j=0; j<sizeList; j++){
//            qDebug() << "List" << (int)NodeList[j]->contain << NodeList[j]->freq;
//        }

    }
}


void Encoding::writeHuffTree(QFile * out, HuffNode * TreeRoot,HuffNode *CodeList[255]){

    QString TreeCode;
    QString code;
    QTextStream outdata(&TreeCode);

    HuffNode * curr = new HuffNode;
    HuffNode * aux = new HuffNode;

    curr = TreeRoot;

    curr->prev = new HuffNode;

    while(curr->prev != NULL) {
        if(curr->isLeaf==0){
            if(curr->lc!=NULL){

                //qDebug() << "(";
                code += '0';
                outdata << "(";
                aux = curr;
                curr = curr->lc;

                curr->prev = aux;
            } else if(curr->rc!=NULL) {
                code += '1';
                aux = curr;
                curr = curr->rc;
                curr->prev = aux;

            } else {
                //qDebug() << ")";
                outdata << ")";
                curr = curr->prev;
                if(curr->lc == NULL){
                    curr->rc = NULL;
                } else {
                    curr->lc = NULL;
                }

                int lpos;
                lpos = code.size()-1;
                code.remove(lpos, 1);
            }
        }

        else {

            //qDebug() << curr->contain << "BinCode:" << code;

            int asc = (int) curr->contain;
            if(asc < 0)
                asc += 256;

            CodeList[asc]->code = code;
            //qDebug() << CodeList[asc]->code;

            //qDebug() << curr->contain;

            if(curr->contain == '(' || curr->contain == ')'){
                outdata << '00x0';
            }

            //Removing last code;
            int lpos;
            lpos = code.size()-1;
            code.remove(lpos, 1);
            outdata << curr->contain;
            curr = curr->prev;
            if(curr->lc == NULL){
                curr->rc = NULL;
            } else {
                curr->lc = NULL;
            }
        }
    }

    //qDebug() << TreeCode;

    QTextStream outfile(out);
    outfile << TreeCode.size();
    outfile << TreeCode;

}

void Encoding::writeHuffCode(QFile *src, QFile *out, HuffNode *CodeList[255]){

    QString HuffCode;

    QTextStream outcode(&HuffCode);
    char ch;

    while(!src->atEnd()){

        src->getChar(&ch);

        //ASCII Extended
        int asc = 0;
        asc = (int) ch;
        if(asc<0){
            asc += 256;
        }

        outcode << CodeList[asc]->code;
    }

    QString hcode;

    int j=0;
    int codeasc=0;
    char code;
    for(int i=0; i<HuffCode.size(); i++){
        if(HuffCode[i]=='1'){
            codeasc += pow(2,j);
        }
        j++;
        if(j==8){
            j = 0;
            code = (char) codeasc;
            hcode += code;
        }

        if(i==HuffCode.size()-1 && j<8){
            code = (char) codeasc;
            hcode += code;
        }
    }

    qDebug() << hcode.size();
    QTextStream outfile(out);
    outfile << src->fileName();
    outfile << hcode;
}

void Encoding::encodeFile(QString inFileName, QString outFileName){

    //Declaring initial vars
    QFile inFile(inFileName);
    if(!inFile.open(QIODevice::ReadOnly)){
        qDebug() << "failed.";
    }

    int freqList[255];
    char ch = 0;
    HuffNode * NodeList[255];

    //Calculating Frequency
    calcFreqChar(&inFile,freqList,ch);
    qDebug("Frequency ready.");
    inFile.close();

    //Building List
    buildNodeList(NodeList, freqList);
    qDebug("Node List ready.");

    //Saving a copy of list
    HuffNode * ListCopy[255];
    buildCopyList(NodeList, ListCopy);
    qDebug("Copy ready");

    //Sorting List
    sortNodeList(NodeList);
    qDebug("Node List sorted ready.");

    //Building Huffman Tree and chars binary code
    buildHuffTree(NodeList);
    qDebug("Huffman Tree ready.");

    //Creating output file
    QFile out(outFileName);
    if(!out.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        qDebug("failed");
    }

    //Building huffman binary code.
    writeHuffTree(&out, NodeList[255], ListCopy);
    qDebug("Huffman Tree write.");

    if(!inFile.open(QIODevice::ReadOnly)){
        qDebug() << "failed.";
    }
    //Building file coding.
    writeHuffCode(&inFile, &out, ListCopy);
    qDebug("Huffman code ready.");
    inFile.close();

}
