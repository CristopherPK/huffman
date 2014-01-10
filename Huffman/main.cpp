#include <QCoreApplication>
#include <QDataStream>

#include "encoding.h"

void showHelpMe(){

}

int main(int argc, char *argv[])
{         
    Encoding * Encode = new Encoding;

    Encode->encodeFile("in.jpg","out.huff");

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
            //Compression with different path of output file.
            if(c == "-o"){

            }
        }
        //Descompression with different path of output file.
        else {

        }
    }    else {
        qDebug() << "Invalid command. There are excess of parameters.";
        showHelpMe();
    }
}
