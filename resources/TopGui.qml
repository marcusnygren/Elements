import QtQuick 2.0
import ElementsEngine 1.0
import QtQuick.Controls 1.1
import QtQuick.Window 2.0

import "js/predefineValue.js" as PredefineValue

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

            MouseArea {
                anchors.fill: parent
                onPressed: { parent.source = 'pics/fire.png' }
                onReleased: { parent.source = 'pics/smoke.png' }
            }
        }

        Image{
            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: "pics/fire.png"

            MouseArea {
                anchors.fill: parent
                onClicked: { parent.color = 'green' }
            }
        }

        Image{
            width: (parent.width*(1/3) - 10)
            height: parent.height
            source: "pics/water.png"

            MouseArea {
                anchors.fill: parent
                onClicked: { parent.color = 'blue' }
            }
        }
    }
}
