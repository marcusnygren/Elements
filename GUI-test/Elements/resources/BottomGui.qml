import QtQuick 2.0
import OpenGLUnderQML 1.0
import QtQuick.Controls 1.1

Row{
    id: varRow
    width: 235
    height: 375
    anchors.leftMargin: 10
    anchors.topMargin: 18
    anchors.top: presetRow.bottom
    spacing: 10

    Column{
        id: column1
        spacing: 10
        width: 30

        Text{
            id: fi
            text: "Fi:"
        }
        Text{
            id: delta
            text: "Delta:"
        }
        Text{
            id: roo
            text: "Roo:"
        }
    }

    Column{
        id: column2
        spacing: 10
        width: 175

        Slider{
            id: fiSlider
            tickmarksEnabled: true
            stepSize: 0
            minimumValue: 0
            maximumValue: 100
            width: parent.width
            onValueChanged: print(value)
        }

        Slider{
            id: deltaSlider
            tickmarksEnabled: true
            stepSize: 1
            minimumValue: 1
            maximumValue: 5
            width: parent.width
        }

        Slider{
            id: rooSlider
            tickmarksEnabled: true
            stepSize: 1
            minimumValue: 1
            maximumValue: 5
            width: parent.width
        }
    }
}