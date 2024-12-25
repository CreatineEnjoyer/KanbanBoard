import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property alias taskTitle: taskTitle.text
    property alias taskDescription: taskDescription.text
    property int sourceColumn: -1
    property int sourceTask: -1
    property int priority: 0

    width: 210
    height: 40  + taskDescription.height

    Rectangle {
        id: taskContainer
        width: parent.width
        height: parent.height
        color: parent.priority === 1 ? "orangered" : parent.priority === 2 ? "yellow" : "lime"
        radius: 5
        border.color: parent.priority === 1 ? "red" : parent.priority === 2 ? "orange" : "green"
        border.width: 1

        Column {
            anchors.fill: parent
            anchors.margins: 5

            RowLayout {
                Layout.fillWidth: true
                Text {
                    id: taskTitle
                    font.pixelSize: 16
                    color: "black"
                    text: "Task Title"
                }
                Item {
                    Rectangle {
                        id: deleteTask
                        width: 15
                        height: 15
                        radius: 5
                        Text {
                            text: "X"
                            anchors.centerIn: parent
                            font.pixelSize: 12
                            color: "#333333"
                        }
                    }
                    MouseArea {
                        anchors.fill: deleteTask
                        onClicked: {
                            kanbanModel.removeTask(sourceColumn, sourceTask);
                        }
                    }
                }
            }
            Text {
                id: taskDescription
                font.pixelSize: 12
                color: "black"
                text: "Task Description"
                wrapMode: Text.WordWrap
                visible: taskDescription.text.length > 0
            }
        }

        Drag.active: dragArea.drag.active
        Drag.hotSpot.x: dragArea.width / 2
        Drag.hotSpot.y: dragArea.height / 2

        MouseArea {
            propagateComposedEvents: true
            id: dragArea
            anchors.fill: parent
            drag.target: taskContainer
            drag.axis: Drag.XAndYAxis
            onPressed: {
                dragData.sourceColumn = sourceColumn;
                dragData.sourceTask = sourceTask;
                dragData.wasDropped = false;
            }
            onReleased: {
                parent.Drag.drop()
                if (!dragData.wasDropped) {
                    taskContainer.x = 0;
                    taskContainer.y = 0;
                }
            }
        }
    }

}
