/*
 *   SPDX-FileCopyrightText: 2001 Matthias Hoelzer-Kluepfel <mhk@caldera.de>
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef _KCMUSB_H
#define _KCMUSB_H

#include "usbmodel.h"
#include <QMap>

#include <KCModule>

class QTreeWidget;
class QTreeWidgetItem;
class QStandardItem;
class QTreeView;
class QTextEdit;
class QModelIndex;

class USBViewer : public KCModule
{
    Q_OBJECT

public:
    explicit USBViewer(QWidget *parent = nullptr, const QVariantList &list = QVariantList());
    ~USBViewer() override;

protected Q_SLOTS:

    void selectionChanged(const QModelIndex &index);

private:
    QTreeView *_devices;
    USBModel *model;
    QTextEdit *_details;
};

#endif
