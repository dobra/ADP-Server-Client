import QtQuick 2.0


Rectangle
{
    property string song_Name_mp3 : "aaaa";
    property string song_Full_Path;


    id: rectangle_id
    anchors.left: parent.left
    anchors.right: parent.right


    width: 250
    height: 30
    color :"blue"

    Text {
        anchors.horizontalCenter:  parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        id: song_name
        text: qsTr(song_Name_mp3);
    }
}

