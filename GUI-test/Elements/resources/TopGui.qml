import QtQuick 2.0
import OpenGLUnderQML 1.0
import QtQuick.Controls 1.1
import QtQuick.Window 2.0

Row{
    id: presetRow
    height: parent.height/3
    width: parent.width

    Slider{
        id: presetSlider
        tickmarksEnabled: true
        stepSize: 1
        minimumValue: 1
        maximumValue: 3
        width: parent.width/1.1
    }
}
