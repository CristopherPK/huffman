#include <QCoreApplication>

int main(int argc, char *argv[])
{
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

        }
        //Compression with different path of output file.
        else if(c == "-o"){

        }

        //Descompression with different path of output file.
        else if(b == "-d"){

        }
        else {
            qDebug() << "Invalid command.";
            showHelpMe();
        }

    }    else {
        qDebug() << "Invalid command. There are excess of parameters.";
        showHelpMe();
    }
}
