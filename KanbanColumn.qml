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
            }
        }
        Button {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            text: "+ Add Task"
            font.bold: true
            onClicked: {
                kanbanModel.addTask(columnIndex, "New Task", "Description");
            }
        }
    }
}
