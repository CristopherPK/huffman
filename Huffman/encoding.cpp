#include "encoding.h"

void Encoding::calcFreqChar(QFile * src, int freqList[256]){

    char ch;

    //Inicializer array of char frequency
    for(int i=0; i<=255; i++){
        freqList[i] = 0;
    }

    //Counting char frequency
    while(!src->atEnd()){
        src->getChar(&ch);

        //Counting char frequency
        freqList[(unsigned char)ch]++;
    }

//    for(int i=0; i<=255; i++){
//        if(freqList[i]!=0)
//            qDebug() << i << freqList[i];
//    }

}

void Encoding::buildNodeList(HuffNode * NodeList[255], int freqList[255]){

    sizeList = 0;

    //Filling list.
    for(int i=0; i<=255; i++){
        //
        if(freqList[i] > 0){
            NodeList[sizeList] = new HuffNode;
            NodeList[sizeList]->contain = (unsigned char) i;
            NodeList[sizeList]->freq = freqList[i];
            NodeList[sizeList]->lc = NULL;
            NodeList[sizeList]->rc = NULL;
            NodeList[sizeList]->isLeaf = 1;
            sizeList++;
        }
    }

    sizeList--;

//   for(int i=0; i<sizeList; i++){
//        qDebug() << (int) NodeList[i]->contain << NodeList[i]->freq;
//   }

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
            if(NodeList[j]->freq >= NodeList[j+1]->freq){
                aux = NodeList[j+1];
                NodeList[j+1] = NodeList[j];
                NodeList[j] = aux;
                aux = NULL;                            
            }
        }
    }

//   for(int i=0; i<sizeList; i++){
//        qDebug() << i <<(int) NodeList[i]->contain << NodeList[i]->freq;
//   }

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

        /*for(int j=0; j<sizeList; j++){
            qDebug() << "List" << (int)NodeList[j]->contain << NodeList[j]->freq;
        }*/

    }
}


void Encoding::writeHuffTree(QFile * out, HuffNode * TreeRoot,HuffNode *CodeList[255]){

    QString TreeCode;
    QTextStream outdata(&TreeCode);

    //Bit Array for char huffman binary code
    int bits = 0;
    QBitArray code(1);

    HuffNode * curr = new HuffNode;
    HuffNode * aux = new HuffNode;

    //Current receiving huffman tree
    curr = TreeRoot;

    /* QUANDO .TXT NÃO PASSA DAQUI. */
    curr->prev = new HuffNode;

    while(curr->prev != NULL) {

        //Bit position        
        int pos;

        //Verifying if node is leaf
        if(curr->isLeaf==0){

            //Verifying left children
            if(curr->lc!=NULL){

                //qDebug() << "(";
                bits++;

                if(bits>1){                    
                    code.resize(bits);
                }

                outdata << '(';
                aux = curr;
                curr = curr->lc;
                curr->prev = aux;

            //Verifying right children
            } else if(curr->rc!=NULL) {                

                bits++;
                pos = bits - 1;
                code.resize(bits);
                code.setBit(pos,1);

                aux = curr;
                curr = curr->rc;
                curr->prev = aux;

            //Have no children, move back
            } else {

                //qDebug() << ")";
                outdata << ')';
                curr = curr->prev;
                if(curr->lc == NULL){
                    curr->rc = NULL;
                } else {
                    curr->lc = NULL;
                }

                //Removing the last bit
                bits--;
                code.resize(bits);

            }
        }

        else {

            //qDebug() << curr->contain << "BinCode:" << code;

            //Saving char huffman code
            CodeList[curr->contain]->code = code;

            if(curr->contain == '(' || curr->contain == ')'){
                outdata << '0';
            }

            //Removing the last bit
            bits--;
            code.resize(bits);

            //Saving char at huffman tree
            outdata << (char) curr->contain;
            curr = curr->prev;

            //Unmounting huffman tree
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

    QDataStream outcode(out);
    outcode << src->fileName();
    char ch;

    while(!src->atEnd()){

        src->getChar(&ch);

        outcode << CodeList[(unsigned char) ch]->code;
    }
}

void Encoding::encodeFile(QString inFileName, QString outFileName){

    //Declaring initial vars
    QFile inFile(inFileName);
    if(!inFile.open(QIODevice::ReadOnly)){
        qDebug() << "failed.";
    }

    int freqList[256];
    HuffNode * NodeList[256];

    //Calculating Frequency
    calcFreqChar(&inFile,freqList);
    //qDebug("Frequency ready.");
    inFile.close();

    //Building List
    buildNodeList(NodeList, freqList);
    //qDebug("Node List ready.");

    //Saving a copy of list
    HuffNode * ListCopy[255];
    buildCopyList(NodeList, ListCopy);
    //qDebug("Copy ready");

    //Sorting List
    sortNodeList(NodeList);
    //qDebug("Node List sorted ready.");

    //Building Huffman Tree and chars binary code
    buildHuffTree(NodeList);
    //qDebug("Huffman Tree ready.");

    //Creating output file
    QFile out(outFileName);
    if(!out.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        qDebug("failed");
    }

    //Building huffman binary code.
    writeHuffTree(&out, NodeList[255], ListCopy);
    //qDebug("Huffman Tree write.");

    if(!inFile.open(QIODevice::ReadOnly)){
        qDebug() << "failed.";
    }

    //Building file coding.
    writeHuffCode(&inFile, &out, ListCopy);
    //qDebug("Huffman code ready.");
    inFile.close();
    qDebug("Compression finished.");

}
