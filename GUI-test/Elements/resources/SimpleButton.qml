import QtQuick 2.0
import OpenGLUnderQML 1.0
import QtQuick.Controls 1.1

Rectangle  {
    id: container
    radius: 10

    property string text: "Button"

    signal clicked

    width: buttonLabel.width + 20; height: buttonLabel.height + 5
    border  { width: 1; color: Qt.darker(activePalette.button) }
    smooth: true

    // color the button with a gradient
    gradient: Gradient  {
        GradientStop  {
            position: 0.0
            color:  {
                if (mouseArea.pressed)
                    return activePalette.dark
                else
                    return activePalette.light
            }
        }
        GradientStop  { position: 1.0; color: activePalette.button }
    }

    MouseArea  {
        id: mouseArea
        anchors.fill: parent
        onClicked: container.clicked();
    }

    Text  {
        id: buttonLabel
        color: "#da2a2a"
        text: "KnappN"
        anchors.centerIn: container
    }
}
