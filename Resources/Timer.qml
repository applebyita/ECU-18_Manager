import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3


Item{
    id: item1
    property alias itemTimer: item1
    width: 320
    height: 300

    GroupBox {
        id: groupBox
        enabled: true
        anchors.fill: parent

        RowLayout {
            id: rowLayout
            spacing: 0
            enabled: true
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.bottomMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent

            Tumbler {
                id: tumbler_ore
                Layout.fillWidth: true
                wrap: true
                enabled: true
                wheelEnabled: true
                font.bold: true
                font.pointSize: 25
                visibleItemCount: 3
                currentIndex: MainWindow.timerore
                Layout.fillHeight: true
                model: 100

                onMovingChanged:{
                    MainWindow.timerore=currentIndex;
                }
            }

            ColumnLayout {
                id: columnLayout_ore
                width: 100
                height: 100
                Layout.fillHeight: true

                Frame {
                    id: frame_ore1
                    width: 200
                    height: 200
                    visible: false
                    Layout.fillHeight: true
                }

                Label {
                    id: label_ore
                    text: qsTr("hh")
                    font.bold: true
                    font.pointSize: 15
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.fillHeight: false
                }

                Frame {
                    id: frame_ore2
                    width: 200
                    height: 200
                    visible: false
                    Layout.fillHeight: true
                }

            }

            Tumbler {
                id: tumbler_minuti
                Layout.fillWidth: true
                wrap: true
                wheelEnabled: true
                font.bold: true
                font.pointSize: 25
                visibleItemCount: 3
                currentIndex: MainWindow.timerminuti
                Layout.fillHeight: true
                model: 60

               onMovingChanged:{
                   MainWindow.timerminuti=currentIndex;
               }
            }

            ColumnLayout {
                id: columnLayout_min
                width: 100
                height: 100
                Layout.fillHeight: true


                Frame {
                    id: frame_min1
                    width: 200
                    height: 200
                    visible: false
                    Layout.fillHeight: true
                }

                Label {
                    id: label_minuti
                    text: qsTr("mm")
                    font.bold: true
                    font.pointSize: 15
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.fillHeight: false
                }

                Frame {
                    id: frame_min2
                    width: 200
                    height: 200
                    visible: false
                    Layout.fillHeight: true
                }
            }


        }


    }

    function disableItem() {
    enabled=false;
    }

    function enableItem() {
    enabled=true;
    }
}
