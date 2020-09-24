/**
 * SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
 *
 * SPDX-LicenseIndentifier: GPL-2.0-or-later
 */

#pragma once

#include <QAbstractTableModel> 

class NetworkModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    struct MyNIC {
        QString name;
        QString addr;
        QString netmask;
        QString state;
        QString type;
        QString HWaddr;
    };
    
    enum Roles {
        NameRole = 0,
        AddrRole = 1,
        NetMaskRole = 2,
        TypeRole = 3,
        HWAddrRole = 4,
        StateRole = 5,
    };
    
    explicit NetworkModel(QObject *parent = nullptr);
    virtual ~NetworkModel() = default;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    
    Q_INVOKABLE void update();
    
private:
    QList<MyNIC *> m_nics;
};
