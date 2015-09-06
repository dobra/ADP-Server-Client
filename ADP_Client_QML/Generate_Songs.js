
var object;
var component1;

function generate_my_Songs()
{
    console.log("Is aici coae!");

    component1 = Qt.createComponent("Song_Rectangle.qml");
    if(component1.status === Component.Ready)
    {
        object = component1.createObject(rectangle_id,{"x":100, "y": 100});
        if(object === null)
        {
            console.log("Eroare Coae!");
        }
    }



}
