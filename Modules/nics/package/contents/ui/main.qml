/**
 * SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
 * SPDX-FileCopyrightText: 2021 Harald Sitter <sitter@kde.org>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Layouts 1.2

import org.kde.kirigami 2.14 as Kirigami
import org.kde.kcm 1.3
import org.kde.kinfocenter.nic.private 1.0

SimpleKCM {
    ConfigModule.quickHelp: i18n("Network Interfaces")
    clip: true

    Kirigami.CardsLayout {
        Rectangle {
            implicitHeight: 128
            Layout.fillWidth: true
            color: "red"
        }
        Rectangle {
            implicitHeight: 128
            Layout.fillWidth: true
            color: "green"
        }
        Repeater {
            id: repeater
            model: NetworkModel {}
            Kirigami.AbstractCard {
                Layout.fillHeight: true
                Layout.fillWidth: true
                contentItem: GridLayout {
                    columns: 2
                    Layout.fillWidth: true
                    QQC2.Label {
                        text: i18nc("@label", "Name:")
                    }
                    QQC2.Label {
                        text: model.name
                    }

                    QQC2.Label {
                        text: i18nc("@label", "Address:")
                    }
                    QQC2.Label {
                        text: model.address
                    }

                    QQC2.Label {
                        text: i18nc("@label", "Network Mask:")
                    }
                    QQC2.Label {
                        text: model.netmask
                    }

                    QQC2.Label {
                        text: i18nc("@label", "Type:")
                    }
                    QQC2.Label {
                        text: model.type
                    }

                    QQC2.Label {
                        text:  i18nc("@label", "Hardware Address:")
                        visible: hardwareAddress.visible
                    }
                    QQC2.Label {
                        id: hardwareAddress
                        text: model.hardwareAddress
                        visible: text.length > 0
                    }


                    QQC2.Label {
                        text: i18nc("@label", "State:")
                    }
                    RowLayout {
                        Rectangle {
                            implicitWidth: Kirigami.Units.largeSpacing
                            implicitHeight: implicitWidth
                            radius: width / 2
                            color: model.state ? Kirigami.Theme.positiveTextColor : Kirigami.Theme.negativeBackgroundColor
                        }
                        QQC2.Label {
                            text: model.state ? i18nc("State of network card is connected", "Up")
                                              : i18nc("State of network card is disconnected", "Down")
                        }
                    }
                }
            }
        }
    }

    footer: QQC2.Button {
        icon.name: "view-refresh"
        text: i18nc("@action:button", "Refresh")
        onClicked: model.update();
    }
}

