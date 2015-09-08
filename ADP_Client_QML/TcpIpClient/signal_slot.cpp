#include "TcpIpClient/signal_slot.h"
#include <QtNetwork>

TCP_Song::TCP_Song(void)
{
    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty())
    {
        qDebug() << name << endl;
    }
     // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (!ipAddressesList.at(i).isLoopback())
                  qDebug() << ipAddressesList.at(i).toString() << endl;
    }

    tcpSocket = new QTcpSocket(this);



    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readSongs()));
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired)
    {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered)
        {
            config = manager.defaultConfiguration();
        }

         networkSession = new QNetworkSession(config, this);
         connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

         networkSession->open();
    }

}

void TCP_Song::readSongs()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    if (blockSize == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }


    qDebug()<<"Am ajuns aici cu:"<<tcpSocket->bytesAvailable()<<endl;
    QString nextFortune;
    in >> nextFortune;
    currentFortune = nextFortune;


    switch(currentFortune.at(0).toLatin1())
    {
    case '0':
    {
        Songs_List = currentFortune.split(",");
        foreach (QString song, Songs_List)
        {
          this->send_Song_Signal(song);
        }

        qDebug()<<currentFortune<<endl;
        break;
    }
    case '1':
    {
        currentFortune=currentFortune.mid(3,3);
        qDebug()<<currentFortune<<endl;
        emit SongDurationSignal(currentFortune);
        break;
    }
    }
    blockSize = 0;


}

void TCP_Song::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    qDebug() << "Something Started!" << endl;
}

void TCP_Song::requestNewConnection()
{

    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(hostString,
                             portString.toInt());
}


void TCP_Song::send_Song_Signal(const QString value1)
{
    qDebug() << "I've emited:" << value1 <<endl;
    emit valueSignal(GetSongName(value1));
}

void TCP_Song::cppSlot(const QString &msg)
     {
         qDebug() << "Called the C++ slot with message:" << msg;
         this->SendSong(msg);
     }
QString TCP_Song::GetSongName(const QString SongFullName)
{
    QStringList PathTree;

    PathTree = SongFullName.split("/");

    return PathTree.last();
}

void TCP_Song::SendSong(const QString SongName)
{
    QByteArray ba ;


    switch (SongName.at(0).toLatin1()) {
    case '1':
    {   //Sending duration
        ba = SongName.toLatin1();
        break;
    }
    case '0':
    {   //Sending Stop
        ba = SongName.toLatin1();
        break;
    }
    case '2':
    {   //Sending Resume
        ba = SongName.toLatin1();
        break;
    }
    case '3':
    {   //Sending volume
        ba = SongName.toLatin1();
        break;
    }
    default:
    {
        foreach (QString song, Songs_List)
        {
            if(song.contains(SongName))
          ba = song.toLatin1();
        }
        break;
    }
    }



    tcpSocket->write(ba.data());
}
