import QtQuick 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3


Item{
    id: item1
    width: 320
    height: 240

    signal cifra_changed(int posizione, int valore)

    Rectangle {
        id: rectangle
        color: "transparent"
        anchors.fill: parent

        RowLayout {
            id: rowLayout
            z: 1
            spacing: 0
            enabled: true
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            anchors.bottomMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent

            Rectangle {
                id: rectangle3
                width: 200
                height: 200
                color: "#00000000"
                border.width: 1
                Layout.fillHeight: true
                Layout.fillWidth: true

                Tumbler {
                    id: tumbler_cifra3
                    anchors.fill: parent
                    z: 2
                    spacing: 0
                    Layout.fillWidth: true
                    wrap: false
                    enabled: true
                    wheelEnabled: true
                    font.bold: true
                    font.pointSize: 21
                    visibleItemCount: 3
                    currentIndex: 0
                    Layout.fillHeight: true
                    model: 10

                    onMovingChanged:{
                        item1.cifra_changed(3, currentIndex);
                        rectangle.color="yellow";
                    }
                }
            }

            Rectangle {
                id: rectangle2
                width: 200
                height: 200
                color: "#00000000"
                border.width: 1
                Layout.fillHeight: true
                Layout.fillWidth: true

                Tumbler {
                    id: tumbler_cifra2
                    anchors.fill: parent
                    enabled: true
                    z: 2
                    Layout.fillWidth: true
                    wrap: false
                    wheelEnabled: true
                    font.bold: true
                    font.pointSize: 21
                    visibleItemCount: 3
                    currentIndex: 0
                    Layout.fillHeight: true
                    model: 10

                    onMovingChanged:{
                        item1.cifra_changed(2, currentIndex);
                        rectangle.color="yellow";
                    }
                }
            }

            Rectangle {
                id: rectangle1
                width: 200
                height: 200
                color: "#00000000"
                border.width: 1
                Layout.fillHeight: true
                Layout.fillWidth: true

                Tumbler {
                    id: tumbler_cifra1
                    anchors.fill: parent
                    enabled: true
                    z: 2
                    topPadding: 0
                    font.letterSpacing: 0
                    font.wordSpacing: 0
                    wrap: false
                    visibleItemCount: 3
                    currentIndex: 0
                    model: 10
                    wheelEnabled: true
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    font.bold: true
                    font.pointSize: 21

                    onMovingChanged:{
                        item1.cifra_changed(1, currentIndex);
                        rectangle.color="yellow";
                    }
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

    function updateTumbler(centinaia, decine, unita) {
        tumbler_cifra3.currentIndex=centinaia;
        tumbler_cifra2.currentIndex=decine;
        tumbler_cifra1.currentIndex=unita;
    }

    function sfondoBianco() {
        rectangle.color="white";
    }

    function sfondoGiallo() {
        rectangle.color="yellow";
    }
}

/*##^## Designer {
    D{i:5;anchors_x:96;anchors_y:0}
}
 ##^##*/
