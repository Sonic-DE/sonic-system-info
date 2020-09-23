/**
 * SPDX-FileCopyrightText: 2001 Alexander Neundorf <neundorf@kde.org>
 * SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
 *
 * SPDX-LicenseIndentifier: GPL-2.0-or-later
 */

#include "nic.h"

#include <KPluginFactory>
#include <KAboutData>
#include <KLocalizedString>

#include "networkmodel.h"

K_PLUGIN_CLASS_WITH_JSON(KCMNic, "nic.json")

KCMNic::KCMNic(QObject *parent, const QVariantList &list)
    : KQuickAddons::ConfigModule(parent, list)
    , m_networkModel(std::make_unique<NetworkModel>())
{
    KAboutData *about = new KAboutData(i18n("kcminfo"),
            i18n("System Information Control Module"),
            QString(), QString(), KAboutLicense::GPL,
            i18n("(c) 2001 - 2002 Alexander Neundorf"));

    about->addAuthor(i18n("Alexander Neundorf"), QString(), QStringLiteral("neundorf@kde.org"));
    setAboutData(about);
}

void KCMNic::update()
{
}

NetworkModel *KCMNic::networkModel() const
{
    return m_networkModel.get();
}


#include "nic.moc"
