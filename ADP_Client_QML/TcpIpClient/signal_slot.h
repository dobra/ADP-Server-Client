#ifndef SIGNAL_SLOT_H
#define SIGNAL_SLOT_H
#include <QObject>
#include <QDebug>
#include <QTcpSocket>

class QTcpSocket;
class QNetworkSession;

class TCP_Song : public QObject
{
    Q_OBJECT
public slots:
    void cppSlot(const QString &msg);
public:
    TCP_Song(void);
    void requestNewConnection();
    void send_Signal(const QString value);

signals:
    void valueSignal(QVariant value1);

private:
    QString GetSongName(const QString SongFullName);
    void SendSong(const QString SongName);

    /*Needed fot the Client-Server session*/
private slots:
    void sessionOpened();
    void readSongs();
private:
    QString hostString="192.168.100.8";
    QString portString="57104";
    QTcpSocket *tcpSocket;
    QString currentFortune;
    quint16 blockSize;
    QStringList Songs_List;
    QNetworkSession *networkSession;
};

#endif
