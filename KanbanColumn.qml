import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    id: col
    property int columnIndex: -1
    property alias columnTitle: columnText.text
    property var columnTasks
    property var colAreaWidth: width - 20 - margins
    property var margins: 10
    width: 240
    height: parent.height

    // Column name
    Row {
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

    // Scrollable area
    Flickable {
        id: flickable
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true
        rightMargin: parent.margins

        // Dynamic height of scrollbar area
        contentHeight: tasksList.contentHeight + dropArea.height + addTaskArea.height
        interactive: true

        // Scrollbar
        ScrollBar.vertical: ScrollBar {
            id: verticalScrollBar
            policy: ScrollBar.AlwaysOn
        }

        // Task list
        ListView {
            id: tasksList
            width: col.colAreaWidth
            height: tasksList.contentHeight
            model: col.columnTasks
            spacing: 5

            delegate: KanbanTask {
                taskTitle: model.title
                taskDescription: model.description
                sourceColumn: columnIndex
                sourceTask: index
                priority: model.priority
            }
        }

        // Drop area for moving tasks
        DropArea {
            id: dropArea
            width: col.colAreaWidth
            height: 70
            anchors.top: tasksList.bottom

            Rectangle {
                id: dropRect
                width: col.colAreaWidth
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

        // Add Task Area
        Item {
            id: addTaskArea
            width: col.colAreaWidth
            height: taskTitle.height + taskPriority.height + taskDesc.height + newTask.height + 20
            anchors.top: dropArea.bottom

            TextField {
                id: taskTitle
                width: parent.width
                placeholderText: "Set Task Title"
                font.pixelSize: 14
                color: "black"
                font.bold: true
            }
            TextField {
                id: taskPriority
                anchors.top: taskTitle.bottom
                width: parent.width
                placeholderText: "Set Task Priority 1 > 3"
                font.pixelSize: 12
                color: "black"
                validator: IntValidator { bottom: 1; top: 3 }
            }
            TextArea {
                id: taskDesc
                anchors.top: taskPriority.bottom
                width: parent.width
                placeholderText: "Set Task Description"
                font.pixelSize: 12
                color: "black"
            }

            // Add new task button
            Rectangle {
                id: newTask
                anchors.top: taskDesc.bottom
                height: 50
                color: "#dddddd"
                radius: 8
                border.color: "#aaaaaa"
                width: parent.width
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
        }
    }
}
