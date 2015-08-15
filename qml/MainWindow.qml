import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.1

ApplicationWindow {
    id: window
    visible: true

    function showMessage() {
        if(mainWindow.textError) {
            messageDialog.title = qsTr("Erro")
            messageDialog.text = mainWindow.textError;
            messageDialog.icon = StandardIcon.Critical;
            messageDialog.open();
            mainWindow.textError = "";
        } else if(mainWindow.textWarning) {
            messageDialog.title = qsTr("Aviso")
            messageDialog.text = mainWindow.textWarning;
            messageDialog.icon = StandardIcon.Warning;
            messageDialog.open();
            mainWindow.textWarning = "";
        }
    }

    MessageDialog {
        id: messageDialog
        standardButtons: StandardButton.Ok
        onAccepted: {
            close();
        }
    }

    TextArea {
        id: txtData
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: btnSalvar.top
        }
        inputMethodHints: Qt.ImhNoPredictiveText
        text: mainWindow.textDecript;
    }

    Button {
        id: btnSalvar
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        text: qsTr("Salvar")
        onClicked: {
            mainWindow.textDecript = txtData.text;
            if(mainWindow.saveFile())
            {
                messageDialog.title = qsTr("Informação")
                messageDialog.text = qsTr("Os dados foram salvos com sucesso!")
                messageDialog.icon = StandardIcon.Information;
                messageDialog.open();
            } else {
                window.showMessage();
            }
        }

    }

    Loader {
        id: lodMain
        anchors.fill: parent
    }

    Component {
        id: dlgLogin

        Rectangle {
            anchors.fill: parent
            color: "white"
            Column {
                anchors.centerIn: parent
                spacing: 20

                TextField {
                    id: txtInput
                    anchors.horizontalCenter: parent.horizontalCenter
                    inputMethodHints: Qt.ImhNoPredictiveText
                    echoMode: TextInput.Password
                    placeholderText: qsTr("Senha")
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 10

                    Button {
                        id: btnOk
                        text: qsTr("Ok")
                        onClicked: {
                            if(mainWindow.login(txtInput.text)) {
                                window.showMessage();
                                lodMain.sourceComponent = null;
                            } else {
                                window.showMessage();
                            }

                        }
                    }
                    Button {
                        id: btnCancel
                        text: qsTr("Cancelar")
                        onClicked: Qt.quit();
                    }

                }
            }
        }
    }

    Component.onCompleted: lodMain.sourceComponent = dlgLogin;
}

