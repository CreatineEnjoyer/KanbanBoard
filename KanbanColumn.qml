import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    id: col
    property int columnIndex: -1
    property alias columnTitle: columnText.text
    property var columnTasks

    width: 200
    height: parent.height
    Row {
        width: col.width
        Layout.maximumHeight: 40
        Text {
            id: columnText
            text: ""
            font.pixelSize: 16
            color: "black"
            font.bold: true
            width: col.width - 20
            height: 40
        }
        Button {
            text: "X"
            onClicked: {
                kanbanModel.removeColumn(columnIndex);
            }
        }
    }

    Flickable {
        id: flickable
        contentHeight: tasksList.contentHeight
        Layout.minimumHeight: parent.height
        ListView {
            id: tasksList
            anchors.fill: parent
            model: columnTasks

            delegate: KanbanTask {
                taskTitle: model.title
                taskDescription: model.description
                sourceColumn: columnIndex
                sourceTask: index
                priority: model.priority
            }
            spacing: 5
        }

        DropArea {
            id: dropArea
            anchors.top: tasksList.bottom
            width: col.width
            height: 100

            Rectangle {
                id:dropRect
                width: parent.width
                height: parent.height
                color: "#55dddddd"
                radius: 8
                border.color: "#aaaaaa"
                visible: false
            }
            Text {
                id: dropText
                text: "Drop here"
                anchors.centerIn: parent
                font.pixelSize: 16
                color: "#333333"
                visible: false
            }

            onEntered: {
                dropRect.visible = true;
                dropText.visible = true;
            }
            onExited: {
                dropRect.visible = false;
                dropText.visible = false;
            }
            onDropped: {
                var sourceColumn = dragData.sourceColumn;
                var sourceTask = dragData.sourceTask;

                dropRect.visible = false;
                dropText.visible = false;

                if (sourceColumn != columnIndex && columnIndex != -1 && sourceColumn != -1) {
                    dragData.wasDropped = true;
                    kanbanModel.moveTask(sourceColumn, sourceTask, columnIndex);                    
                }
            }
        }

        // Add Task
        Item {
            Layout.fillWidth: true
            anchors.top: dropArea.bottom

            Rectangle {
                id: newTask
                anchors.top: taskDesc.bottom
                height: 50
                color: "#dddddd"
                radius: 8
                border.color: "#aaaaaa"
                width: col.width
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
                width: col.width
                text: "Set Task Title"
                font.pixelSize: 14
                color: "black"
                font.bold: true
            }
            TextField {
                id: taskPriority
                anchors.bottom: taskDesc.top
                width: col.width
                text: "Set Task Priority 1 > 3"
                font.pixelSize: 12
                color: "black"
                validator: IntValidator{bottom: 1; top: 3}
            }
            TextArea {
                id: taskDesc
                width: col.width
                text: "Set Task Description"
                font.pixelSize: 12
                color: "black"
            }
        }
    }
}
