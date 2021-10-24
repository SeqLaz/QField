import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Theme 1.0
import org.qfield 1.0

Item {
    Rectangle {
        color: "black"
        opacity: 0.8
        anchors.fill: parent
        MouseArea {
            anchors.fill: parent
            onClicked: {
                parent.parent.visible = false
            }
        }
    }

    ColumnLayout {
        id: aboutContainer
        anchors.fill: parent
        anchors.margins: 20

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            contentItem: information
            contentWidth: information.width
            contentHeight: information.height
            clip: true

            ColumnLayout {
                id: information
                width: aboutContainer.width
                height: Math.max(mainWindow.height - changelogButton.height * 2 - 60, childrenRect.height)

                ColumnLayout {
                    Layout.fillHeight: true
                    width: aboutContainer.width

                    MouseArea {
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: Math.min( 138, mainWindow.height / 4 )
                        Layout.preferredHeight: Math.min( 138, mainWindow.height / 4 )
                        Image {
                            id: qfieldLogo
                            width: parent.width
                            source: "qrc:/images/qfield_logo.svg"
                            sourceSize.width: width * screen.devicePixelRatio
                        }
                        onClicked: Qt.openUrlExternally("https://qfield.org/")
                    }

                    Label {
                        Layout.fillWidth: true
                        Layout.maximumWidth: parent.width
                        Layout.alignment: Qt.AlignCenter
                        horizontalAlignment: Text.AlignHCenter
                        font: Theme.strongFont
                        color: Theme.light
                        textFormat: Text.RichText
                        text: {
                            var links = '<a href="https://github.com/opengisch/QField/commit/' + gitRev + '">' + gitRev.substr(0, 6) + '</a>'

                            if (appVersion && appVersion !== '1.0.0')
                                links += ' <a href="https://github.com/opengisch/QField/releases/tag/' + appVersion + '">' + appVersion + '</a>'

                            return "<style>a, a:hover, a:visited { color:" + Theme.mainColor + "; }></style>QField<br>" + appVersionStr + " (" + links + ")"
                        }
                        onLinkActivated: Qt.openUrlExternally(link)
                    }
                }

                ColumnLayout {
                    Layout.fillHeight: true
                    width: aboutContainer.width

                    MouseArea {
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: Math.min( 91, mainWindow.height / 4 )
                        Layout.preferredHeight: Math.min( 138, mainWindow.height / 4 )
                        Image {
                            id: opengisLogo
                            width: parent.width
                            source: "qrc:/images/opengis-logo.svg"
                            sourceSize.width: width * screen.devicePixelRatio
                        }
                        onClicked: Qt.openUrlExternally("https://opengis.ch")
                    }

                    Label {
                        Layout.fillWidth: true
                        Layout.maximumWidth: parent.width
                        Layout.alignment: Qt.AlignCenter
                        horizontalAlignment: Text.AlignHCenter
                        font: Theme.strongFont
                        color: Theme.light
                        textFormat: Text.RichText
                        text: '<style>a, a:hover, a:visited { color:' + Theme.mainColor + '; }></style>' +qsTr( "Developed by" ) + '<br><a href="https://opengis.ch">OPENGIS.ch</a>'
                        onLinkActivated: Qt.openUrlExternally(link)
                    }
                }
            }
        }


        QfButton {
            id: sponsorshipButton
            Layout.fillWidth: true

            text: qsTr( 'Help sponsor QField development')
            onClicked: Qt.openUrlExternally("https://github.com/sponsors/opengisch")
        }

        QfButton {
            id: changelogButton
            Layout.fillWidth: true

            text: qsTr( 'Changelog' )

            onClicked: changelogPopup.open()
        }
    }
}
