#include "encoding.h"

void Encoding::initHuffList(HuffNode * NodeList[256], HuffNode * ListCopy[256],int freqList[256]){
    //Inicializer array of char frequency
    for(int i=0; i<256; i++){
        freqList[i] = 0;
        NodeList[i] = NULL;
        ListCopy[i] = NULL;
    }
}

void Encoding::calcFreqChar(QFile * src, int freqList[256]){

    char ch;

    //Counting char frequency
    while(!src->atEnd()){
        src->getChar(&ch);

        //Counting char frequency
        freqList[(unsigned char)ch]++;
    }

//    for(int i=0; i<256; i++){
//        if(freqList[i]!=0)
//            qDebug() << i << freqList[i];
//    }

}

void Encoding::buildNodeList(HuffNode * NodeList[256], HuffNode * ListCopy[256],int freqList[256]){

    sizeList = 0;

    //Filling list.
    for(int i=0; i<256; i++){

        ListCopy[i] = new HuffNode;
        ListCopy[i]->contain = (unsigned char) i;
        ListCopy[i]->freq = freqList[i];
        ListCopy[i]->lc = NULL;
        ListCopy[i]->rc = NULL;
        ListCopy[i]->isLeaf = 1;
        ListCopy[i]->visited = 0;

        if(freqList[i] > 0){
            NodeList[sizeList] = new HuffNode;
            NodeList[sizeList]->contain = (unsigned char) i;
            NodeList[sizeList]->freq = freqList[i];
            NodeList[sizeList]->lc = NULL;
            NodeList[sizeList]->rc = NULL;
            NodeList[sizeList]->isLeaf = 1;
            NodeList[sizeList]->visited = 0;
            sizeList++;
        }
    }

//   for(int i=0; i<sizeList; i++){
//        qDebug() << (int) NodeList[i]->contain << NodeList[i]->freq;
//   }

}

void Encoding::sortNodeList(HuffNode * NodeList[256]){

    HuffNode * aux = new HuffNode;
    for(int i=0; i < sizeList - 1; i++){
        for(int j=0; j < sizeList - 1; j++){
            if(NodeList[j]->visited!=true){
                if(NodeList[j]->freq > NodeList[j+1]->freq){
                    aux = NodeList[j+1];
                    NodeList[j+1] = NodeList[j];
                    NodeList[j] = aux;
                    aux = NULL;
                }
            }
        }
    }

//   for(int i=0; i<sizeList; i++){
//       qDebug() << i <<(int) NodeList[i]->contain << NodeList[i]->freq << NodeList[i]->isLeaf;
//   }

}

void Encoding::buildHuffTree(HuffNode *NodeList[256]){

    for(int i=0; i<sizeList-1; i++){

        HuffNode * head = new HuffNode;
        HuffNode * aux = new HuffNode;

        head->freq = NodeList[i]->freq + NodeList[i+1]->freq;
        head->isLeaf = 0;
        head->lc = aux->lc = NodeList[i];
        head->rc = aux->rc = NodeList[i+1];
        head->prev = 0;
        head->visited = 0;

        NodeList[i+1] = head;
        NodeList[i]->visited = true;

        sortNodeList(NodeList);
   }
}


void Encoding::writeHuffTree(HuffNode * TreeRoot,HuffNode *CodeList[256]){

    QString TreeCode;
    QTextStream outdata(&TreeCode);

    //Bit Array for char huffman binary code
    int bits = 0;
    QString code;

    HuffNode * curr = new HuffNode;
    HuffNode * aux = new HuffNode;

    //Current receiving huffman tree    
    curr = TreeRoot;

    /* QUANDO .TXT NÃO PASSA DAQUI. */
    curr->prev = new HuffNode;

    while(curr->prev != NULL) {

        //Verifying if node is leaf
        if(curr->isLeaf==0){

            //Verifying left children
            if(curr->lc!=NULL){

                //qDebug() << "(";
                bits++;
                code.append('0');
                outdata << '(';
                aux = curr;                
                curr = curr->lc;
                curr->prev = aux;

            //Verifying right children
            } else if(curr->rc!=NULL) {                

                bits++;                                
                code.append('1');
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

            //qDebug() << (char) curr->contain << "BinCode:" << code;

            //Saving char huffman code

            CodeList[(int) curr->contain]->code = code;

            //qDebug() << CodeList[(int) curr->contain]->code;

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

    //qDebug() << TreeCode.size();
    //qDebug() << TreeCode;

//    QTextStream outfile(out);
//    outfile << TreeCode;

    HuffTree = TreeCode.toLocal8Bit();

}

void Encoding::writeHuffCode(QFile *src, HuffNode *CodeList[256]){

    QString huffCode;
    QTextStream outcode(&huffCode);

    char ch;

    while(!src->atEnd()){

        src->getChar(&ch);

        outcode << CodeList[(unsigned char) ch]->code;

    }

    //qDebug() << huffCode;

    int rest = huffCode.size()%8;

    int trash = 0;

    while(rest!=0){
        huffCode.append('0');
        trash++;
        rest = huffCode.size()%8;
    }

    tsize = trash;

    unsigned char code;

    for(int i=0;i<huffCode.size(); i++){
        int bits = 7;
        if(i>7){
            bits -= i%8;
        } else {
            bits -= i;
        }
        if(huffCode[i]=='1'){
            code += pow(2,(double) bits);
        }

        if(bits==0){
            HuffCode += code;
            code = 0;
        }
    }
    //qDebug() << HuffCode;
}

QByteArray Encoding::convertBinToDec(QByteArray entry){

    //qDebug() << entry;

    QByteArray output;

    unsigned char code = 0;

    for(int i=0;i<entry.size(); i++){
        //qDebug() << entry[i];
        int bits = 7;
        if(i>7){
            bits -= i%8;
        } else {
            bits -= i;
        }

        if(entry[i]=='1'){            
            code += pow(2,(double) bits);
        }

        if(bits==0){            
            //qDebug() << code;
            output += code;
            code = 0;
        }
    }

    return output;

}

QByteArray Encoding::convertDecToBin(int entry){

    QByteArray bitSize;

    int bin[8];

    for(int i=0;i<8;i++){
        bin[i]=0;
    }

    int a=7;

    do{
        bin[a] = entry % 2;
        a--;
        if(entry==2){
            bin[a] = entry/2;
        }
        entry = entry/2;
        //qDebug() << entry;

    }while(entry>=2);

    if(entry==1){
        bin[a] = 1;
    }

    bool warn = false;

    for(int i=0;i<8;i++){
        if(bin[i] == 0 && warn == true){
            bitSize += '0';            
        }
        if(bin[i] == 1){
            bitSize += '1';
            warn = true;
        }

    }

    return bitSize;

}

void Encoding::freeHuffTree(HuffNode *NodeList[256], HuffNode *ListCopy[256]){
    for(int i=0; i<256; i++){
        NodeList[i] = NULL;
        ListCopy[i] = NULL;
        delete NodeList[i];
        delete ListCopy[i];
    }
}

void Encoding::encodeFile(QString inFileName, QString outFileName){

    //Declaring initial vars
    QFile inFile(inFileName);
    if(!inFile.open(QIODevice::ReadOnly)){
        qDebug() << "File not found.";
        return;
    }

    int freqList[256];
    HuffNode * NodeList[256];
    HuffNode * ListCopy[256];

    //Initializer arrays.
    initHuffList(NodeList, ListCopy, freqList);

    //Calculating Frequency
    calcFreqChar(&inFile,freqList);
    qDebug("Frequency ready.");
    inFile.close();

    //Building List
    buildNodeList(NodeList, ListCopy,freqList);
    qDebug("Node List ready.");

    //Sorting List
    sortNodeList(NodeList);
    qDebug("Node List sorted ready.");

    //Building Huffman Tree and chars binary code
    buildHuffTree(NodeList);
    qDebug("Huffman Tree ready.");

    HuffNode * TreeRoot = new HuffNode;
    TreeRoot = NodeList[sizeList-1];

    //Building huffman binary code.
    writeHuffTree(TreeRoot, ListCopy);
    qDebug("Huffman Tree write.");

    if(!inFile.open(QIODevice::ReadOnly)){
        qDebug() << "failed.";
    }

    //Building file coding.
    writeHuffCode(&inFile, ListCopy);
    //qDebug("Huffman code ready.");


    //Creating output file
    QFile out(outFileName);
    if(!out.open(QIODevice::WriteOnly)){
        qDebug("failed");
    }

    //Writing header of output file.

    //Writing size of trash and huffman tree
    QByteArray bitSize;
    QByteArray byteSize;

    qDebug() <<"Tree: " <<HuffTree.size();

    bitSize += convertDecToBin(HuffTree.size());

    while(bitSize.size()<13){
        bitSize.prepend('0');
    }

    qDebug() <<"Trash: " <<tsize;

    bitSize.prepend(convertDecToBin(tsize));

    while(bitSize.size()<16){
        bitSize.prepend('0');
    }

    qDebug() << "Bytes(2): " <<bitSize;

    byteSize = convertBinToDec(bitSize);

    //qDebug() << byteSize;

    out.write(byteSize);

    //qDebug() << inFile.fileName().size();

    QByteArray fileName;
    fileName = inFile.fileName().toLocal8Bit();
    out.write(fileName);

    //qDebug() << HuffTree.size();

    out.write(HuffTree);

    //qDebug() << HuffCode.size();

    out.write(HuffCode);

    int outsize = HuffCode.size() + HuffTree.size() + inFile.fileName().size() + byteSize.size();

    qDebug() << "Tamanho do arquivo esperado:" << outsize;
    qDebug() << "Tamanho real: " << out.size();

    //Closing files.
    inFile.close();
    out.close();
    qDebug("Compression finished.");

    //Free memory byffer
    freeHuffTree(NodeList, ListCopy);
    delete TreeRoot;
}
