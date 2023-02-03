/*
    SPDX-FileCopyrightText: 2023 Jeremy Whiting <jeremy.whiting@collabora.com>
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "ThirdPartyEntry.h"

#include <QDebug>

#define kLanguageProperty "language"

ThirdPartyEntry::ThirdPartyEntry(const QString &scriptPath)
    : Entry(ki18nc("Unused but needs to be : to avoid assertion in Entry constructor", ":"), QString())
    , m_scriptPath(scriptPath)
{
    // Get default and value
    runScript(m_scriptPath);
    runScript(m_scriptPath, "en_US:C", Language::English);
}

QString ThirdPartyEntry::localizedLabel(Language language) const
{
    return m_labels[language];
}

QString ThirdPartyEntry::localizedValue(Language language) const
{
    return m_values[language];
}

void ThirdPartyEntry::processFinished()
{
    auto process = qobject_cast<QProcess *>(sender());
    Language language = process->property(kLanguageProperty).value<Language>();

    processOutput(QString(process->readAllStandardOutput()).split('\n'), language);
}

void ThirdPartyEntry::processOutput(const QStringList &output, Language language)
{
    for (const auto &line : output) {
        if (!line.isEmpty()) {
            const QStringList words = line.split(QLatin1Char('\t'));
            if (words.size() < 2) {
                qWarning() << "Script: " << m_scriptPath << " missing 2 tab separated values, ignoring: " << line;
            } else {
                m_labels[language] = words.at(0);
                m_values[language] = words.at(1);
            }
        }
    }
}

void ThirdPartyEntry::runScript(const QString &command, const QString &lang, Language language)
{
    auto process = new QProcess(nullptr);
    if (!lang.isEmpty()) {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("LANGUAGE", lang); // Add an environment variable
        process->setProcessEnvironment(env);
    }

    process->setProperty(kLanguageProperty, QVariant::fromValue(language));
    bool worked = connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished()));
    qDebug() << "Connect result: " << worked;
    process->start(command, QStringList());
}
