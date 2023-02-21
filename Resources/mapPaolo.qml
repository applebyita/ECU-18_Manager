import QtQuick 2.11
import QtWebView 1.1
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Item {
    id: item1
    property string pageHTML: mainWindow.fileHTML
    focus: false

    WebView{
        id: webView1
        url: "about:blank"
        anchors.fill: parent
        scale: 1
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        visible: true
    }

    onWidthChanged:{
//    webView1.loadHtml(mainWindow.fileHTML);
    }

    function reloadMap() {
    webView1.loadHtml(pageHTML);
    item1.visible=true;
    item1.focus=false;
    }

    function eraseMap() {
    webView1.reload();
    item1.visible=false;
    }

}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
