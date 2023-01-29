/*
    SPDX-FileCopyrightText: 2023 Ravi Mistry <rvstry@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15

import org.kde.kcm 1.6
import org.kde.kirigami 2.20 as Kirigami
import org.kde.kinfocenter.usbview.private 1.0

SimpleKCM {

    title: "[Insert Product Name Here]"
    property var detailList

    Kirigami.FormLayout {

        Component {
            id: detailDelegate

            RowLayout {
                Kirigami.FormData.label: "Detail:"
                Kirigami.SelectableLabel {
                    text: modelData
                }
            }
        }

        Repeater {
            model: detailList
            delegate: detailDelegate
        }
    }
}
