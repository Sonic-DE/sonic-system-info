/*
    SPDX-FileCopyrightText: 2023 Jeremy Whiting <jeremy.whiting@collabora.com>
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#pragma once

#include "Entry.h"

class ThirdPartyEntry : public Entry
{
public:
    ThirdPartyEntry(const QString &scriptPath);
    QString localizedLabel(Language language = Language::System) const override;
    QString localizedValue(Language language = Language::System) const override;

private:
    void processOutput(const QStringList &output, Language language);
    QString m_scriptPath;
    // Runs the above script with LANG set and caches results
    QMap<Entry::Language, QString> m_cachedLabels;
    // Values are the same for all languages, so just cache the value
    QString m_value;
};
