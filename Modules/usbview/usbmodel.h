/*
 *   SPDX-FileCopyrightText: 2001 Matthias Hoelzer-Kluepfel <mhk@caldera.de>
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef __USB_MODEL_H__
#define __USB_MODEL_H__

#include <QMap>
#include <QStandardItem>
#include <QStandardItemModel>

class USBModel : public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList details READ details() NOTIFY detailsChanged)

public:
    USBModel();
    ~USBModel();

    QStringList details() const;

signals:
    void detailsChanged();

private:
    void refresh();
    QMap<int, QStandardItem *> _items;
    QList<QStandardItem *> preparedRow;
};

#endif
