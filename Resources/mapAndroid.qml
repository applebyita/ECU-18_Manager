import QtQuick 2.11
import QtWebView 1.1
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3


Item {
    id: item1
    property string pageHTML: mainWidget.fileHTML
    property string urlMap: mainWidget.urlMap
    width: 300
    height: 400

    WebView{
        id: webView1
        url: "about:blank"
        anchors.fill: parent
        scale: 1
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        visible: false

        onLoadingChanged: {
            if (loadRequest.errorString)
                console.error(loadRequest.errorString);
        }
    }

    Rectangle {
        id: rectangle1
        color: "#ffffff"
        anchors.fill: parent
        visible: true

        Text {
            id: text1
            text: qsTr("Loading...")
            font.bold: false
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 25
        }
    }

    onWidthChanged:{
        //    webView1.loadHtml(mainWindow.fileHTML);
    }

    function reloadMap() {
        console.log("carico HTML:")
        webView1.visible=true;
        webView1.stop();
        webView1.loadHtml(pageHTML);
        webView1.update();
        console.log(pageHTML);
        rectangle1.visible = false;
    }

    function reloadMapFromUrl() {
        console.log("carico URL:")
        webView1.visible=true;
        webView1.stop();
        //webView1.url=urlMap;
        webView1.url="http://www.google.it";
        webView1.reload();
        console.log("Url=");
        console.log(webView1.url);
        rectangle1.visible = false;
    }

    function eraseMap() {
        webView1.visible=false;
        rectangle1.visible = true;
        console.log("carico Blank!!!")
        //webView1.loadHtml(pageHTMLBlank);
        webView1.url=urlMap;
        webView1.reload();
        console.log("Url=");
        console.log(webView1.url);
 //       webView1.loadHtml(pageHTML);
    }

    function turnOff(){
        item1.visible=false;
        webView1.url="about:blank";
        webView1.reload();
    }

    function turnOn(){
        item1.visible=true;
    }

    function setSize( itemWidth, itemHeight){
       item1.width=itemWidth;
       item1.height=itemHeight;
    }

}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;invisible:true}D{i:3;anchors_height:200;anchors_width:200}
}
 ##^##*/
