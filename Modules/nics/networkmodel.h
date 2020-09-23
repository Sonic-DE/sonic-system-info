/**
 * SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
 *
 * SPDX-LicenseIndentifier: GPL-2.0-or-later
 */

#pragma once

#include <QAbstractListModel> 

class NetworkModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    enum NICRoles {
        NameRole = Qt::UserRole + 1,
        AddresseRole,
        NetMaskRole,
        StateRole,
        TypeRole,
        HardwareAddressRole,
    };
    
    struct MyNIC {
        QString name;
        QString addr;
        QString netmask;
        QString state;
        QString type;
        QString HWaddr;
    };
    
    explicit NetworkModel(QObject *parent = nullptr);
    virtual ~NetworkModel() = default;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    
    void update();
    
private:
    QList<MyNIC *> m_nics;
};
