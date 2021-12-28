import QtQuick
import QtQuick.Window
import QtMultimedia
import com.github.swex.QZXingNu
import QtQuick.Controls
import QtQuick.Layouts

Window {
  visible: true
  width: 640
  height: 480
  title: Qt.application.name
  property int tagsFound: 0
  property string lastTag: ""
  ColumnLayout {
    anchors.fill: parent
    QZXingNuFilter {
      id: filter
      onTagFound: {
        tagsFound++
        lastTag = tag
      }
      qzxingNu: QZXingNu {
        id: zxingItself
        formats: [QZXingNu.QR_CODE]
        onDecodeResultChanged: console.log(decodeResult)
      }
      videoSink: videoOutput.videoSink
    }

    MediaDevices {
      id: mediaDevices
    }
    CaptureSession {
      camera: Camera {
        id: camera
        focusMode: Camera.FocusModeAutoNear
        cameraDevice: mediaDevices.defaultVideoInput
      }
      videoOutput: videoOutput
    }

    VideoOutput {
      id: videoOutput
      fillMode: VideoOutput.PreserveAspectCrop
      Layout.fillHeight: true
      Layout.fillWidth: true
    }

    RowLayout {
      Layout.fillWidth: true
      Layout.fillHeight: false
      visible: mediaDevices.videoInputs.length > 1
      Label {
        text: qsTr("Camera: ")
        Layout.fillWidth: false
      }
      ComboBox {
        id: camerasComboBox
        Layout.fillWidth: true
        model: mediaDevices.videoInputs
        textRole: "displayName"
        currentIndex: 0
      }
    }
    Label {
      id: resultOutput
      text: "tags found: " + tagsFound + (lastTag ? " last tag: " + lastTag : "")
    }
  }
  Timer {
    repeat: true
    running: true
    interval: 1000
    onTriggered: {

    }
  }
  Component.onCompleted: {
    console.log("completed!")
  }
}
