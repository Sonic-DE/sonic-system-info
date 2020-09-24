/**
 * SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.15 as QQC2

import org.kde.kirigami 2.7 as Kirigami
import org.kde.kcm 1.2
import org.kde.kinfocenter.nic.private 1.0

ScrollViewKCM {
    ConfigModule.quickHelp: i18n("Network Information")
    clip: true
    
    view: TableView {
        id: tableview
        clip: true
        
        columnWidthProvider: function (column) { 
            return tableview.model ? tableview.width / tableview.model.columnCount() : 0
        }

        model: NetworkModel {
            id: model
        }

        delegate: Rectangle {
            readonly property real cellPadding: Kirigami.Units.largeSpacing * 2
            color: row % 2 === 0 ? "transparent" : Kirigami.Theme.alternateBackgroundColor
            
            implicitWidth: tableview.width / tableview.model.columnCount()
            implicitHeight: text.contentHeight + Kirigami.Units.largeSpacing
            
            QQC2.Label {
                id: text
                text: display
                x: Kirigami.Units.smallSpacing
                width: parent.width - Kirigami.Units.largeSpacing
                height: parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WrapAnywhere
            }
        }
        
        topMargin: headerView.implicitHeight
        
        QQC2.HorizontalHeaderView {
            id: headerView
            anchors.bottom: parent.top
            syncView: tableview
        }
    }
    
    footer: QQC2.Button {
        icon.name: "view-refresh"
        text: i18nc("@action:button", "Refresh")
        onClicked: model.update();
    }
}

