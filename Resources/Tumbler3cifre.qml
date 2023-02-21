import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3


Item{
    id: item1
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
                id: tumbler_cifra3
                Layout.fillWidth: true
                wrap: true
                enabled: true
                wheelEnabled: true
                font.bold: true
                font.pointSize: 25
                visibleItemCount: 3
                currentIndex: 0
                Layout.fillHeight: true
                model: 10

                onMovingChanged:{
                //    MainWindow.timerore=currentIndex;
                }
            }

            Tumbler {
                id: tumbler_cifra2
                Layout.fillWidth: true
                wrap: true
                wheelEnabled: true
                font.bold: true
                font.pointSize: 25
                visibleItemCount: 3
                currentIndex: 0
                Layout.fillHeight: true
                model: 10

                onMovingChanged:{
                //    MainWindow.timerminuti=currentIndex;
                }
            }

            Tumbler {
                id: tumbler_cifra1
                wrap: true
                visibleItemCount: 3
                currentIndex: 0
                model: 10
                wheelEnabled: true
                Layout.fillHeight: true
                Layout.fillWidth: true
                font.bold: true
                font.pointSize: 25

                onMovingChanged:{

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
