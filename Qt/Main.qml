import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Effects
import QtMultimedia

import QtEsp32Demo

Window {
  width: 1920
  height: 1080
  visible: true
  title: qsTr("QtEsp32Demo")
  color: "#091613"
  visibility: Window.FullScreen

  MediaPlayer {
    id: parkingSound
    source: "sound.m4a"
    audioOutput: AudioOutput {}
  }

  MediaPlayer {
    id: parkingSoundSecond
    source: "sound_second.m4a"
    audioOutput: AudioOutput {}
  }

  MediaPlayer {
    id: parkingSoundThird
    source: "sound_third.m4a"
    audioOutput: AudioOutput {}
  }

  ColumnLayout {
    component ProximityRange: Rectangle {
      required property bool isInRange
      required property color changeColor
      Layout.minimumHeight: 64
      color: isInRange ? changeColor : "#09100f"
      radius: height / 2

      border {
        width: 2
        color: "#038661"
      }
    }

    anchors {
      bottom: carIcon.top
      horizontalCenter: parent.horizontalCenter
    }

    spacing: 16

    width: 512

    ProximityRange {
      Layout.minimumWidth: parent.width
      isInRange: BLEController.distance < 25 && BLEController.distance >= 10
      changeColor: "green"

      onIsInRangeChanged: {
        if(isInRange) {
          parkingSound.play()
        } else{
          parkingSound.stop()
        }
      }
    }

    ProximityRange {
      Layout.minimumWidth: parent.width
      isInRange: BLEController.distance < 10 && BLEController.distance >= 5
      changeColor: "yellow"

      onIsInRangeChanged: {
        if(isInRange) {
          parkingSoundSecond.play()
        } else{
          parkingSoundSecond.stop()
        }
      }
    }

    ProximityRange {
      Layout.minimumWidth: parent.width
      isInRange: BLEController.distance < 5 && BLEController.distance > 0
      changeColor: "red"

      onIsInRangeChanged: {
        if(isInRange) {
          parkingSoundThird.play()
        } else{
          parkingSoundThird.stop()
        }
      }
    }
  }

  Image {
    id: carIcon

    anchors.centerIn: parent

    source: "car.png"
    visible: false
    width: 512
    height: 512
  }

  MultiEffect {
    source: carIcon
    anchors.fill: carIcon
    colorization: 1
    brightness: 1
    colorizationColor: "#038661"
  }

  Text {
    anchors {
      top: carIcon.bottom
      horizontalCenter: parent.horizontalCenter
    }

    text: "Distance: " + BLEController.distance.toFixed(2) + " cm"
    font.pixelSize: 48
    color: "#038661"
  }

  Button {
    id: connectButton

    property bool deviceConnected: false

    anchors {
      bottom: parent.bottom
      bottomMargin: 32
      horizontalCenter: parent.horizontalCenter
    }

    width: 512
    height: 64
    padding: 0

    text: connectButton.deviceConnected ? "Device connected - ESP32_Scythe" : "Start BLE"

    background: Rectangle {
      color: connectButton.deviceConnected ? "#038661" : "#091613"
      border {
        width: 2
        color: connectButton.deviceConnected ? "#091613" : "#038661"
      }
      radius: height / 2
    }

    font.pixelSize: 32
    contentItem: Text {
      font.pixelSize: 32
      color: "white"
      text:connectButton.text
      verticalAlignment: Text.AlignVCenter
      horizontalAlignment: Text.AlignHCenter
    }

    onClicked: {
      BLEController.startBLE()
      connectButton.deviceConnected = !connectButton.deviceConnected
    }
  }
}
