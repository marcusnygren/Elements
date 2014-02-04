import QtQuick 2.0
import OpenGLUnderQML 1.0
import QtQuick.Controls 1.1
import QtQml 2.0

Item {
    id: item1

    width: 800
    height: 600

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

    Rectangle {
        id: container
        radius: 10
        border.width: 1
        border.color: "white"
        width: 250
        height: 500
        anchors {right : item1.right; top: item1.top}
        anchors.margins: 10
        color: Qt.rgba(1, 1, 1, 0.5)

        TopGui{
            
        }

        BottomGui{

        }
    }
}
