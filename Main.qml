import QtQuick
import QtQuick.Controls

Window {
    id: windowId
    width: 1280
    height: 720
    visible: true
    title: qsTr("Kanban Board")
    property var margins: 10
    property var columnAreaWidth: width - margins * 2
    property var columnAreaHeight: height - margins * 2

    Flickable {
        anchors.fill: parent
        contentWidth: kanbanColumns.width
        contentHeight: height
        topMargin: windowId.margins
        bottomMargin: windowId.margins
        leftMargin: windowId.margins
        rightMargin: windowId.margins

        Row {
            id: kanbanColumns
            spacing: 20
            height: windowId.columnAreaHeight
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
                height: windowId.columnAreaHeight - 80
                TextField {
                    id: columnName
                    width: parent.width
                    placeholderText: "Set Column Name"
                    font.pixelSize: 16
                    color: "black"
                    font.bold: true
                    validator: RegularExpressionValidator { regularExpression: /.{2,20}/ }
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
                    hoverEnabled: true
                    onEntered: newColumn.color = "#bbbbbb"
                    onExited: newColumn.color = "#dddddd"
                    onClicked: {
                        if (columnName.text.length != 0 && kanbanModel.checkUniqueness(columnName.text)) {
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
