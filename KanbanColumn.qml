import QtQuick
import QtQuick.Layouts

Item {
    property int columnIndex: -1
    property alias columnTitle: columnTitle.text
    property var columnTasks

    width: 200
    height: parent.height

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            Text {
                id: columnTitle
                text: "coluumnsas"
                font.pixelSize: 16
                color: "black"
                font.bold: true
                Layout.fillWidth: true
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
    }
}
