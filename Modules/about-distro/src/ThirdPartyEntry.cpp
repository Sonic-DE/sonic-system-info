/*
    SPDX-FileCopyrightText: 2023 Jeremy Whiting <jeremy.whiting@collabora.com>
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "ThirdPartyEntry.h"

#include <QProcess>
#include <QtDebug>

QStringList getCommandOutput(const QString &command, const QString lang = QString())
{
    QProcess *process = new QProcess(nullptr);
    if (!lang.isEmpty()) {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("LANG", lang); // Add an environment variable
        process->setProcessEnvironment(env);
    }

    QString allOutput;

    QStringList arguments;
    process->start(command, arguments);
    process->waitForFinished();
    allOutput = process->readAllStandardOutput();

    return allOutput.split("\n");
}

ThirdPartyEntry::ThirdPartyEntry(const QString &scriptPath)
    : Entry(ki18nc("Unused but needs to be : to avoid assertion in Entry constructor", ":"), QString())
    , m_scriptPath(scriptPath)
{
    // Get default and value
    QStringList output = getCommandOutput(m_scriptPath);
    processOutput(output, Language::System);

    QStringList englishOutput = getCommandOutput(m_scriptPath, "en-US");
    processOutput(englishOutput, Language::English);
}

QString ThirdPartyEntry::localizedLabel(Language language) const
{
    return m_cachedLabels[language];
}

QString ThirdPartyEntry::localizedValue(Language language) const
{
    return m_value;
}

void ThirdPartyEntry::processOutput(const QStringList &output, Language language)
{
    for (const auto &line : output) {
        if (!line.isEmpty()) {
            const QStringList words = line.split("\t");
            if (words.size() < 2) {
                qDebug() << "Script missing 2 tab separated values, ignoring: " << line;
            } else {
                m_cachedLabels[language] = words.at(0);
                m_value = words.at(1);
            }
        }
    }
}
