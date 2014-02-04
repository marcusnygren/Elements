import QtQuick 2.0
import OpenGLUnderQML 1.0
import QtQuick.Controls 1.1

Row{
    id: presetRow
    anchors.left: parent.left
    anchors.top: parent.top
    height: 150
    width: 235

    Slider{
        tickmarksEnabled: true
        stepSize: 1
        minimumValue: 1
        maximumValue: 5
        width: 100
        id: presetSlider
    }
}