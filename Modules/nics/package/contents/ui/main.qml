/**
 * SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2

import org.kde.kirigami 2.7 as Kirigami
import org.kde.kcm 1.2
import org.kde.kinfocenter.nic.private 1.0

ScrollViewKCM {
    ConfigModule.quickHelp: i18n("Network Information")
    clip: true
    
    TableView {
        id: tableview
        anchors.fill: parent
        contentWidth: parent.width
        clip: true

        model: NetworkModel {}

        delegate: Rectangle {
            implicitWidth: tableview.width / 6
            implicitHeight: 50
            border.width: 1
            Text {
                text: display
                anchors.centerIn: parent
                anchors.margins: Kirigami.Units.largeSpacing
            }
        }
    }
}

