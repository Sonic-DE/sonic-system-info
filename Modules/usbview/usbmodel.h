/*
 *   SPDX-FileCopyrightText: 2001 Matthias Hoelzer-Kluepfel <mhk@caldera.de>
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef __USB_MODEL_H__
#define __USB_MODEL_H__

#include "usbdevices.h"
#include <QMap>
#include <QStandardItem>
#include <QStandardItemModel>

class USBModel : public QStandardItemModel
{
public:
    USBModel();
    ~USBModel();

signals:
    void refresh();

private:
    QMap<int, QStandardItem *> _items;
    QList<QStandardItem *> preparedRow;
};

#endif
