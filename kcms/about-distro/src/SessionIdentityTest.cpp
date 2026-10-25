/*
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/
#include "SessionIdentity.h"

#include <QCoreApplication>
#include <cstdio>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    KLocalizedString::setLanguages({QStringLiteral("en_US")});
    struct Case {
        const char *environment;
        SessionIdentity::Desktop expected;
        const char *desktopLabel;
        const char *frameworksLabel;
    };
    const Case cases[] = {
        {"SonicDE:KDE", SessionIdentity::Desktop::SonicDE, "SonicDE Version:", "SonicDE Frameworks Version:"},
        {"KDE:SonicDE", SessionIdentity::Desktop::SonicDE, "SonicDE Version:", "SonicDE Frameworks Version:"},
        {"SonicDE", SessionIdentity::Desktop::SonicDE, "SonicDE Version:", "SonicDE Frameworks Version:"},
        {"KDE", SessionIdentity::Desktop::KDE, "KDE Plasma Version:", "KDE Frameworks Version:"},
        {"GNOME:KDE", SessionIdentity::Desktop::KDE, "KDE Plasma Version:", "KDE Frameworks Version:"},
        {"NotSonicDE", SessionIdentity::Desktop::Other, "Desktop Version:", "Frameworks Version:"},
        {"", SessionIdentity::Desktop::Other, "Desktop Version:", "Frameworks Version:"},
    };
    for (const auto &test : cases) {
        const auto session = SessionIdentity::desktop(test.environment);
        if (session != test.expected || SessionIdentity::desktopVersionLabel(session).toString() != QString::fromLatin1(test.desktopLabel)
            || SessionIdentity::frameworksVersionLabel(session).toString() != QString::fromLatin1(test.frameworksLabel)) {
            std::fprintf(stderr, "Unexpected session labels for %s\n", test.environment);
            return 1;
        }
    }
    return 0;
}
