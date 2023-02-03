/*
    SPDX-FileCopyrightText: 2023 Jeremy Whiting <jeremy.whiting@collabora.com>
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "ThirdPartyEntry.h"

#include <QDebug>
#include <QProcess>

QStringList getCommandOutput(const QString &command, const QString &lang = QString())
{
    auto process = new QProcess(nullptr);
    if (!lang.isEmpty()) {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("LANGUAGE", lang); // Add an environment variable
        process->setProcessEnvironment(env);
    }

    process->start(command, QStringList());
    process->waitForFinished();
    return QString(process->readAllStandardOutput()).split('\n');
}

ThirdPartyEntry::ThirdPartyEntry(const QString &scriptPath)
    : Entry(ki18nc("Unused but needs to be : to avoid assertion in Entry constructor", ":"), QString())
    , m_scriptPath(scriptPath)
{
    // Get default and value
    QStringList output = getCommandOutput(m_scriptPath);
    processOutput(output, Language::System);

    QStringList englishOutput = getCommandOutput(m_scriptPath, "en_US:C");
    processOutput(englishOutput, Language::English);
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
    Q_ASSERT(output.size() >= 2);
    m_labels[language] = output.at(0);
    m_values[language] = output.at(1);
}
