import QtQuick 2.0
import OpenGLUnderQML 1.0
import QtQuick.Controls 1.1
import QtQuick.Window 2.0

Row{
    id: koefRow
    width: parent.width
    height: parent.height*(2/3)
    spacing: 10

    Rectangle{
        id: containerBackground
        width: parent.width*(19/20)
        height: parent.height*(3/4)
        color: Qt.rgba(0, 0, 0, 0.25)

        Row{
            id: columnContainer
            width: parent.width
            height: parent.height
            spacing: 10
            anchors {right: containerBackground.right; top: containerBackground.top; topMargin: 15; rightMargin: -10}

            Column{
                id: koefTextColumn
                spacing: 10
                width: parent.width/7

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
                id: koefSliderColumn
                spacing: 10
                width: parent.width*(5/7)

                Slider{
                    id: fiSlider
                    tickmarksEnabled: true
                    stepSize: 0
                    minimumValue: 0
                    maximumValue: 100
                    width: parent.width
                    onValueChanged: print(Window.width)
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
    }
}
