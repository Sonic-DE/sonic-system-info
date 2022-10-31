/*
    SPDX-FileCopyrightText: 2022 Jeremy Whiting <jeremy.whiting@collabora.com>
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#ifndef OSVERSIONENTRY_H
#define OSVERSIONENTRY_H

#include "Entry.h"

class OSVersionEntry : public Entry
{
public:
    OSVersionEntry();
    QString localizedValue(Language language = Language::System) const override;
    QString logoPath() const;
    QString url() const;
    QString variant() const;

private:
    KLocalizedString m_nameVersion;
    QString m_logoPath;
    QString m_url;
    QString m_buildId;
    QString m_variant;
};

#endif // OSVERSIONENTRY_H
