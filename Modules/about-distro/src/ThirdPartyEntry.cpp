/*
    SPDX-FileCopyrightText: 2023 Jeremy Whiting <jeremy.whiting@collabora.com>
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "ThirdPartyEntry.h"

#include <QDebug>
#include <QProcess>

ThirdPartyEntry::ThirdPartyEntry(const QString &scriptPath)
    : Entry(ki18nc("Unused but needs to be : to avoid assertion in Entry constructor", ":"), QString())
{
    for (const auto &language : {Entry::Language::System, Entry::Language::English}) {
        QProcess process;

        switch (language) {
        case Entry::Language::English: {
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            env.insert("LANGUAGE", "en_US:C"); // Add an environment variable
            process.setProcessEnvironment(env);
            break;
        }
        case Entry::Language::System:
            break;
        }

        process.start(scriptPath, QStringList());
        process.waitForFinished();
        processOutput(QString::fromUtf8(process.readAllStandardOutput()).split('\n'), language);
    }
}

QString ThirdPartyEntry::localizedLabel(Language language) const
{
    return m_labels[language];
}

QString ThirdPartyEntry::localizedValue(Language language) const
{
    return m_values[language];
}

void ThirdPartyEntry::processOutput(const QStringList &output, Language language)
{
    for (const auto &line : output) {
        if (line.isEmpty()) {
            continue;
        }

        const QStringList words = line.split(QLatin1Char('\t'));
        if (words.size() < 2) {
            qWarning() << "Script: " << m_scriptPath << " missing 2 tab separated values, ignoring: " << line;
        } else {
            m_labels[language] = words.at(0);
            m_values[language] = words.at(1);
        }
    }
}
