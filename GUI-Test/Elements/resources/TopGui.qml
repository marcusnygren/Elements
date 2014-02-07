import QtQuick 2.0
import OpenGLUnderQML 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Controls 1.1
import QtQuick.Window 2.0

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
            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: "pics/smoke.png"
        }

        Image{
            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: "pics/fire.png"
        }

        Image{
            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: "pics/water.png"
        }
    }

    Slider{
        id: presetSlider
        tickmarksEnabled: true
        stepSize: 1
        minimumValue: 1
        maximumValue: 3
        width: (parent.width - 10)
        style: SliderStyle{ groove: Rectangle {radius: 5; height: 5; color: "lightgray"}}
    }
}
