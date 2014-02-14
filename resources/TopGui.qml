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
            property string filename: "pics/fire_inactive.png"

            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: filename

            MouseArea {
                anchors.fill: parent
                onClicked: { 
                    fiSlider.value = 50
                    deltaSlider.value = 2
                    rooSlider.value = 3
                }
            }
        }

        Image{
            id: smoke
            property string filename: "pics/smoke_inactive.png"

            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: filename

            MouseArea {
                anchors.fill: parent
                onClicked: { PreValue.smokeSettings() }
            }
        }

        Image{
            id: water
            property string filename: "pics/smoke_inactive.png"

            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: filename

            MouseArea {
                anchors.fill: parent
                onClicked: { PreValue.waterSettings() }
            }
        }
    }
}
