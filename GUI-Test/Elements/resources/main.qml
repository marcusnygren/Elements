import QtQuick 2.0
import OpenGLUnderQML 1.0
import QtQuick.Controls 1.1
import QtQml 2.0
import QtQuick.Window 2.0

Item {
    id: item1

    width: Screen.width
    height: Screen.height

    Squircle {
        SequentialAnimation on t {
            NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
            NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
            loops: Animation.Infinite
            running: true
        }
    }

    Rectangle {
        opacity: 0.5
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 1
        border.color: "white"
        anchors.fill: label
        anchors.margins: -10
    }

    Text {
        id: label
        color: Qt.rgba(0, 0, 0, 0.5)
        wrapMode: Text.WordWrap
        text: "The background here is a squircle rendered with raw OpenGL using the 'beforeRender()' signal in QQuickWindow. This text label and its border is rendered using QML"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }
    MouseArea{
        id: dragMouseArea
        width: 250
        height: 500
        anchors {right : item1.right; top: item1.top}
        x: container.x
        y: container.y
        drag.target: container
        onReleased: parent = container.Drag.target !== null ? container.Drag.target : item1

        Rectangle {
            id: container
            radius: 10
            border.width: 1
            border.color: "white"
            width: 250
            height: 500
            //anchors {right : item1.right; top: item1.top}
            anchors.margins: 10
            color: Qt.rgba(1, 1, 1, 0.5)

            Drag.active: dragMouseArea.drag.active

            Column{
                id: guiComposition
                spacing: 10
                height: parent.height
                width: parent.width*(19/20)
                anchors {right: container.right; top: container.top; topMargin: 15}
                TopGui{}
                BottomGui{}
            }
        }
    }
}
