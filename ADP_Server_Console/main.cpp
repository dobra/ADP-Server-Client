#include <QCoreApplication>

#include <stdlib.h>

#include "TcpIpServer/server.h"
#include "QThread"
#include "SongParser/songs.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /* Getting the instances of the Singletons         */
    Server *server = Server::GetInstange();             //
    /*Finished getting the instances of the Singletons */


    return a.exec();
}
