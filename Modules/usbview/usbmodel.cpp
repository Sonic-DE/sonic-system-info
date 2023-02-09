/*
 *   SPDX-FileCopyrightText: 2001 Matthias Hoelzer-Kluepfel <mhk@caldera.de>
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "usbmodel.h"

#include <QTimer>

#include "usbdevices.h"

static quint32 key(USBDevice &dev)
{
    return dev.bus() * 256 + dev.device();
}

static quint32 key_parent(USBDevice &dev)
{
    return dev.bus() * 256 + dev.parent();
}

static QList<QStandardItem *> prepareRow(const QString a, const QString b)
{
    return {new QStandardItem(a), new QStandardItem(b)};
}

static void delete_recursive(QStandardItem *item, const QMap<int, QStandardItem *> new_items)
{
    if (!item)
        return;

    for (int r = 0; r < item->rowCount(); r++) {
        if ((new_items.contains(item->child(r, 1)->text().toUInt())) == false) {
            item->removeRow(r);
        }
        delete_recursive(item->child(r), new_items);
    }
}

USBModel::USBModel()
{
    _items.clear();
    USBModel::refresh();

    QTimer *refreshTimer = new QTimer(this);
    // 1 sec seems to be a good compromise between latency and polling load.
    refreshTimer->start(1000);

    connect(refreshTimer, &QTimer::timeout, this, &USBModel::refresh);
}

USBModel::~USBModel()
{
    USBDevice::clear();
}

void USBModel::refresh()
{
    QMap<int, QStandardItem *> new_items;
    QStandardItem *rootItem = this->invisibleRootItem();

    if (!USBDevice::load())
        return;

    int level = 0;
    bool found = true;

    while (found) {
        found = false;
        foreach (USBDevice *usbDevice, USBDevice::devices()) {
            if (usbDevice->level() == level) {
                quint32 k = key(*usbDevice);
                if (level == 0) {
                    QStandardItem *item = _items.value(k);
                    if (!item) {
                        preparedRow = prepareRow(usbDevice->product(), QString::number(k));
                        rootItem->appendRow(preparedRow);
                        item = preparedRow.at(0);
                    }
                    new_items.insert(k, item);
                    found = true;
                } else {
                    QStandardItem *parent = new_items.value(key_parent(*usbDevice));
                    if (parent) {
                        QStandardItem *item = _items.value(k);
                        if (!item) {
                            preparedRow = prepareRow(usbDevice->product(), QString::number(k));
                            parent->appendRow(preparedRow);
                            item = preparedRow.at(0);
                        }
                        new_items.insert(k, item);
                        found = true;
                    }
                }
            }
        }
        ++level;
    }
    // recursive delete all items not in new_items
    delete_recursive(rootItem, new_items);
    _items = new_items;
}

quint32 USBModel::find(const QString &name)
{
    QStandardItem *foundItem = findItems(name, Qt::MatchRecursive).at(0);
    quint32 busdev = foundItem->index().siblingAtColumn(1).data().toUInt();
    USBDevice *dev = USBDevice::find(busdev >> 8, busdev & 255);
    if (dev) {
        setDetails(dev->dump());
        setProduct(dev->product());
    }
    return busdev;
}

QList<QList<QString>> USBModel::details() const
{
    return m_details;
}

void USBModel::setDetails(QList<QList<QString>> details)
{
    m_details = details;
    emit detailsChanged();
}

QString USBModel::product() const
{
    return m_product;
}

void USBModel::setProduct(QString product)
{
    m_product = product;
    emit productChanged();
}
