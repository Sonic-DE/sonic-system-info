/*
 *   SPDX-FileCopyrightText: 2001 Matthias Hoelzer-Kluepfel <mhk@caldera.de>
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "kcmusb.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QList>

#include <QModelIndex>
#include <QSplitter>
#include <QTextEdit>
#include <QTimer>
#include <QTreeView>

#include <KAboutData>

#include <KLocalizedString>
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(USBViewer, "kcmusb.json")

USBViewer::USBViewer(QWidget *parent, const QVariantList &)
    : KCModule(parent)
{
    setQuickHelp(
        i18n("This module allows you to see"
             " the devices attached to your USB bus(es)."));

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QSplitter *splitter = new QSplitter(this);
    splitter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    mainLayout->addWidget(splitter);

    _devices = new QTreeView(splitter);

    model = new USBModel();
    _devices->setModel(model);

    QStringList headers;
    headers << i18n("Device");
    // _devices->setHeaderLabels(headers);
    _devices->expandAll();
    _devices->setItemsExpandable(false);
    _devices->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _devices->header()->hide();
    _devices->hideColumn(1); // hides treeview column containing busdev
    _devices->resizeColumnToContents(0);

    QList<int> sizes;
    sizes.append(200);
    splitter->setSizes(sizes);

    _details = new QTextEdit(splitter);
    _details->setReadOnly(true);

    QTimer *refreshTimer = new QTimer(this);
    // 1 sec seems to be a good compromise between latency and polling load.
    refreshTimer->start(1000);

    connect(refreshTimer, &QTimer::timeout, model, &USBModel::refresh);
    connect(_devices, &QTreeView::clicked, this, &USBViewer::selectionChanged);

    if (model->invisibleRootItem()->hasChildren() == true) {
        _devices->clicked(model->invisibleRootItem()->child(0)->index());
    } // Select first item in treeview so that details pane is not empty.

    KAboutData *about =
        new KAboutData(i18n("kcmusb"), i18n("USB Devices"), QString(), QString(), KAboutLicense::GPL, i18n("(c) 2001 Matthias Hoelzer-Kluepfel"));

    about->addAuthor(i18n("Matthias Hoelzer-Kluepfel"), QString(), QStringLiteral("mhk@kde.org"));
    about->addCredit(i18n("Leo Savernik"), i18n("Live Monitoring of USB Bus"), QStringLiteral("l.savernik@aon.at"));
    setAboutData(about);
}

USBViewer::~USBViewer()
{
    USBDevice::clear();
}

void USBViewer::selectionChanged(const QModelIndex &index)
{
    if (index.isValid()) {
        quint32 busdev = index.siblingAtColumn(1).data().toUInt();
        USBDevice *dev = USBDevice::find(busdev >> 8, busdev & 255);
        if (dev) {
            _details->setHtml(dev->dump());
            return;
        }
    }
    _details->clear();
}

#include "kcmusb.moc"
