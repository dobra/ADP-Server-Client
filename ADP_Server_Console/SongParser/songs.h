#ifndef SONGS_H
#define SONGS_H

#include <QObject>
#include <QMediaPlayer>
#include <QCoreApplication>
#include <QDir>

class songs : public QObject
{
    Q_OBJECT
public:
    static songs *getInstange();
    void report(const QString& msg);
    void Add_To_Song_List (QString song);
    bool Verify_Song_Exists(QString song);
    qint8 Play_Song(QString path);
    void Change_Volume(int Volume);
    void Refresh_Songs(QDir Songs_folders);
    qint64 Get_Duration();
    QString Returneaza_lista(void);
private:
    QList<QString> private_Songs_List;
    QMediaPlayer *ADP_player;

    /* Making a singleton, we only need 1 object */
    songs(QObject *parent = 0);
    ~songs(){}
    songs(songs const&){}             // copy constructor is private
    songs& operator=(songs const&){}  // assignment operator is private
     static songs* my_Singleton;
     /* Ending the singleton, we only need 1 object */
signals:

public slots:
private:
   QString SongsPath="/home/dobra/Music";

};

#endif // SONGS_H
