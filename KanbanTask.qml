import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property alias taskTitle: taskTitle.text
    property alias taskDescription: taskDescription.text
    property int sourceColumn: -1
    property int sourceTask: -1

    width: parent.width
    height: 50

    Rectangle {
        id: taskContainer
        width: parent.width
        height: parent.height
        color: "lightblue"
        radius: 5
        border.color: "blue"
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
                color: "gray"
                text: "Task Description"
                wrapMode: Text.WordWrap
                visible: taskDescription.text.length > 0
            }
        }
    }
}
