import QtQuick
import QtQuick.Controls

import sgy.pine.screenlock

Window {
    id: root
    visible: true
    flags: Qt.WA_TranslucentBackground |  Qt.ToolTip | Qt.FramelessWindowHint
    color: "transparent"

    function hideAndStopTimer(){
        root.visible = false
        timeUpdater.stop()
    }

    function showAndStartTimer(){
        root.visible = true
        timeUpdater.start()
    }

    DbusManager {
        id: dbusManager
        onLockStateChanged: function(state){
            if (!state){
                hideAndStopTimer()
            } else {
                dbusManager.screenLocked()
                showAndStartTimer()
            }
        }
    }

    Text {
        id: dateText
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
        font.pixelSize: 36
        topPadding: parent.height * 0.15
    }

    Text {
        id: timeText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: dateText.bottom
        color: "white"
        font.pixelSize: 48
    }

    Timer {
        id: timeUpdater
        interval: 1000
        running: root.visible
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            // left pad with 0s, up to 2 chars
            function padWith0(str){
                return String(str).padStart(2, '0')
            }

            var d = new Date()
            dateText.text = d.getFullYear() + "-"  + padWith0(d.getMonth()) + "-" + padWith0(d.getDate())
            timeText.text = padWith0(d.getHours()) + ":" + padWith0(d.getMinutes())
        }
    }

    Slider {
        id: unlockSlider
        value: 0
        from: 0
        to: 100

        y: parent.height * 0.90
        x: parent.width * 0.05
        width: parent.width * 0.9

        onPressedChanged: {
            if (!pressed) {
                if (value > 80) {
                    dbusManager.screenUnlocked()
                    hideAndStopTimer()
                }
                value = 0
            }
        }

        handle: Rectangle {
            x: unlockSlider.leftPadding + unlockSlider.visualPosition * (unlockSlider.availableWidth - width)
            y: unlockSlider.topPadding + unlockSlider.availableHeight / 2 - height / 2
            implicitWidth: 40
            implicitHeight: 40
            radius: 20
            color: unlockSlider.pressed ? "#f0f0f0" : "#f6f6f6"
            border.color: "#bdbebf"
        }


    }
}
