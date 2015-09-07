import QtQuick 2.2
import QtQuick.Controls 1.1
import "./"

ApplicationWindow
{
    objectName: "Dobra_List"
    id: main_window
    visible: true
    width: 720
    height: 1280
    title: qsTr("ADP_Player")

    property var songcounts: 0;
    property var player_status:"NotPlaying";
    property real progress:0;


    function qmlSlot(mesaj)
    {
        main_window.songcounts += 1;
        renumita_lista.append(
                    {"name":mesaj, "number":main_window.songcounts
                           })
    }

    function qmlSongDuration(mesaj)
    {
        progress= mesaj/100;
        if(progress == 1)
            timer_item.running=false;
    }

    signal qmlSignal(string msg)


    function timerTriggered()
    {
        qmlSignal("1RefSongDuration");
    }

    Rectangle{
        width: 200; height:200
        anchors.right: parent.right
        color:"red";
        MouseArea{
            anchors.fill: parent;
            onClicked:
            {
                console.log(container.progress);
                if(player_status == "Playing")
                {
                    player_status="Paused";
                    qmlSignal("0Stop!");
                    timer_item.running=false;
                }
                else
                    if(player_status == "Paused")
                    {
                        player_status="Playing";
                        qmlSignal("2Resume!");
                        timer_item.running=true;
                    }
            }
        }
    }

    Rectangle {
        id: progressHorizontal1
        width: 800
        height: 30
        anchors.bottom: parent.bottom
        ProgressBarDC
        {
            anchors.fill: parent
            id:container
            progress: main_window.progress
        }
    }



    ListBox
    {
        id:renumita_lista
    }

    ListView {
        width: 180; height: 800
        id: listview1
        Component {
            id: contactsDelegate
            Rectangle {
                id: wrapper
                width: 180
                height: contactInfo.height
                color: ListView.isCurrentItem ? "black" : "red"
                Text {
                    id: contactInfo
                    text: /*number + ": " +*/ name
                    color: wrapper.ListView.isCurrentItem ? "red" : "black"
                }
                MouseArea{
                    anchors.fill: parent

                    onClicked:
                    {
                        listview1.currentIndex = index;
                        main_window.qmlSignal(name);
                        timer_item.running=true;
                        player_status="Playing";
                    }

                }
            }
        }

        model: renumita_lista
        delegate: contactsDelegate
        focus: true

        add: Transition {
               NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 500 }
               NumberAnimation { property: "scale"; easing.type: Easing.OutBounce; from: 0; to: 1.0; duration: 750 }
           }

           addDisplaced: Transition {
               NumberAnimation { properties: "y"; duration: 600; easing.type: Easing.InBack }
           }

           remove: Transition {
               NumberAnimation { property: "scale"; from: 1.0; to: 0; duration: 200 }
               NumberAnimation { property: "opacity"; from: 1.0; to: 0; duration: 200 }
           }

           removeDisplaced: Transition {
               NumberAnimation { properties: "x,y"; duration: 500; easing.type: Easing.OutBack }
           }

    }

    Item {


        Timer {
             id:timer_item
            interval: 1000; running: false; repeat: true
            onTriggered: main_window.timerTriggered();
        }
    }


}
