// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2020 Harald Sitter <sitter@kde.org>

import org.kde.kcm 1.4 as KCM
import QtQuick 2.14
import org.kde.kirigami 2.12 as Kirigami
import QtQuick.Controls 2.14 as QQC2
import QtQuick.Layouts 1.14
import org.kde.kinfocenter.samba 1.0 as Samba

// This is a slightly bespoke variant of a BasicListItem. It features equally sized labels for the share information.
Kirigami.AbstractListItem {
    id: listItem

    contentItem: RowLayout {
        spacing: LayoutMirroring.enabled ? listItem.rightPadding : listItem.leftPadding

        ColumnLayout {
            spacing: 0
            Layout.fillWidth: true

            QQC2.Label {
                Layout.fillWidth: true
                elide: Text.ElideMiddle
                font: Kirigami.Theme.smallFont
                text: xi18nc("@info", 'Path: <a href="file://%1">%1</a>', ROLE_Path)
                onLinkActivated: Qt.openUrlExternally(link)
            }

            QQC2.Label {
                Kirigami.FormData.label: i18nc("@label", "Shared at:")
                Layout.fillWidth: true
                elide: Text.ElideMiddle
                font: Kirigami.Theme.smallFont
                text: ROLE_ShareUrl !== undefined ?
                      xi18nc("@info %1 is a URL", 'Shared at: <a href="%1">%1</a>', ROLE_ShareUrl) :
                      xi18nc("@info %1 is a dir", 'Shared at: /%1', ROLE_Name)
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }

        QQC2.ToolButton {
            action: Kirigami.Action {
                iconName: "document-properties"
                tooltip: xi18nc("@info:tooltip", "Open folder properties to change share settings")
                displayHint: Kirigami.Action.DisplayHint.IconOnly
                onTriggered: view.model.showPropertiesDialog(model.row)
            }

            QQC2.ToolTip {
                text: parent.action.tooltip
            }
        }
    }
}
