/*
    SPDX-FileCopyrightText: 2022 Jeremy Whiting <jeremy.whiting@collabora.com>
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "OSVersionEntry.h"

#include <KConfigGroup>
#include <KOSRelease>
#include <KSharedConfig>

OSVersionEntry::OSVersionEntry()
    : Entry(ki18n("Operating System:"), QString())
{
    // NOTE: do not include globals, otherwise kdeglobals could provide values
    //       even though we only explicitly want them from our own config.
    KSharedConfig::Ptr config = KSharedConfig::openConfig(QStringLiteral("kcm-about-distrorc"), KConfig::NoGlobals);
    KConfigGroup cg = KConfigGroup(config, "General");

    KOSRelease os;

    QString logoPath = cg.readEntry("LogoPath", os.logo());
    if (logoPath.isEmpty()) {
        logoPath = QStringLiteral("start-here-kde");
    }
    m_logoPath = logoPath;

    // We allow overriding of the OS name for branding purposes.
    // For example OS Ubuntu may be rebranded as Kubuntu. Also Kubuntu Active
    // as a product brand is different from Kubuntu.
    const QString distroName = cg.readEntry("Name", os.name());
    const QString osrVersion = cg.readEntry("UseOSReleaseVersion", false) ? os.version() : os.versionId();
    const QString versionId = cg.readEntry("Version", osrVersion);

    // Default to not show Build
    bool showBuild = cg.readEntry("ShowBuild", false);

    // This creates a trailing space if versionId is empty, so trimming String
    // to remove possibly trailing spaces
    m_nameVersion =
        (showBuild ? ki18nc("@label %1 is the distro name, %2 is the version, %3 is the 'build' which should be a number, or 'rolling'", "%1 %2 Build: %3")
                         .subs(distroName)
                         .subs(versionId)
                         .subs(os.buildId())
                   : ki18nc("@label %1 is the distro name, %2 is the version", "%1 %2").subs(distroName).subs(versionId));

    const QString variant = cg.readEntry("Variant", os.variant());
    m_variant = variant;

    const QString url = cg.readEntry("Website", os.homeUrl());
    m_url = url;
}

QString OSVersionEntry::localizedValue(Language language) const
{
    return localize(m_nameVersion, language);
}

QString OSVersionEntry::logoPath() const
{
    return m_logoPath;
}

QString OSVersionEntry::url() const
{
    return m_url;
}

QString OSVersionEntry::variant() const
{
    return m_variant;
}
