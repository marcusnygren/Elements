import QtQuick 2.0
import ElementsEngine 1.0
import QtQuick.Controls 1.1
import QtQuick.Window 2.0

import "js/predefineValue.js" as PreValue

Column{
    id: presetColumn
    height: parent.height/3
    width: parent.width
    spacing: 10

    Row{
        height: parent.height*(2/3)
        width: parent.width
        spacing: 10

        Image{
            id: fire
            property string imagepath: "pics/fire_inactive.png"

            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: imagepath

            MouseArea {
                anchors.fill: parent
                onClicked: { 
                    PreValue.fireSettings()
                    fiSlider.value = 50;
                    deltaSlider.value = 2;
                    rooSlider.value = 3;
                }
            }
        }

        Image{
            id: smoke
            property string imagepath = "pics/smoke_inactive.png"

            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: imagepath

            MouseArea {
                anchors.fill: parent
                onClicked: { PreValue.smokeSettings() }
            }
        }

        Image{
            id: water
            property string imagepath: "pics/smoke_inactive.png"

            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: imagepath

            MouseArea {
                anchors.fill: parent
                onClicked: { PreValue.waterSettings() }
            }
        }
    }
}

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
                    z: 5

                    Tooltip {
                        id: toolTipFi
                        text: "Testing Tooltip Testing"
                        visible: mouseAreaFi.hoverEnabled
                    }

                    MouseArea {
                        id: mouseAreaFi
                        hoverEnabled: true
                        onEntered: { toolTipFi.opacity = 1.0 }
                        onExited: { toolTipFi.opacity = 0.0 }
                        anchors.fill: parent                        
                    }
                }
                Text{
                    id: delta
                    text: "Delta:"
                    z: 4

                    Tooltip {
                        id: toolTipDelta
                        text: "Testing Tooltip Testing"
                        visible: mouseAreaDelta.hoverEnabled
                    }

                    MouseArea {
                        id: mouseAreaDelta
                        hoverEnabled: true
                        onEntered: { toolTipDelta.opacity = 1.0 }
                        onExited: { toolTipDelta.opacity = 0.0 }
                        anchors.fill: parent
                    }
                }
                Text{
                    id: roo
                    text: "Roo:"
                    z: 3

                    Tooltip {
                        id: toolTipRoo
                        text: "Testing Tooltip Testing"
                        visible: mouseAreaRoo.hoverEnabled
                    }

                    MouseArea {
                        id: mouseAreaRoo
                        hoverEnabled: true
                        onEntered: { toolTipRoo.opacity = 1.0 }
                        onExited: { toolTipRoo.opacity = 0.0 }
                        anchors.fill: parent
                    }
                }
            }

            Column{
                id: koefSliderColumn
                spacing: 10
                width: parent.width*(5/7)
                z: -1

                Slider{
                    id: fiSlider
                    /*property int fiSliderValue
                    value: fiSliderValue*/
                    tickmarksEnabled: true
                    stepSize: 0
                    minimumValue: 0
                    maximumValue: 100
                    width: parent.width
                    onValueChanged: print(Window.width)
                }

                Slider{
                    id: deltaSlider
                    /*property int deltaSliderValue
                    value: deltaSliderValue*/
                    tickmarksEnabled: true
                    stepSize: 1
                    minimumValue: 1
                    maximumValue: 5
                    width: parent.width
                }

                Slider{
                    id: rooSlider
                    /*property int rooSliderValue
                    value: rooSliderValue*/
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