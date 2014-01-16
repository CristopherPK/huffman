#include "decoding.h"


QString Decoding::convertDecToBin(int entry){

    QString bitSize;

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

    }while(entry>=2);

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

QString Decoding::convertBinToDec(QString entry){

    //qDebug() << entry;

    QString output;

    unsigned char code;

    for(int i=0;i<entry.size(); i++){
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
            output += code;
            code = 0;
        }
    }

    return output;

}

void Encoding::calcSizeTree(int TreeSize, int TrashSize){

    //Reading first 2 bytes an calculating trash and tree sizes
    QString header = inFile.read(2);
    bool ok;
    int decSize = header.toInt(&ok);
    header = convertDecToBin(decSize); //header is now a binary number

    QString trashSize;
    QString treeSize;
    for(int i = 0; i < 16; ++i)
    {
        if(i < 3)
            trashSize[i] = header[i];
        else
            treeSize[i] = header[i];
    }

    trashSize = convertBinToDec(trashSize);
    treeSize = convertBinToDec(treeSize);

    qDebug() << trashSize;
    qDebug() << treeSize;

    int integerTreeSize = treeSize.toInt(&ok);

}

QString Decoding::getFileName(QFile * src){

    //Read original file name
    header = inFile.read(140);

    QString originalName;

    int marker;
    for(int i = 2; i < 140; ++i){

        if(header[i] == ')'){
            marker = i;
            break;
        }
    }
    for(int i = 2; i < marker; ++i){
        originalName[i] = header[i];
    }
}

void Decoding::buildHuffTree(QFile * src, HuffNode *TreeRoot,  int treeSize, int markerIndice){

    char ch;
    bool warn;
    int countdown = treeSize;
    src->seek(markerIndice + 1); //Seek to marker at the end of file name + 1

    while(countdown > 0){

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

        --countdown;
    }

}

void Decoding::writeDecodeFile(){

}


void Decoding::decodeFile(QString inFileName, QString outPath)
{
    //Opening source file
    QFile inFile(inFileName);
    if(!inFile.open(QIODevice::ReadOnly)){
        qDebug() << "File not found.";
        return;
    }

    int TreeSize;
    int TrashSize;

    //Getting tree size and trash size.
    calcSizeTree(TreeSize, TrashSize);

    //Getting file name.
    QString outFileName;
    outFileName = getFileName(inFileName, TreeSize);

    //Re-building the tree
    HuffNode *TreeRoot = new HuffNode;

    buildHuffTree(&inFile, TreeRoot, integerTreeSize, marker);
}
