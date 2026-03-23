/*
    SPDX-FileCopyrightText: 2021 Méven Car <meven.car@kdemail.net>
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "GraphicsPlatformEntry.h"
#include <QGuiApplication>
#include <X11/Xlib.h>

GraphicsPlatformEntry::GraphicsPlatformEntry()
    : Entry(ki18n("Graphics Platform:"), graphicsPlatform())
{
}

QString GraphicsPlatformEntry::graphicsPlatform()
{
    const QString platformName = QGuiApplication::platformName();
    if (platformName.startsWith(QLatin1String("xcb"), Qt::CaseInsensitive)) {
        if (auto *x11App = qGuiApp->nativeInterface<QNativeInterface::QX11Application>()) {
            Display *dpy = x11App->display();
            if (dpy) {
                QString vendor = QString::fromLatin1(ServerVendor(dpy));
                int release = VendorRelease(dpy);

                int major = release / 10000000;
                int minor = (release / 100000) % 100;
                int patch = (release / 1000) % 100;

                return QStringLiteral("X11 (%1) %2.%3.%4").arg(vendor).arg(major).arg(minor).arg(patch);
            } else {
                return QStringLiteral("X11");
            }
        } else {
            return QStringLiteral("X11");
        }
    }
    if (platformName.startsWith(QLatin1String("wayland"), Qt::CaseInsensitive)) {
        return QStringLiteral("Wayland");
    }
    return platformName;
}
