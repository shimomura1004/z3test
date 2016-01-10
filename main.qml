import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    visible: true
    width: 100
    height: 100

    MouseArea {
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onClicked: console.log("clicked!")

            Rectangle {
                x: 50
                y: 0
                width: 50
                height: 100
                color: "blue"
                MouseArea { anchors.fill: parent }
            }
        }

        Rectangle {
            x: 0
            y: 0
            width: 30
            height: 30
            color: "red"
            MouseArea { anchors.fill: parent }
        }

        Rectangle {
            x: 0
            y: 50
            width: 150
            height: 100
            color: "green"
            MouseArea { anchors.fill: parent }
        }
    }

//    Rectangle {
//        width: 50
//        height: 100
//        x: 50
//        y: 0

//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//                console.log("native:" + mouseX + "," + mouseY);

//                // local -> global(null)
//                var point1 = mapToItem(null, mouseX, mouseY);
//                console.log("mapToItem:" + point1);

//                var point2 = mapFromItem(null, mouseX, mouseY);
//                console.log("mapFromItem:" + point2);

//                // global(null) -> local
//                var point3 = mapFromItem(null, point1.x, point1.y);
//                console.log("mapFromItem:" + point3);
//            }
//        }
//    }

//    Text {
//        text: qsTr("Hello World")
//        anchors.centerIn: parent
//    }
}
