#include <QCoreApplication>
#include <QDataStream>

#include "encoding.h"
#include "decoding.h"

void showHelpMe(){
}

int main(int argc, char *argv[])
{         
    Encoding * Encode = new Encoding;

    Encode->encodeFile("in.jpg","out.huff");
    return 0;

    QString a;
    a = argv[1];
    QString b;
    b = argv[2];
    QString c;
    c = argv[3];
    QString d;
    d = argv[4];

    if(argc <= 4){

        //Compression Standart.
        if(a == "-c"){

//            Encoding * Encode = new Encoding;

//            Encode->encodeFile("in.txt","out.huff");

            //Compression with different path of output file.
            if(c == "-o"){

                Encode->encodeFile(b,d);
            }
        }
        //Descompression with different path of output file.
        else {
            qDebug() << "I'm sorry, not working out";

        }
    }    else {
        qDebug() << "Invalid command.";
        showHelpMe();
    }
}
