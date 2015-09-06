#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QStringList>
#include "SongParser/songs.h"
#include <QTcpSocket>
#include <QCoreApplication>

class QTcpServer;
class QNetworkSession;



enum class CommandTypes
  {
     SendSong,
     SendDuration
  };

enum class ConnectionStatus
  {
     NoConnection,
     ConnectedToAClient
  };


class Server : public QObject
{
    Q_OBJECT

public:
    void report(const QString& msg);
    static Server *GetInstange();
    ConnectionStatus connection_status = (ConnectionStatus)0;
private slots:
    void sessionOpened();
    void GetNewConnection();
    void SendInformation(CommandTypes command, QString data);
    void startRead();

private:
    songs *song= songs::getInstange();                 //

    /* Making a singleton, we only need 1 object */
    Server(QObject *parent = 0);
    ~Server(){}
    Server(Server const&){}             // copy constructor is private
    Server& operator=(Server const&){}  // assignment operator is private
    static Server *my_Singleton;
    /* Ending a singleton, we only need 1 object */

    QTcpServer *tcpServer;
    QTcpSocket* tcpclient;
    QStringList fortunes;
    QNetworkSession *networkSession;
};

#endif // SERVER_H
