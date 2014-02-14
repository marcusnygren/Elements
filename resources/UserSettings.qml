import QtQuick 2.0
import ElementsEngine 1.0
import QtQuick.Controls 1.1
import QtQuick.Window 2.0


Column{
    id: presetColumn
    height: parent.height/3
    width: parent.width
    spacing: 100

    Row{
        id: resizeWindow
        width: parent.width
        height: parent.height*(1/10)
        spacing: 200

        Rectangle{
            color: Qt.rgba(0, 0, 0, 0.25)
            width: parent.width
            height: parent.height
        
            Text{
                id: minimize
                text: "Minimize window"
                z: 4

                Tooltip {
                    id: toolTipMinimize
                    text: "Testing Tooltip Testing"
                    visible: mouseAreaMinimize.hoverEnabled
                }

                MouseArea {
                    id: mouseAreaMinimize
                    hoverEnabled: true
                    onEntered: { toolTipMinimize.opacity = 1.0 }
                    onExited: { toolTipMinimize.opacity = 0.0 }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: { 
                    dragMouseArea.height = 50
                    koefRow.visible = false
                    elementPicker.visible = false
                }
            }
        }
    }

    //TopGUI
    Row{
        id: elementPicker
        height: parent.height*(2/3)
        width: parent.width
        spacing: 10

        Image{
            id: smoke
            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: "pics/smoke_inactive.png"

            MouseArea {
                anchors.fill: parent
                onClicked: { 
                    parent.source = 'pics/smoke2.png'
                    fiSlider.value = Math.floor(Math.random()*(fiSlider.maximumValue-fiSlider.minimumValue+1)+fiSlider.minimumValue)
                    deltaSlider.value = Math.floor(Math.random()*(deltaSlider.maximumValue-deltaSlider.minimumValue+1)+deltaSlider.minimumValue)
                    rooSlider.value = Math.floor(Math.random()*(rooSlider.maximumValue-rooSlider.minimumValue+1)+rooSlider.minimumValue)
                    fire.source = "pics/fire_inactive.png"
                    water.source = "pics/water_inactive.png"
                }
            }
        }

        Image{
            id: fire
            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: "pics/fire_inactive.png"

            MouseArea {
                anchors.fill: parent
                onClicked: { 
                    parent.source = 'pics/fire2.png'
                    fiSlider.value = Math.floor(Math.random()*(fiSlider.maximumValue-fiSlider.minimumValue+1)+fiSlider.minimumValue)
                    deltaSlider.value = Math.floor(Math.random()*(deltaSlider.maximumValue-deltaSlider.minimumValue+1)+deltaSlider.minimumValue)
                    rooSlider.value = Math.floor(Math.random()*(rooSlider.maximumValue-rooSlider.minimumValue+1)+rooSlider.minimumValue)
                    smoke.source = "pics/smoke_inactive.png"
                    water.source = "pics/water_inactive.png"
                }
            }
        }

        Image{
            id: water
            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: "pics/water_inactive.png"

            MouseArea {
                anchors.fill: parent
                onClicked: { 
                    parent.source = 'pics/water2.png'
                    fiSlider.value = Math.floor(Math.random()*(fiSlider.maximumValue-fiSlider.minimumValue+1)+fiSlider.minimumValue)
                    deltaSlider.value = Math.floor(Math.random()*(deltaSlider.maximumValue-deltaSlider.minimumValue+1)+deltaSlider.minimumValue)
                    rooSlider.value = Math.floor(Math.random()*(rooSlider.maximumValue-rooSlider.minimumValue+1)+rooSlider.minimumValue)
                    smoke.source = "pics/smoke_inactive.png"
                    fire.source = "pics/fire_inactive.png"
                }
            }
        }
    }

    //BottomGUI
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
}