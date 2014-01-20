#include <QCoreApplication>
#include <QDataStream>

#include "encoding.h"
#include "decoding.h"

void showHelpMe(){
    qDebug("|-------|Huffman Compressor Help|-------|");
    qDebug("|---------------------------------------|");
    qDebug(" -c 'file'  // Compress file.");
    qDebug(" -c 'file' -o 'outname.huff'' // Compress file with different name.");
    qDebug("|---------------------------------------|");
    qDebug(" file.huff // Descompress file.");
    qDebug(" file.huff -d 'outpath' // Descompress file with different outpath");
    qDebug("|---------------------------------------|");
}

int main(int argc, char *argv[])
{

//    Encoding * Encode = new Encoding;
//    Encode->encodeFile("in.bmp","out.huff");
//    return 0;

//    Decoding * Decode = new Decoding;
//    Decode->decodeFile("out.huff","");
//    return 0;

    QString a;
    a = argv[1];
    QString b;
    b = argv[2];
    QString c;
    c = argv[3];
    QString d;
    d = argv[4];



    if(argc <= 6){
        if(a=="-h"){
            showHelpMe();
        }

        //Compression Standart.
        else if(a == "-c"){
            Encoding * Encode = new Encoding;
            //Compression with different path of output file.
            if(c == "-o"){
                if(d.contains(".huff")){
                    Encode->encodeFile(b,d);
                } else {
                    qDebug() << "Invalid command. Type -h to get help.";
                }
            } else {
                Encode->encodeFile(b,"out.huff");
            }

        }
        //Descompression with different path of output file.
        else if(a.contains(".huff")){
            Decoding * Decode = new Decoding;
            if(b=="-d"){
                Decode->decodeFile(a,c);
            } else {
                Decode->decodeFile(a,"");
            }
        } else {
            qDebug() << "Invalid command. Type -h to get help.";
        }
    }

    else {
        qDebug() << "Invalid command. Type -h to get help.";
    }

    return 0;
}
