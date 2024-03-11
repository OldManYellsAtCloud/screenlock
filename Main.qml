import QtQuick
import QtQuick.Controls

import sgy.pine.screenlock

Window {
    id: root
    visible: true
    flags: Qt.WA_TranslucentBackground |  Qt.ToolTip | Qt.FramelessWindowHint
    color: "transparent"
    property double globalOpacity: 1.0

    function hideAndStopTimer(){
        visibleAnimation.start()
        timeUpdater.stop()
        idleTimer.stop()
    }

    function showAndStartTimer(){
        root.visible = true
        globalOpacity = 1.0
        timeUpdater.start()
    }

    Component.onCompleted: idleTimer.start()

    NumberAnimation {
        id: visibleAnimation
        target: root
        properties: "globalOpacity"
        to: 0.0
        duration: 200
        onFinished: {
            root.visible = false
        }
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

        onScreenStateChanged: function(screenIsOff){
            if (screenIsOff){
                hideAndStopTimer();
            } else {
                showAndStartTimer();
                idleTimer.restart();
            }
        }
    }

    Text {
        id: dateText
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
        font.pixelSize: 36
        topPadding: parent.height * 0.15
        opacity: globalOpacity
    }

    Text {
        id: timeText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: dateText.bottom
        color: "white"
        font.pixelSize: 48
        opacity: globalOpacity
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
            dateText.text = d.getFullYear() + "-"  + padWith0(d.getMonth() + 1) + "-" + padWith0(d.getDate())
            timeText.text = padWith0(d.getHours()) + ":" + padWith0(d.getMinutes())
        }
    }

    Timer {
        id: idleTimer
        interval: 1000 * 20 // 20sec
        running: false
        repeat: false
        onTriggered: {
            dbusManager.idleTimeout()
        }
    }

    Slider {
        id: unlockSlider
        value: 0
        from: 0
        to: 100
        live: false
        opacity: globalOpacity

        y: parent.height * 0.90
        x: parent.width * 0.05
        width: parent.width * 0.9

        NumberAnimation {
            id: sliderResetAnimation
            target: unlockSlider
            properties: "value"
            to: 0
            duration: 100
        }

        onPressedChanged: {
            if (!pressed) {
                if (value > 80) {
                    idleTimer.stop()
                    dbusManager.screenUnlocked()
                    hideAndStopTimer()
                }
                sliderResetAnimation.start()
            }
        }

        // If it is not here, sometimes (often) the slider gets
        // "stuck" after release, instead of firing the corresponding
        // action - apparently value can change without pressing also.
        onValueChanged: {
            onPressedChanged()
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
