/*
 * SPDX-FileCopyrightText: 2026 Alexander Wilms <f.alexander.wilms@outlook.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "NetworkEntry.h"

#include <KLocalizedString>
#include <NetworkManagerQt/Device>
#include <NetworkManagerQt/Manager>
#include <Solid/Device>

std::vector<Entry *> NetworkEntryFactory::factorize()
{
    std::vector<Entry *> entries;

    // Get all network devices from NetworkManager
    auto devices = NetworkManager::networkInterfaces();

    if (devices.isEmpty()) {
        return entries;
    }

    int adapterCount = 0;
    for (int i = 0; i < devices.size(); ++i) {
        const auto &device = devices[i];

        // Only show physical network adapters (whitelist approach)
        // Skip virtual devices, loopback, bridges, tunnels, etc.
        using DeviceType = NetworkManager::Device::Type;
        const auto type = device->type();
        if (type != DeviceType::Ethernet && type != DeviceType::Wifi && type != DeviceType::Bluetooth && type != DeviceType::Modem
            && type != DeviceType::InfiniBand && type != DeviceType::Adsl) {
            continue;
        }

        adapterCount++;

        // Label: "Network Adapter:" or "Network Adapter 1:", "Network Adapter 2:", etc.
        KLocalizedString label;
        if (devices.size() > 1) {
            label = ki18n("Network Adapter %1:").subs(adapterCount);
        } else {
            label = ki18n("Network Adapter:");
        }

        // Get human-readable name from Solid with vendor and product on separate lines
        QString value = device->interfaceName(); // Fallback

        if (!device->udi().isEmpty()) {
            QString solidUdi = device->udi();
            if (!solidUdi.startsWith(QLatin1String("/org/kde/solid/"))) {
                solidUdi = QLatin1String("/org/kde/solid/udev") + solidUdi;
            }

            Solid::Device solidDevice(solidUdi);
            if (solidDevice.isValid()) {
                QString vendorName = solidDevice.vendor();
                QString productName = solidDevice.product();

                // Format vendor and product on separate lines for better readability
                if (!vendorName.isEmpty() && !productName.isEmpty()) {
                    // Check if product already contains vendor to avoid duplication
                    if (productName.toLower().contains(vendorName.toLower())) {
                        value = productName;
                    } else {
                        value = vendorName + QLatin1String("\n") + productName;
                    }
                } else if (!productName.isEmpty()) {
                    value = productName;
                } else if (!vendorName.isEmpty()) {
                    value = vendorName;
                }
            }
        }

        auto *entry = new Entry(label, value);
        entries.push_back(entry);
    }

    return entries;
}
