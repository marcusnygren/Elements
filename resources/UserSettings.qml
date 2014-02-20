import QtQuick 2.0
import ElementsEngine 1.0
import QtQuick.Controls 1.1
import QtQuick.Window 2.0


Column{
    id: presetColumn
    height: parent.height/3
    width: parent.width
    spacing: 10

    Row{
        id: resizeWindow
        width: parent.width
        height: parent.height*(1/10)
        spacing: 50

        Rectangle{
            id: minimizeButton
            color: Qt.rgba(0.2, 0.2, 0.2, 0)
            width: 20
            height: parent.height
        
            Text{
                id: minimize
                text: "-"
                color: "white"
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
                    dragMouseArea.height = 500
                    koefRow.visible = false
                    elementPicker.visible = false
                }
            }
        }

        Rectangle{
            id: maximizeButton
            color: Qt.rgba(0.2, 0.2, 0.2, 0)
            width: 20
            height: parent.height
            
            anchors.left: minimizeButton
        
            Text{
                id: maximize
                text: "+"
                color: "white"
                z: 4

                Tooltip {
                    id: toolTipMaximize
                    text: "Testing Tooltip Testing"
                    visible: mouseAreaMinimize.hoverEnabled
                }

                MouseArea {
                    id: mouseAreaMaximize
                    hoverEnabled: true
                    onEntered: { toolTipMinimize.opacity = 1.0 }
                    onExited: { toolTipMinimize.opacity = 0.0 }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: { 
                    dragMouseArea.height = 500
                    koefRow.visible = true
                    elementPicker.visible = true
                }
            }
        }
    }

    /*Row{
        id: maximizeWindow
        width: 50
        height: parent.height*(1/10)
        spacing: 50
        anchors.left: minimizeButton.right;


    }*/

    //TopGUI
    Row{
        id: elementPicker
        height: parent.height*(2/3)
        width: parent.width
        spacing: 10

        Image{
            id: smoke
            source: "pics/Smoke.png"
            width: (parent.width*(1/3) - 10)
            height: parent.height
            opacity: 0.60
            verticalAlignment: AlignBottom;


            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: { if(parent.opacity != 1.00){playSmokeEnterHover.start()} }
                onExited: { if(parent.opacity != 1.00){playSmokeExitHover.start()} }
                onClicked: { 
                    parent.opacity = 1.00
                    fiSlider.value = Math.floor(Math.random()*(fiSlider.maximumValue-fiSlider.minimumValue+1)+fiSlider.minimumValue)
                    deltaSlider.value = Math.floor(Math.random()*(deltaSlider.maximumValue-deltaSlider.minimumValue+1)+deltaSlider.minimumValue)
                    rooSlider.value = Math.floor(Math.random()*(rooSlider.maximumValue-rooSlider.minimumValue+1)+rooSlider.minimumValue)
                    playFireExitHover.start()
                    playWaterExitHover.start()
                }
            }

            SequentialAnimation {
                id: playSmokeEnterHover
                running: false
                NumberAnimation { target: smoke; property: "opacity"; to: 0.8; duration: 200}
            }
            SequentialAnimation {
                id: playSmokeExitHover
                running: false
                NumberAnimation { target: smoke; property: "opacity"; to: 0.5; duration: 200}
            }
        }

        Image{
            id: fire
            source: "pics/Fire.png"
            width: (parent.width*(1/3) - 10)
            height: parent.height
            opacity: 0.60
            verticalAlignment: AlignBottom;

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: { if(parent.opacity != 1.00){playFireEnterHover.start()} }
                onExited: { if(parent.opacity != 1.00){playFireExitHover.start()} }
                onClicked: { 
                    parent.opacity = 1.00
                    fiSlider.value = Math.floor(Math.random()*(fiSlider.maximumValue-fiSlider.minimumValue+1)+fiSlider.minimumValue)
                    deltaSlider.value = Math.floor(Math.random()*(deltaSlider.maximumValue-deltaSlider.minimumValue+1)+deltaSlider.minimumValue)
                    rooSlider.value = Math.floor(Math.random()*(rooSlider.maximumValue-rooSlider.minimumValue+1)+rooSlider.minimumValue)
                    playSmokeExitHover.start()
                    playWaterExitHover.start()
                }
            }

            SequentialAnimation {
                id: playFireEnterHover
                running: false
                NumberAnimation { target: fire; property: "opacity"; to: 0.8; duration: 200}
            }
            SequentialAnimation {
                id: playFireExitHover
                running: false
                NumberAnimation { target: fire; property: "opacity"; to: 0.5; duration: 200}
            }
        }

        Image{
            id: water
            source: "pics/Water.png"
            width: (parent.width*(1/3) - 10)
            height: parent.height
            opacity: 0.50
            verticalAlignment: AlignBottom;

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: { if(water.opacity != 1.00){playWaterEnterHover.start()} }
                onExited: { if(water.opacity != 1.00){playWaterExitHover.start()} }
                onClicked: { 
                    parent.opacity = 1.00
                    fiSlider.value = Math.floor(Math.random()*(fiSlider.maximumValue-fiSlider.minimumValue+1)+fiSlider.minimumValue)
                    deltaSlider.value = Math.floor(Math.random()*(deltaSlider.maximumValue-deltaSlider.minimumValue+1)+deltaSlider.minimumValue)
                    rooSlider.value = Math.floor(Math.random()*(rooSlider.maximumValue-rooSlider.minimumValue+1)+rooSlider.minimumValue)
                    playSmokeExitHover.start()
                    playFireExitHover.start()
                }
            }

            SequentialAnimation {
                id: playWaterEnterHover
                running: false
                NumberAnimation { target: water; property: "opacity"; to: 0.8; duration: 200}
            }
            SequentialAnimation {
                id: playWaterExitHover
                running: false
                NumberAnimation { target: water; property: "opacity"; to: 0.5; duration: 200}
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
            color: container.color

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
                        color: "white"
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
                        color: "white"
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
                        color: "white"
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
                        onValueChanged: print(water.width, water.height)
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
