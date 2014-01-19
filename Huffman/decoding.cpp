#include "decoding.h"

int Decoding::convertBinToDec(QByteArray entry){

    //qDebug() << entry;

    int size = 0;

    for(int i=0;i<entry.size(); i++){
        //qDebug() << entry[i];
        int bits = entry.size() - 1;

        bits -= i;

        if(entry[i]=='1'){
            size += pow(2,(double) bits);
        }
    }

    return size;

}

QByteArray Decoding::convertDecToBin(int entry){

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

void Decoding::calcSizeTree(QFile *src){

    QByteArray bytes;
    int byteCount = 0;

    char ch;

    while(byteCount < 2){
        byteCount++;
        src->getChar(&ch);
        bytes += convertDecToBin((unsigned char)ch);
    }

    QByteArray trashSize;
    QByteArray treeSize;

    for(int i=0;i<bytes.size(); i++){
        if(i<3){
            trashSize += bytes[i];
        } else {
            treeSize += bytes[i];
        }
    }

    TrashSize = convertBinToDec(trashSize);
    TreeSize = convertBinToDec(treeSize);

}

QByteArray Decoding::getFileName(QFile *src){

    QByteArray FileName;

    char ch;

    while(!src->atEnd()){

        src->getChar(&ch);

        if(ch=='('){
            break;
        }

        FileName += ch;
    }

    return FileName;
}

void Decoding::buildHuffTree(QFile *src, HuffNode *TreeRoot){

    src->seek(src->pos()-1);

    TreeRoot->lc = TreeRoot->rc = TreeRoot->prev = 0;

    char ch;
    bool warn = false;
    int count = 0;

    while(count < TreeSize){

        src->getChar(&ch);
        qDebug() << ch;

        if(ch=='('){
            if(warn == true){

                TreeRoot->contain = ch;
                TreeRoot->isLeaf = 1;
                TreeRoot = TreeRoot->prev;
            }
            else if(TreeRoot->lc==NULL){
                qDebug() << "Left";
                TreeRoot->lc = new HuffNode;
                TreeRoot->lc->prev = TreeRoot;
                TreeRoot = TreeRoot->lc;
                TreeRoot->lc = TreeRoot->rc = 0;
            } else if(TreeRoot->rc==NULL){
                qDebug() << "Right";
                TreeRoot->rc = new HuffNode;
                TreeRoot->rc->prev = TreeRoot;
                TreeRoot = TreeRoot->rc;
                TreeRoot->lc = TreeRoot->rc = 0;
            }
        }

        else if(ch==')'){
            if(warn == true){
                warn == false;
                TreeRoot->contain = ch;
                TreeRoot->isLeaf = 1;
                TreeRoot = TreeRoot->prev;
            } else {
                TreeRoot = TreeRoot->prev;
            }
        }

        else {

            TreeRoot = TreeRoot->prev;
            if(ch == '0'){
                if(warn == true){
                    TreeRoot->contain = ch;
                    TreeRoot->isLeaf = 1;
                    TreeRoot = TreeRoot->prev;
                }

                warn = true;
            }

            else if(TreeRoot->lc==NULL) {
                qDebug() << "Left";
                TreeRoot = TreeRoot->lc;
                TreeRoot->contain = ch;
                TreeRoot->isLeaf = 1;
                TreeRoot = TreeRoot->prev;
            }

            else {
                qDebug() << "Right";
                TreeRoot->rc = new HuffNode;
                TreeRoot->rc->prev = TreeRoot;
                TreeRoot = TreeRoot->rc;
                TreeRoot->contain = ch;
                TreeRoot->isLeaf = 1;
                TreeRoot = TreeRoot->prev;
            }

        }

        count++;

    }


}

void Decoding::buildHuffCode(QFile * src){
    char ch;

    src->seek(src->pos()-1);

    while(!src->atEnd()){
        src->getChar(&ch);
        HuffCode += convertDecToBin((unsigned char) ch);
    }

    int size = HuffCode.size() - TrashSize;

    HuffCode.resize(size);

    qDebug() << HuffCode;
}

void Decoding::writeDecodeFile(QFile *out, HuffNode *TreeRoot){

    HuffNode * curr = new HuffNode;
    curr = TreeRoot;

    for(int i=0; i<HuffCode.size(); i++){
        if(HuffCode[i]=='0'){
            curr = curr->lc;
        } else {
            curr = curr->rc;
        }

        if(curr->isLeaf==1){
            char * ref;
            ref = (char *)curr->contain;
            out->write(ref);
            curr = TreeRoot;
        }
    }
}

void Decoding::decodeFile(QString inFileName, QString outPath)
{
    //Opening source file
    QFile inFile(inFileName);
    if(!inFile.open(QIODevice::ReadOnly)){
        qDebug() << "failed.";
    }

    calcSizeTree(&inFile);

    qDebug() << "Tree Size: " <<TreeSize;
    qDebug() << "Trash Size: " <<TrashSize;

    QString outFileName;
    outFileName = getFileName(&inFile);

    qDebug() << outFileName;

    HuffNode * HuffTree = new HuffNode;

    buildHuffTree(&inFile,HuffTree);

    buildHuffCode(&inFile);

    QFile out(outPath + outFileName);
    if(!out.open(QIODevice::WriteOnly)){
        qDebug() << "File can not be descompressed. Try again";
    }

    writeDecodeFile(&out, HuffTree);

}
