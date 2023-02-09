/*
    SPDX-FileCopyrightText: 2023 Ravi Mistry <rvstry@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15

import org.kde.kcm 1.6
import org.kde.kirigami 2.20 as Kirigami
import org.kde.kirigamiaddons.treeview 1.0
import org.kde.kinfocenter.usbview.private 1.0

ScrollViewKCM {

    // Turns this page into a side panel
    framedView: true
    Kirigami.ColumnView.fillWidth: false

    implicitHeight: Kirigami.Units.gridUnit * 20
    implicitWidth: Kirigami.Units.gridUnit * 20

    // Replicates the old Widgets UI
    Component.onCompleted: kcm.push("DetailsPage.qml")

    view: TreeListView { // TODO: Replace with TreeView in Qt6

        sourceModel: USBModel {
            id: usbModel
        }

        expandsByDefault: true

        delegate: BasicTreeItem {
            contentItem: Controls.Label {
                    text: model.display
                    }

                property int busdev
                onClicked: {
                    busdev = usbModel.find(model.display)
                    kcm.push("DetailsPage.qml", {detailList: usbModel.details, product: usbModel.product})
            }
        }
    }
}
