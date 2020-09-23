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
    
    header:  Row {
        id: rowsHeader
        x: tableview.contentX
        z: 2
        Repeater {
            model: tableview.columns > 0 ? tableview.columns : 1
            
            QQC2.Label {
                width: tableview.columnWidthProvider(modelData)
                text: tableview.model ? tableview.model.headerData(modelData, Qt.Horizontal): 0
                font.pixelSize: 15
                padding: 10
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
    
    TableView {
        id: tableview
        anchors.fill: parent
        contentWidth: parent.width
        clip: true
        
        columnWidthProvider: function (column) { 
            return tableview.model ? tableview.width /tableview.model.columnCount() : 0
        }

        model: NetworkModel {}

        delegate: Rectangle {
            border.width: 1
            QQC2.Label {
                text: display
                anchors.centerIn: parent
                padding: Kirigami.Units.largeSpacing
            }
        }
    }
}

