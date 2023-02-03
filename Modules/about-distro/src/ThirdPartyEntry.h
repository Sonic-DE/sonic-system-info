/*
    SPDX-FileCopyrightText: 2023 Jeremy Whiting <jeremy.whiting@collabora.com>
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#pragma once

#include "Entry.h"

#include <QProcess>

class ThirdPartyEntry : public Entry
{
    Q_OBJECT
public:
    explicit ThirdPartyEntry(const QString &scriptPath);
    QString localizedLabel(Language language = Language::System) const override;
    QString localizedValue(Language language = Language::System) const override;

private slots:
    void processFinished();

private:
    void runScript(const QString &command, const QString &lang = QString(), Language language = Language::System);

    void processOutput(const QStringList &output, Language language);
    const QString m_scriptPath;
    // Runs the above script with LANGUAGE set and caches results
    QMap<Entry::Language, QString> m_labels;
    // Same for values
    QMap<Entry::Language, QString> m_values;
};
