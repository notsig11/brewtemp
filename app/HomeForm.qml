import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtCharts 2.0

Page {
    width: 720 / 2
    height: 1280 / 2

    property bool firstLoad: true
    property double maxTemp

    function toMsecsSinceEpoch(date) {
        var msecs = date.getTime();
        return msecs;
    }

    function fetchData() {
        var request = new XMLHttpRequest();
        request.onreadystatechange = function() {
            if(request.readyState == XMLHttpRequest.DONE) {
                var serverData = JSON.parse(request.responseText);

                if(serverData.length < 1) {
//                    console.log("No results received.");
                    return;
                }

                if(firstLoad) {
                    tempData.clear();
                    maxTemp = 0;
                    firstLoad = false;
                }

                for(var i in serverData) {
//                    console.log("XYPoint { x: new Date(\"" + serverData[i].Date + "\").getTime(); y: " + serverData[i].Temp + " }");
                    var temp = serverData[i].Temp;
                    var datetime = new Date(serverData[i].Date + "Z"); // Not storing TZ info in the DB so treat as GMT
                    maxTemp = Math.max(temp, maxTemp);
                    tempData.append(datetime, temp);
                }
                tempData.pointsVisible = true;
//                console.log("points loaded:" + tempData.count + " new points: " + i + " max temp: " + maxTemp);
//                console.log("x: " + new Date(tempData.at(0).x) + " y: " + tempData.at(0).y);
//                console.log("to x: " + new Date(tempData.at(tempData.count - 1).x) + " y: " + tempData.at(tempData.count - 1).y);
                tempData.axisX.rangeChanged(new Date(tempData.at(0).x), new Date(tempData.at(tempData.count - 1).x));
                tempData.axisY.rangeChanged(0, maxTemp + 10);
            }
        }

        if(firstLoad) {
            request.open("GET", "http://192.168.1.222/temperaturejson.php");
        } else {
//            console.log("fromDate: " + new Date(tempData.at(tempData.count - 1).x).toISOString());
            request.open("GET", "http://192.168.1.222/temperaturejson.php?fromDate=" + new Date(tempData.at(tempData.count - 1).x).toISOString());
        }

        request.send();
    }

    title: qsTr("Home")

    ChartView {
        id: graph
        title: "Mash Temperature"
        width: parent.width
        height: parent.height / 2
        backgroundColor: Material.background
        titleColor: Material.foreground

        ValueAxis {
            id: tempAxis
            min: 0
            color: Material.foreground
            labelsColor: Material.foreground
            gridLineColor: Material.foreground
            onRangeChanged: function(min, max) {
                tempAxis.min = min;
                tempAxis.max = max;
            }
        }

        DateTimeAxis {
            id: timeAxis
            min: new Date(tempData.at(0).x)
            format: "HH:mm:ss"
            color: Material.foreground
            labelsColor: Material.foreground
            gridLineColor: Material.foreground
            onRangeChanged: function(min, max) {
                timeAxis.min = min;
                timeAxis.max = max;
            }
        }

        legend.labelColor: Material.foreground
        legend.color : Material.foreground
        legend.borderColor: Material.foreground
        legend.visible: true

        LineSeries {
            id: tempData
            name: "Temp"
            axisY: tempAxis
            axisX: timeAxis
        }
    }

    Label {
        anchors.top: graph.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        text: "Current Temp: " + tempData.at(tempData.count - 1).y
    }

    // TODO: Polling with a timer now... Want to add a WebSockets option.
    Timer {
        interval: 10000
        repeat: true
        triggeredOnStart: true
        running: true
        onTriggered: fetchData()
    }


}
