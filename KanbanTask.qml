import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: kanbanTask
    property alias taskTitle: taskTitle.text
    property alias taskDescription: taskDescription.text
    property int sourceColumn: -1
    property int sourceTask: -1
    property string priority: ""

    width: 210
    height: 40  + taskDescription.height

    opacity: 0
    scale: 0.8
    property int durationAnimation: 300

    Behavior on opacity {
        NumberAnimation { duration: kanbanTask.durationAnimation; easing.type: Easing.InOutQuad }
    }
    Behavior on scale {
        NumberAnimation { duration: kanbanTask.durationAnimation; easing.type: Easing.InOutQuad }
    }
    Component.onCompleted: {
        opacity = 1;
        scale = 1;
    }

    // Timer to delay removing task for started animations
    Timer {
        id: deleteTimer
        interval: parent.durationAnimation
        repeat: false
        onTriggered: {
            kanbanModel.removeTask(sourceColumn, sourceTask);
        }
    }

    Rectangle {
        id: taskContainer
        width: parent.width
        height: parent.height
        color: parent.priority == "High" ? "orangered" : parent.priority == "Medium" ? "yellow" : "lime"
        radius: 5
        border.color: parent.priority == "High" ? "red" : parent.priority == "Medium" ? "orange" : "green"
        border.width: 1

        // Task title
        Column {
            anchors.fill: parent
            anchors.margins: 5

            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                spacing: 5
                Text {
                    id: taskTitle
                    font.pixelSize: 16
                    color: "black"
                    text: ""
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }
            }
            Text {
                id: taskDescription
                font.pixelSize: 12
                color: "black"
                text: ""
                wrapMode: Text.WordWrap
                visible: taskDescription.text.length > 0
            }
        }

        // Edit task button
        Item {
            Rectangle {
                id: editTask
                width: 30
                height: 15
                radius: 5
                color: "white"
                border.color: "#333333"
                border.width: 1
                Text {
                    text: "Edit"
                    anchors.centerIn: parent
                    font.pixelSize: 12
                    color: "#333333"
                }
            }
            MouseArea {
                anchors.fill: editTask
                onClicked: {
                    editDialog.open();
                }
            }
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: editTask.width + deleteTask.width
        }

        // Delete task button
        Item {
            Rectangle {
                id: deleteTask
                width: 15
                height: 15
                radius: 5
                color: "white"
                border.color: "#333333"
                border.width: 1
                Text {
                    text: "X"
                    anchors.centerIn: parent
                    font.pixelSize: 12
                    color: "#333333"
                }
            }
            MouseArea {
                anchors.fill: deleteTask
                onClicked: {
                    kanbanTask.opacity = 0;
                    kanbanTask.scale = 0;
                    deleteTimer.start();
                }
            }
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: deleteTask.width
        }

        // Dialog popup to edit task
        Dialog {
            id: editDialog
            title: "Edit Task"
            standardButtons: Dialog.Ok | Dialog.Cancel
            visible: false
            modal: true

            ColumnLayout {
                spacing: 10
                anchors.margins: 20

                TextField {
                    id: editTitle
                    text: taskTitle.text
                    placeholderText: "Edit Task Title"
                }

                ComboBox {
                    id: editPriority
                    model: ["High", "Medium", "Low"]
                    currentIndex: ["High", "Medium", "Low"].indexOf(priority)
                }

                TextArea {
                    id: editDescription
                    text: taskDescription.text
                    placeholderText: "Edit Task Description"
                }
            }

            onAccepted: {
                kanbanModel.editTask(sourceColumn, sourceTask, editTitle.text, editDescription.text, editPriority.currentText);
                taskTitle.text = editTitle.text;
                taskDescription.text = editDescription.text;
                priority.text = editPriority.currentText;
            }
        }

        // Drag task functionality
        Drag.active: dragArea.drag.active
        Drag.hotSpot.x: dragArea.width / 2
        Drag.hotSpot.y: dragArea.height / 2

        MouseArea {
            propagateComposedEvents: true
            id: dragArea
            anchors.fill: parent
            drag.target: taskContainer
            drag.axis: Drag.XAndYAxis
            onPressed: {
                dragData.sourceColumn = sourceColumn;
                dragData.sourceTask = sourceTask;
                dragData.wasDropped = false;
            }
            onReleased: {
                parent.Drag.drop()
                if (!dragData.wasDropped) {
                    taskContainer.x = 0;
                    taskContainer.y = 0;
                }
            }
        }
    }
}
