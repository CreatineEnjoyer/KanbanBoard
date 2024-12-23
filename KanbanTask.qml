import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property alias taskTitle: taskTitle.text
    property alias taskDescription: taskDescription.text
    property int sourceColumn: -1
    property int sourceTask: -1
    property int priority: 0

    width: 200
    height: 40  + taskDescription.height

    Rectangle {
        id: taskContainer
        width: parent.width
        height: parent.height
        color: priority === 1 ? "orangered" : priority === 2 ? "yellow" : "lime"
        radius: 5
        border.color: priority === 1 ? "red" : priority === 2 ? "orange" : "green"
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
                Button {
                    text: "X"
                    onClicked: {
                        kanbanModel.removeTask(sourceColumn, sourceTask);
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
    }
}
