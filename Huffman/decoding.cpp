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

    for(int i=0;i<8;i++){
        if(bin[i] == 0){
            bitSize += '0';
        }
        if(bin[i] == 1){
            bitSize += '1';
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

void Decoding::buildHuffTree(QFile *src){

    src->seek(src->pos()-1);

    char ch;
    bool warn = false;
    int count = 0;

    while(count < TreeSize){

        count++;
        //qDebug() << count;

        src->getChar(&ch);

        if(ch=='('){
            if(warn == true){
                //qDebug() << ch;
                warn = false;
                if(TreeRoot->lc==NULL) {
                    TreeRoot->lc = new HuffNode;
                    TreeRoot->lc->contain = ch;
                    //qDebug() << TreeRoot->lc->contain;
                    TreeRoot->lc->isLeaf = 1;
                }

                else {
                    //qDebug() << ch << "Right";
                    TreeRoot->rc = new HuffNode;
                    TreeRoot->rc->prev = new HuffNode;
                    TreeRoot->rc->prev = TreeRoot;
                    TreeRoot = TreeRoot->rc;
                    TreeRoot->contain = ch;
                    TreeRoot->isLeaf = 1;
                    TreeRoot = TreeRoot->prev;
                }

            }

            else if(TreeRoot == NULL){
                TreeRoot = new HuffNode;
                TreeRoot->isLeaf = 0;
                TreeRoot->lc = TreeRoot->rc = 0;
                TreeRoot->prev = new HuffNode;
                TreeRoot->prev = TreeRoot;
            }

            else if(TreeRoot->lc==NULL){
                //qDebug() << "Left";
                TreeRoot->lc = new HuffNode;
                TreeRoot->lc->prev = new HuffNode;
                TreeRoot->lc->prev = TreeRoot;
                TreeRoot = TreeRoot->lc;
                TreeRoot->lc = TreeRoot->rc = 0;
                TreeRoot->isLeaf = 0;
            }

            else if(TreeRoot->rc==NULL){
                //qDebug() << "Right";
                TreeRoot->rc = new HuffNode;
                TreeRoot->rc->prev = new HuffNode;
                TreeRoot->rc->prev = TreeRoot;
                TreeRoot = TreeRoot->rc;
                TreeRoot->lc = TreeRoot->rc = 0;
                TreeRoot->isLeaf = 0;
            }
        }

        else if(ch==')'){
            if(warn == true){
                //qDebug() << ch;
                warn = false;
                if(TreeRoot->lc==NULL) {
                    TreeRoot->lc = new HuffNode;
                    TreeRoot->lc->contain = ch;
                    //qDebug() << TreeRoot->lc->contain;
                    TreeRoot->lc->isLeaf = 1;
                }

                else {
                    //qDebug() << ch << "Right";
                    TreeRoot->rc = new HuffNode;
                    TreeRoot->rc->prev = new HuffNode;
                    TreeRoot->rc->prev = TreeRoot;
                    TreeRoot = TreeRoot->rc;
                    TreeRoot->contain = ch;
                    TreeRoot->isLeaf = 1;
                    TreeRoot = TreeRoot->prev;
                }
            } else {
                //qDebug() << "Back";
                if(TreeRoot->prev != NULL){
                    TreeRoot = TreeRoot->prev;
                }
            }
        }

        else {

            if(ch == '0'){
                if(warn == true){
                    //qDebug() << ch;
                    warn = false;
                    if(TreeRoot->lc==NULL) {
                        TreeRoot->lc = new HuffNode;
                        TreeRoot->lc->contain = ch;
                        //qDebug() << TreeRoot->lc->contain;
                        TreeRoot->lc->isLeaf = 1;
                    }

                    else {
                        //qDebug() << ch << "Right";
                        TreeRoot->rc = new HuffNode;
                        TreeRoot->rc->prev = new HuffNode;
                        TreeRoot->rc->prev = TreeRoot;
                        TreeRoot = TreeRoot->rc;
                        TreeRoot->contain = ch;
                        TreeRoot->isLeaf = 1;
                        TreeRoot = TreeRoot->prev;
                    }
                } else {
                    warn = true;
                }
            }

            else if(TreeRoot->lc==NULL) {
                TreeRoot->lc = new HuffNode;
                TreeRoot->lc->contain = ch;
                //qDebug() << TreeRoot->lc->contain;
                TreeRoot->lc->isLeaf = 1;
            }

            else {
                //qDebug() << ch << "Right";
                TreeRoot->rc = new HuffNode;
                TreeRoot->rc->prev = new HuffNode;
                TreeRoot->rc->prev = TreeRoot;
                TreeRoot = TreeRoot->rc;
                TreeRoot->contain = ch;
                TreeRoot->isLeaf = 1;
                TreeRoot = TreeRoot->prev;
            }

        }

    }    
}

void Decoding::buildHuffCode(QFile * src){
    char ch;

//    src->seek();

    while(!src->atEnd()){
        src->getChar(&ch);
        //qDebug() << (unsigned char) ch;
        HuffCode += convertDecToBin((unsigned char) ch);
        //qDebug() << HuffCode;
    }

    int size = HuffCode.size() - TrashSize;

    HuffCode.resize(size);

    //qDebug() << HuffCode;
}

void Decoding::writeDecodeFile(QFile *out){

    HuffNode * curr = new HuffNode;
    QByteArray outCode;
    curr = TreeRoot;

    //qDebug() << HuffCode.size();

    for(int i=0; i<HuffCode.size(); i++){
        //qDebug() << i;
        if(HuffCode[i]=='0'){
            curr = curr->lc;
        } else {
            curr = curr->rc;
        }

        if(curr->isLeaf==1){
            //qDebug() << curr->contain;
            //char ch = curr->contain;
            //out->putChar((char)curr->contain);
            outCode += ((char )curr->contain);
            curr = TreeRoot;
        }
    }

    out->write(outCode);
}

void Decoding::decodeFile(QString inFileName, QString outPath)
{
    //Opening source file
    QFile inFile(inFileName);
    if(!inFile.open(QIODevice::ReadOnly)){
        qDebug() << "Failed to try open file.";
    }

    calcSizeTree(&inFile);

    //qDebug() << "Tree Size: " <<TreeSize;
    //qDebug() << "Trash Size: " <<TrashSize;

    QString outFileName;
    outFileName = getFileName(&inFile);

    //qDebug() << outFileName;

    buildHuffTree(&inFile);

    buildHuffCode(&inFile);

    QFile out(outPath + outFileName);
    if(!out.open(QIODevice::WriteOnly)){
        qDebug() << "Failed to try descompress file. Try again";
    }    

    writeDecodeFile(&out);
    qDebug() << "Descompression finished.";

    inFile.close();
    out.close();

}
