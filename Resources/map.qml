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

    signal destroyMap()

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

//        Button {
//            id: button
//            text: qsTr("Close Map")
//            Layout.fillHeight: true
//            Layout.maximumHeight: 40
//            Layout.maximumWidth: 300
//            Layout.minimumHeight: 40
//            Layout.minimumWidth: 300
//            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
//            Layout.fillWidth: true

//            onClicked: {
//                //
//                destroyMap();
//                Qt.quit(0);
//            }
//        }

        WebView{
            id: webView1
            Layout.fillHeight: true
            Layout.minimumHeight: 1
            Layout.minimumWidth: 1
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            url: "about:blank"
            scale: 1
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            visible: true

            onLoadingChanged: {
                if (loadRequest.errorString)
                    console.error(loadRequest.errorString);
            }
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
    }

    function reloadMapFromUrl() {
        console.log("carico URL:")
        webView1.visible=true;
        webView1.stop();
        webView1.url=urlMap;
        webView1.reload();
        console.log("Url=");
        console.log(webView1.url);
    }

    function eraseMap() {
        webView1.visible=false;
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
    D{i:4;anchors_height:100;anchors_width:100}
}
 ##^##*/
