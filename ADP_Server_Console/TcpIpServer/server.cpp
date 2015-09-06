#include <QtNetwork>

#include <stdlib.h>
#include "TcpIpServer/server.h"

Server *Server::my_Singleton = NULL;

Server *Server::GetInstange()
{
    if(NULL == my_Singleton)
    {
        Server::my_Singleton = new Server();
    }
        return Server::my_Singleton;
}

Server::Server(QObject *parent)
:   QObject(parent), tcpServer(0), networkSession(0)
{

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

        report("Opening network session.");
        networkSession->open();
    } else {
        sessionOpened();
    }


        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(GetNewConnection()));

}


void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
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
    }
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        report(tr("Unable to start the server: %1.") .arg(tcpServer->errorString()));
        EXIT_FAILURE;
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    report(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Fortune Client example now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));
}
void Server::GetNewConnection()
{
    connection_status = (ConnectionStatus)0; //ConnectionStatus::NoConnection

    tcpclient = tcpServer->nextPendingConnection();
    connect(tcpclient, SIGNAL(disconnected()),tcpclient, SLOT(deleteLater()));

    connect(tcpclient, SIGNAL(readyRead()), this, SLOT(startRead()));

    SendInformation((CommandTypes)0, song->Returneaza_lista()); //CommandTypes::SendSong

}
void Server::report(const QString& msg)
{
    qDebug("[%i] ADP_Server: %s", (int)QCoreApplication::applicationPid(), qPrintable(msg));
}

void Server::SendInformation(CommandTypes command, QString data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    data = QString::number((quint8)command) + "##" + data;
    out << data;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    report(data);
    tcpclient->write(block);
}

QString Server::startRead()
{
  char buffer[1024] = {0};

  tcpclient->read(buffer, sizeof(buffer));
  report((QString)buffer);
  if(song->Play_Song((QString)buffer))
  {
    QThread::sleep(1);
    QString str;
    SendInformation((CommandTypes)1,str.number(song->Get_Duration()));
  }

}
