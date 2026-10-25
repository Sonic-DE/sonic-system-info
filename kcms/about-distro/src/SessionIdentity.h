/*
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/
#pragma once

#include <KLocalizedString>
#include <QByteArray>

namespace SessionIdentity {
enum class Desktop {
    Other,
    KDE,
    SonicDE
};

inline Desktop desktop(const QByteArray &current)
{
    const auto tokens = current.split(':');
    if (tokens.contains(QByteArrayLiteral("SonicDE"))) {
        return Desktop::SonicDE;
    }
    return tokens.contains(QByteArrayLiteral("KDE")) ? Desktop::KDE : Desktop::Other;
}

inline KLocalizedString desktopVersionLabel(Desktop session)
{
    switch (session) {
    case Desktop::SonicDE:
        return ki18n("SonicDE Version:");
    case Desktop::KDE:
        return ki18n("KDE Plasma Version:");
    default:
        return ki18n("Desktop Version:");
    }
}

inline KLocalizedString frameworksVersionLabel(Desktop session)
{
    switch (session) {
    case Desktop::SonicDE:
        return ki18n("SonicDE Frameworks Version:");
    case Desktop::KDE:
        return ki18n("KDE Frameworks Version:");
    default:
        return ki18n("Frameworks Version:");
    }
}
}
