import QtQuick 2.12
import QtQuick.Window 2.12
import QtMultimedia 5.12
import com.github.swex.QZXingNu 1.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

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
                formats: [QZXingNu.QR_CODE]
            }
        }

        VideoOutput {
            id: videoOutput
            fillMode: VideoOutput.PreserveAspectCrop
            Layout.fillHeight: true
            Layout.fillWidth: true
            filters: [filter]
            source: Camera {
                id: camera
                focus {
                    focusMode: CameraFocus.FocusContinuous
                    focusPointMode: CameraFocus.FocusPointAuto
                }
                deviceId: QtMultimedia.availableCameras[camerasComboBox.currentIndex] ? QtMultimedia.availableCameras[camerasComboBox.currentIndex].deviceId : ""
                onDeviceIdChanged: {
                    console.log("avaliable resolutions: " + supportedViewfinderResolutions(
                                    ))
                    focus.focusMode = CameraFocus.FocusContinuous
                    focus.focusPointMode = CameraFocus.FocusPointAuto
                }
                captureMode: Camera.CaptureViewfinder
                onCameraStateChanged: {

                }
                onLockStatusChanged: {
                    if (tagDiscoveredInSession) {
                        return
                    }
                    if (lockStatus === Camera.Locked) {
                        camera.unlock()
                    }
                }
                onError: console.log("camera error:" + errorString)
            }
            autoOrientation: true
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: false
            visible: QtMultimedia.availableCameras.length > 1
            Label {
                text: qsTr("Camera: ")
                Layout.fillWidth: false
            }
            ComboBox {
                id: camerasComboBox
                Layout.fillWidth: true
                model: QtMultimedia.availableCameras
                textRole: "displayName"
                currentIndex: 0
            }
        }
        Label {
            id: resultOutput
            text: "tags found: " + tagsFound + (lastTag ? " last tag: " + lastTag : "")
        }
    }
}
