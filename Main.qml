import QtQuick
import QtQuick.Controls

Window {
    id: windowId
    width: 1280
    height: 720
    visible: true
    title: qsTr("Hello World")


    Flickable {
        anchors.fill: parent
        contentWidth: kanbanColumns.width
        contentHeight: height

        Row {
            id: kanbanColumns
            spacing: 20
            Repeater {
                model: kanbanModel
                delegate: KanbanColumn {
                    columnTitle: model.name
                    columnIndex: index
                    columnTasks: model.tasks
                }
            }

            // Add Column
            Rectangle {
                width: 200
                height: windowId.height - 80
                color: "#dddddd"
                radius: 8
                border.color: "#aaaaaa"
                Text {
                    id: columnName
                    text: "+ Add Column"
                    anchors.centerIn: parent
                    font.pixelSize: 16
                    color: "#333333"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        kanbanModel.addColumn(columnName.text);
                    }
                }
            }
        }
    }
}
