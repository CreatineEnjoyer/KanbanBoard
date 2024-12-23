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
                TextField {
                    id: columnName
                    width: parent.width
                    placeholderText: "Set Column Name"
                    font.pixelSize: 16
                    color: "black"
                    font.bold: true
                }
                Rectangle {
                    id: newColumn
                    anchors.top: columnName.bottom
                    height: parent.height + 50
                    color: "#dddddd"
                    radius: 8
                    border.color: "#aaaaaa"
                    width: parent.width
                    Text {
                        text: "+ Add Column"
                        anchors.centerIn: parent
                        font.pixelSize: 16
                        color: "#333333"
                    }
                }
                MouseArea {
                    anchors.fill: newColumn
                    onClicked: {
                        if (columnName.text.length != 0) {
                            kanbanModel.addColumn(columnName.text);
                            columnName.text = ""
                        }
                    }
                }
            }
        }
    }
    QtObject {
        id: dragData
        property int sourceColumn: -1
        property int sourceTask: -1
        property bool wasDropped: false
    }
}
