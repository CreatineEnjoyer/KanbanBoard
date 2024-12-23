import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    property int columnIndex: -1
    property alias columnTitle: columnText.text
    property var columnTasks

    width: 200
    height: parent.height

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            Text {
                id: columnText
                text: ""
                font.pixelSize: 16
                color: "black"
                font.bold: true
                Layout.fillWidth: true
            }
            Button {
                text: "X"
                onClicked: {
                    kanbanModel.removeColumn(columnIndex);
                }
            }
        }

        ListView {
            id: tasksList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: columnTasks
            delegate: KanbanTask {
                taskTitle: model.title
                taskDescription: model.description
                sourceColumn: columnIndex
                sourceTask: index
                priority: model.priority
            }
        }
        // Add Task
        Item {
            Layout.fillWidth: true
            Rectangle {
                id: newTask
                anchors.top: taskDesc.bottom
                height: 50
                color: "#dddddd"
                radius: 8
                border.color: "#aaaaaa"
                width: 200
                Text {
                    text: "+ Add Task"
                    anchors.centerIn: parent
                    font.pixelSize: 16
                    color: "#333333"
                }
            }
            MouseArea {
                anchors.fill: newTask
                onClicked: {
                    if (taskTitle.text.length != 0 && taskDesc.text.length != 0) {
                        kanbanModel.addTask(columnIndex, taskTitle.text, taskDesc.text, taskPriority.text);
                        taskTitle.text = ""
                        taskDesc.text = ""
                        taskPriority.text = ""
                    }
                }
            }
            TextField {
                id: taskTitle
                anchors.bottom: taskPriority.top
                width: 200
                text: "Set Task Title"
                font.pixelSize: 14
                color: "black"
                font.bold: true
            }
            TextField {
                id: taskPriority
                anchors.bottom: taskDesc.top
                width: 200
                text: "Set Task Priority 1 > 3"
                font.pixelSize: 12
                color: "black"
                validator: IntValidator{bottom: 1; top: 3}
            }
            TextArea {
                id: taskDesc
                width: 200
                text: "Set Task Description"
                font.pixelSize: 12
                color: "black"
            }
        }
    }
}
