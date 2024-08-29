// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2024 Harald Sitter <sitter@kde.org>

#include <iostream>

#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVulkanFunctions>
#include <QVulkanInstance>

using namespace Qt::StringLiterals;

struct Device {
    QString name;
    VkPhysicalDeviceType type;
};

QList<Device> vulkanDevices()
{
    if (qEnvironmentVariableIntValue("KINFOCENTER_SIMULATION") == 1) {
        return {
            {u"AMD Radeon RX 7900 XTX (RADV NAVI31)]"_s, VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU},
            {u"AMD Radeon Graphics (RADV RAPHAEL_MENDOCINO)"_s, VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU},
        };
    }

    QVulkanInstance instance;
    if (!instance.create()) {
        qWarning() << "Failed to create vulkan instance";
        return {};
    }
    auto functions = instance.functions();

    uint32_t count = 0;
    functions->vkEnumeratePhysicalDevices(instance.vkInstance(), &count, nullptr);
    if (count == 0) {
        qWarning("No vulkan devices");
        return {};
    }

    qDebug() << count;

    QVarLengthArray<VkPhysicalDevice, 4> devices(count);
    VkResult error = functions->vkEnumeratePhysicalDevices(instance.vkInstance(), &count, devices.data());
    if (error != VK_SUCCESS || count == 0) {
        qWarning("Failed to enumerate vulkan devices: %d", error);
        return {};
    }

    QList<Device> deviceList;
    deviceList.reserve(devices.count());
    for (const auto &device : devices) {
        VkPhysicalDeviceProperties properties;
        functions->vkGetPhysicalDeviceProperties(device, &properties);
        qDebug("Physical device %d: '%s' %d.%d.%d (api %d.%d.%d vendor 0x%X device 0x%X type %d)",
               0,
               properties.deviceName,
               VK_VERSION_MAJOR(properties.driverVersion),
               VK_VERSION_MINOR(properties.driverVersion),
               VK_VERSION_PATCH(properties.driverVersion),
               VK_VERSION_MAJOR(properties.apiVersion),
               VK_VERSION_MINOR(properties.apiVersion),
               VK_VERSION_PATCH(properties.apiVersion),
               properties.vendorID,
               properties.deviceID,
               properties.deviceType);

        deviceList.emplace_back(QString::fromUtf8(properties.deviceName), properties.deviceType);
    }
    return deviceList;
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QJsonArray devicesArray;

    const auto devices = vulkanDevices();
    for (const auto &device : devices) {
        devicesArray << QJsonObject{
            {QStringLiteral("name"), device.name},
            {QStringLiteral("vkDeviceType"), device.type},
        };
    }

    std::cout << QJsonDocument(devicesArray).toJson(QJsonDocument::Compact).toStdString() << '\n';

    return 0;
}
