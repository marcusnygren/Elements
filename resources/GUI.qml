import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQml 2.0
import QtQuick.Window 2.0

import ElementsEngine 1.0
import ElementsDataHandler 1.0

Item {
    id: item1
/*
    width: Screen.width
    height: Screen.height
*/
	width: 800
	height: 600
	
	// For data synchronizing 
	Engine 
	{
		id: engine;
		objectName: "engine"

		width: 800
		height: 600

		Data 
		{
			id: data
			Component.onCompleted:
			{
				engine.setData(data);
			}
		}
	}

    Rectangle {
        opacity: 0.5
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 0
        border.width: 1
        border.color: "darkgray"
        anchors.fill: label
        anchors.margins: -10
    }

    Text {
        id: label
        color: Qt.rgba(0, 0, 0, 0.5)
        wrapMode: Text.WordWrap
        text: "The background here is a squircle rendered with raw OpenGL using the 'beforeRender()' signal in QQuickWindow. This text label and its border is rendered using QML"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }

	MouseArea {
        id: dragMouseArea
        width: 250
        height: 500
        //anchors {right : item1.right; top: item1.top}
        drag.target: this
        onReleased: parent = container.Drag.target !== null ? container.Drag.target : item1

		Rectangle {
			id: container
            radius: 0
			border.width: 1
			border.color: "white"
			width: 250
			height: 500
            color: Qt.rgba(0.5, 0.5, 0.5, 0.5)

			Column{
				id: guiComposition
				spacing: 10
				height: parent.height
				width: parent.width*(19/20)
				anchors {right: container.right; top: container.top; topMargin: 15}
                //TopGui{}
				BottomGui{}
			}
		}
	}
}