/*
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
    SPDX-FileCopyrightText: 2021 Harald Sitter <sitter@kde.org>
*/

import QtQuick 2.5
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Layouts 1.1

import org.kde.kirigami 2.12 as Kirigami
import org.kde.kquickcontrolsaddons 2.0
import org.kde.kcm 1.4 as KCM

// SimpleKCM is illsuited because we need a more capable scrollview and also want more control over where items are.
KCM.AbstractKCM {
    id: root

    implicitWidth: Kirigami.Units.gridUnit * 20
    implicitHeight: Kirigami.Units.gridUnit * 20

    // The CommandOutputContext object.
    required property QtObject output
    property int wrapMode: TextEdit.NoWrap

    Component {
        id: dataComponent

        ColumnLayout {
            QQC2.ScrollView {
                id: view
                Layout.fillHeight: true
                Layout.fillWidth: true

                QQC2.TextArea {
                    id: textArea
                    readOnly: true
                    text: output.text
                    font.family: "monospace"
                    textFormat: TextEdit.PlainText
                    wrapMode: root.wrapMode
                }
            }

            Kirigami.SearchField {
                id: filterField

                Layout.fillWidth: true
                Accessible.name: i18nc("accessible name for filter input", "Filter")
                Accessible.searchEdit: true

                placeholderText: i18nc("@label placeholder text to filter for something", "Filter…")
                focusSequence: "Ctrl+I"
                onAccepted: output.filter = text
            }

            Kirigami.SearchField {
                id: searchField

                property string lastText: ''
                property int lastAreaIndex: -1
                property var needleAreas: []

                function hasCapitalLetter(str) {
                    return /[A-Z]/.test(str)
                }

                function updateNeedleAreas() {
                    if (lastText !== text) { // reset state
                        lastText = ''
                        lastAreaIndex = -1
                        needleAreas = []
                        // If the text changes to something that no longer matches we'll not want to keep the old highlight.
                        textArea.deselect()
                    }
                    lastText = text

                    if (text === "")  { // no search term
                        return
                    }
                    if (needleAreas.length > 0) { // already filled and hasn't been reset
                        return
                    }

                    // TODO: maybe have explicit insensitive toggle?
                    const caseSensitive = hasCapitalLetter(text)
                    const haystack = caseSensitive ? textArea.text : textArea.text.toUpperCase()
                    const needle = caseSensitive ? text : text.toUpperCase()

                    let offset = 0
                    while (offset < haystack.length) {
                        const start = haystack.indexOf(needle, offset)
                        if (start < 0) { // not found
                            break
                        }
                        const end = start + text.length
                        needleAreas.push([start, end])
                        offset = end + 1
                    }

                    if (needleAreas.length <= 0) {
                        console.warn("Could not find '%1' in text 😥".arg(needle))
                    }
                }

                function next() {
                    if (needleAreas.length <= 0) {
                        return
                    }

                    const areaIndex = (lastAreaIndex + 1) % needleAreas.length
                    const area = needleAreas[areaIndex]
                    textArea.select(...area)
                    lastAreaIndex = areaIndex
                }

                Layout.fillWidth: true
                onAccepted: {
                    updateNeedleAreas()
                    next()
                }

                Shortcut {
                    enabled: root.focus
                    sequence: StandardKey.FindNext
                    onActivated: searchField.next()
                }

                // TODO could have a label indicating 3/24 match counter. Not sure if useful?
            }
        }
    }

    Component {
        id: loadingComponent
        Item {
            QQC2.BusyIndicator {
                anchors.centerIn: parent
                running: true
        }
    }

    Component {
        id: noDataComponent

        Item {
            Kirigami.PlaceholderMessage {
                anchors.centerIn: parent
                width: parent.width - (Kirigami.Units.largeSpacing * 4)
                text: output.error
                icon.name: "data-warning"
            }
        }
    }

    // This is a bit flimsy but we want to switch the content of the KCM around, based on the data state.
    // We could switch around visiblity but a Loader seems neater over all.
    Loader {
        id: contentLoader
        // always fill with the loaded content to ensure it covers the entire area, but not more. Inside the
        // root item we can then lay out as necessary for the given state item.
        anchors.fill: parent
    }

    states: [
        State {
            name: "loading"
            when: !output.ready
            PropertyChanges { target: contentLoader; sourceComponent: loadingComponent }
        },
        State {
            name: "noData"
            when: output.text === "" || output.error !== ""
            PropertyChanges { target: contentLoader; sourceComponent: noDataComponent }
        },
        State {
            name: "" // default state
            PropertyChanges { target: contentLoader; sourceComponent: dataComponent }
        }
    ]
}
