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
    Q_PROPERTY(QList<QList<QString>> details READ details WRITE setDetails NOTIFY detailsChanged)

public:
    USBModel();
    ~USBModel();

    Q_INVOKABLE QVariant find(const QModelIndex &index);

    QList<QList<QString>> details() const;
    void setDetails(QList<QList<QString>> details);

signals:
    void detailsChanged();

private:
    void refresh();
    QMap<int, QStandardItem *> _items;
    QList<QStandardItem *> preparedRow;

    QList<QList<QString>> m_details;
};

#endif
