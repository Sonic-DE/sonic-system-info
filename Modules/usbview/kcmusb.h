/*
    SPDX-FileCopyrightText: 2001 Matthias Hoelzer-Kluepfel <mhk@caldera.de>
    SPDX-FileCopyrightText: 2023 Ravi Mistry <rvstry@protonmail.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _KCMUSB_H
#define _KCMUSB_H

#include <KQuickAddons/ConfigModule>

class USBViewer : public KQuickAddons::ConfigModule
{
    Q_OBJECT
public:
    USBViewer(QObject *parent, const KPluginMetaData &data, const QVariantList &args);
};

#endif
