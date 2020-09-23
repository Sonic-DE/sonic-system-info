/**
 * SPDX-FileCopyrightText: 2001 Alexander Neundorf <neundorf@kde.org>
 * SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
 *
 * SPDX-LicenseIndentifier: GPL-2.0-or-later
 */

#pragma once

#include <KQuickAddons/ConfigModule>
#include <memory>

class NetworkModel;

class KCMNic : public KQuickAddons::ConfigModule
{
    Q_OBJECT

    /**
     * Expose the network information to the QML frontend
     */
    Q_PROPERTY(NetworkModel *networkModel READ networkModel NOTIFY networkModelChanged)

public:
    explicit KCMNic(QObject *parent = nullptr, const QVariantList &list = QVariantList());
    virtual ~KCMNic() override = default;

    NetworkModel *networkModel() const;

public Q_SLOTS:
    void update();

Q_SIGNALS:
    void networkModelChanged();

protected:
    std::unique_ptr<NetworkModel> m_networkModel;
};
