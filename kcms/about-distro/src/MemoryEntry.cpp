/*
    SPDX-FileCopyrightText: 2012-2020 Harald Sitter <sitter@kde.org>
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "MemoryEntry.h"

#include <KFormat>

#ifdef Q_OS_LINUX
#include <sys/sysinfo.h>
#ifdef UDEV_FOUND
#include <libudev.h>
#endif
#elif defined(Q_OS_FREEBSD)
// clang-format off
#include <sys/types.h>
#include <sys/sysctl.h>
// clang-format on
#endif

MemoryEntry::MemoryEntry()
    : Entry(ki18n("Memory:"), QString() /* overridden here */)
{
    m_totalRam = calculateTotalRam();
    m_availableRam = calculateAvailableRam();
}

qlonglong MemoryEntry::calculateTotalRam()
{
    qlonglong ret = -1;

#if defined(Q_OS_LINUX) && defined(UDEV_FOUND)
    struct udev *udev = udev_new();
    if (!udev) {
        return ret;
    }

    struct udev_device *dmi = udev_device_new_from_syspath(udev, "/sys/devices/virtual/dmi/id");
    if (!dmi) {
        return ret;
    }

    const char *num_memory_devices_cstr = udev_device_get_property_value(dmi, "MEMORY_ARRAY_NUM_DEVICES");
    if (!num_memory_devices_cstr) {
        return ret;
    }

    ret = 0;
    int num_memory_devices = atoi(num_memory_devices_cstr);
    for (int i = 0; i < num_memory_devices; ++i) {
        const char *memory_bytes_cstr = udev_device_get_property_value(dmi, QStringLiteral("MEMORY_DEVICE_%1_SIZE").arg(i).toLatin1());
        if (memory_bytes_cstr) {
            ret += strtoull(memory_bytes_cstr, nullptr, 10);
        }
    }

    udev_device_unref(dmi);
    udev_unref(udev);
#endif

    /*
     * TODO: A FreeBSD impl is likely possible, as sysctlbyname() can be used
     *         with "hw.physmem", "hw.usermem" and "hw.realmem".
     *       We should be careful that both this and calculateAvailableRAM
     *         return what is actually wanted!
     */

    return ret;
}

qlonglong MemoryEntry::calculateAvailableRam()
{
    qlonglong ret = -1;
#ifdef Q_OS_LINUX
    struct sysinfo info;
    if (sysinfo(&info) == 0)
        // manpage "sizes are given as multiples of mem_unit bytes"
        ret = qlonglong(info.totalram) * info.mem_unit;
#elif defined(Q_OS_FREEBSD)
    /* Stuff for sysctl */
    size_t len;

    unsigned long memory;
    len = sizeof(memory);
    sysctlbyname("hw.physmem", &memory, &len, NULL, 0);

    ret = memory;
#endif
    return ret;
}

QString MemoryEntry::localizedValue(Language language) const
{
    const int totalRamPrecision = (m_totalRam % (1024 * 1024 * 1024) == 0) ? 0 : 1;
    const int availableRamPrecision = (m_availableRam % (1024 * 1024 * 1024) == 0) ? 0 : 1;

    if (m_totalRam >= 0 && m_availableRam >= 0) {
        // Both known
        const auto string = ki18nc("@label, %1 is the total amount of installed system memory, %2 is the amount of which is usable", "%1 of RAM (%2 usable)")
                                .subs(KFormat(localeForLanguage(language)).formatByteSize(m_totalRam, totalRamPrecision))
                                .subs(KFormat(localeForLanguage(language)).formatByteSize(m_availableRam, availableRamPrecision));
        return localize(string, language);
    } else if (m_totalRam >= 0 && m_availableRam < 0) {
        // Known total, unknown available
        const auto string = ki18nc("@label, %1 is the total amount of installed system memory", "%1 of RAM")
                                .subs(KFormat(localeForLanguage(language)).formatByteSize(m_totalRam, totalRamPrecision));
        return localize(string, language);
    } else if (m_totalRam < 0 && m_availableRam >= 0) {
        // Unknown total, known available
        const auto string = ki18nc("@label, %1 is the amount of installed system memory which is usable", "%1 of usable RAM")
                                .subs(KFormat(localeForLanguage(language)).formatByteSize(m_availableRam, availableRamPrecision));
        return localize(string, language);
    } else {
        // Both unknown
        return localize(ki18nc("@label, Unknown amount of system memory", "Unknown"), language);
    }
}

QString MemoryEntry::localizedHelp(Language language) const
{
    if (m_totalRam >= 0 && m_availableRam >= 0) {
        // Both known
        return localize(ki18nc("@info:tooltip, referring to system memory or RAM",
                               "Some memory is reserved for use by system hardware, such as integrated graphics memory."),
                        language);
    } else if (m_totalRam >= 0 && m_availableRam < 0) {
        // Known total, unknown available
        return localize(ki18nc("@info:tooltip, referring to system memory or RAM",
                               "The amount of usable memory may be lower than the displayed amount because some memory is reserved for use by system hardware, "
                               "such as integrated graphics memory."),
                        language);
    } else if (m_totalRam < 0 && m_availableRam >= 0) {
        // Unknown total, known available
        return localize(ki18nc("@info:tooltip, referring to system memory or RAM",
                               "The amount of memory displayed may be lower than the installed amount because some memory is reserved for use by system "
                               "hardware, such as integrated graphics memory."),
                        language);
    } else {
        // Both unknown
        return QString();
    }
}
