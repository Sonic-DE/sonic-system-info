/*
    SPDX-FileCopyrightText: 2001 Matthias Hoelzer-Kluepfel <mhk@caldera.de>
    SPDX-FileCopyrightText: 2023 Ravi Mistry <rvstry@protonmail.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kcmusb.h"

#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>

#include "usbmodel.h"

K_PLUGIN_CLASS_WITH_JSON(USBViewer, "kcmusb.json")

USBViewer::USBViewer(QObject *parent, const KPluginMetaData &data, const QVariantList &args)
    : KQuickAddons::ConfigModule(parent, data, args)
{
    setQuickHelp(
        i18n("This module allows you to see"
             " the devices attached to your USB bus(es)."));

    KAboutData *about =
        new KAboutData(i18n("kcm_usb"), i18n("USB Devices"), QString(), QString(), KAboutLicense::GPL, i18n("(c) 2001 Matthias Hoelzer-Kluepfel"));
    about->addAuthor(i18n("Matthias Hoelzer-Kluepfel"), QString(), QStringLiteral("mhk@kde.org"));
    about->addCredit(i18n("Leo Savernik"), i18n("Live Monitoring of USB Bus"), QStringLiteral("l.savernik@aon.at"));
    about->addAuthor(i18n("Ravi Mistry"), QString(), QStringLiteral("rvstry@protonmail.com"));
    setAboutData(about);

    qmlRegisterType<USBModel>("org.kde.kinfocenter.usbview.private", 1, 0, "USBModel");
}

#include "kcmusb.moc"
