#include "songs.h"
songs* songs::my_Singleton = NULL;


songs::songs(QObject *parent) :
    QObject(parent)
{
    ADP_player = new QMediaPlayer;
    QDir Directory_of_Songs;

    Directory_of_Songs.setPath("/home/dobra/Music");
    this->Refresh_Songs(Directory_of_Songs);

}


songs *songs::getInstange()
{
    if(my_Singleton == NULL)
    {
        my_Singleton = new songs();
    }
        return my_Singleton;
}


void songs::Refresh_Songs(QDir Directory_of_Songs)
{
    QStringList Songs_folders;
    QString dummy;

    Songs_folders = Directory_of_Songs.entryList();
    if(Songs_folders.count() != 0)
    {
        foreach(dummy,Songs_folders)
        {
            if ((dummy != ".") && (dummy != ".."))
            {
                this->Refresh_Songs(Directory_of_Songs.path() + "/" + dummy);
            }
        }
    }
    else
    {
        //verify if it's an mp3 file
        if(Directory_of_Songs.path().contains(".mp3"))
        {
            this->Add_To_Song_List(Directory_of_Songs.path());
        }
    }
}

QString songs::Returneaza_lista(void)
{
    QString string1;

    for(int i=0; i<private_Songs_List.size(); i++)
    {
    string1 += private_Songs_List[i];
    if(i<private_Songs_List.size()-1)
       string1 += "," ;
    }
    return string1;
}


void songs::Change_Volume(int Volume)
{
    //report(QString("Volume changed to: %d",&Volume));
    ADP_player->setVolume(Volume);
}

void songs::report(const QString& msg)
{
    qDebug("[%i] ADP_Server: %s", (int)QCoreApplication::applicationPid(), qPrintable(msg));
}

void songs::Play_Song(QString Song_path)
{
    report("Song: %s started playing."+Song_path);
    if(Song_path == "Stop!")
    {
        ADP_player->stop();
        //return 5;
    }
        else
    {
        ADP_player->setMedia(QUrl::fromLocalFile(Song_path));
        ADP_player->play();
    }
    report("Song playing."+Song_path);
}

void songs::Add_To_Song_List (QString song)
{
    private_Songs_List.append(song);
}

bool songs::Verify_Song_Exists(QString song)
{
    QList<QString>::iterator local_Stub_Iterator;
    bool verify_flag = false;

    for (local_Stub_Iterator=private_Songs_List.begin(); local_Stub_Iterator!=private_Songs_List.end(); local_Stub_Iterator++)
    {
        if((*local_Stub_Iterator) == song)
        {
            verify_flag = true;
            report("Melodia Exista!");
        }
    }
    return verify_flag;
}

qint64 songs::Get_Duration()
{
    return ADP_player->position();
}
