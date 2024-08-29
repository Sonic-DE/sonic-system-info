/*
    SPDX-FileCopyrightText: 2016 Rohan Garg <rohan@kde.org>
    SPDX-FileCopyrightText: 2020-2021 Harald Sitter <sitter@kde.org>
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "GPUEntry.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QProcess>
#include <QStandardPaths>

#include <KLibexec>
#include <KLocalizedString>

#include "FancyString.h"

extern "C" {
#include <xf86drm.h>
}

using namespace std::chrono_literals;
using namespace Qt::StringLiterals;

namespace
{
template<typename Output, typename Input>
constexpr Output narrow(Input i)
{
    Output o = static_cast<Input>(i);
    if (i != Input(o)) {
        std::abort();
    }
    return o;
}

QJsonDocument readFromProcess(const QString &executable, std::optional<int> deviceIndex = std::nullopt)
{
    QProcess process;
    if (deviceIndex.has_value() && deviceIndex.value() > 0) {
        auto processEnvironment = QProcessEnvironment::systemEnvironment();
        processEnvironment.insert("DRI_PRIME", QString::number(deviceIndex.value()));
        process.setProcessEnvironment(processEnvironment);
    }
    process.setProcessChannelMode(QProcess::ForwardedErrorChannel);
    process.setProgram(executable);
    process.start();
    process.waitForFinished(std::chrono::milliseconds(2s).count());
    auto output = process.readAllStandardOutput();
    qDebug() << output;
    return QJsonDocument::fromJson(output);
}

int deviceCount()
{
    static auto count = [] {
        if (qEnvironmentVariableIntValue("KINFOCENTER_SIMULATION") == 1) {
            return 2;
        }
        return drmGetDevices2(0, nullptr, 0);
    }();
    return count;
}

QStringList searchPaths()
{
    static auto paths = KLibexec::kdeFrameworksPaths(QLatin1String(LIBEXECDIR_FROM_KCM)) << QLatin1String(KDE_INSTALL_FULL_LIBEXECDIR);
    return paths;
}

std::optional<QStringList> gpusFromJsonArray(const QJsonArray &array)
{
    QStringList gpus;
    gpus.reserve(array.size());
    for (const auto &device : array) {
        const auto obj = device.toObject();
        gpus << FancyString::fromRenderer(obj.value("name").toString());
    }
    if (gpus.count() != deviceCount()) {
        qWarning() << "GPU count mismatch" << gpus.count() << deviceCount();
        return {};
    }
    return gpus;
}

std::optional<QStringList> vulkanGPUs()
{
    auto vulkanHelperExecutable = QStandardPaths::findExecutable("kinfocenter-vulkan-helper", searchPaths());
    qDebug() << "Looking at" << searchPaths() << vulkanHelperExecutable;

    auto document = readFromProcess(vulkanHelperExecutable);
    if (!document.isArray()) {
        qWarning() << "Failed to read GPU info from vulkan helper";
        return {};
    }

    return gpusFromJsonArray(document.array());
}

std::optional<QStringList> openglGPUs()
{
    auto openglHelperExecutable = QStandardPaths::findExecutable("kinfocenter-opengl-helper", searchPaths());
    qDebug() << "Looking at" << searchPaths() << openglHelperExecutable;

    QJsonArray array;
    for (auto i = 0; i < deviceCount(); ++i) {
        auto document = readFromProcess(openglHelperExecutable, i);
        if (!document.isArray()) {
            qWarning() << "Failed to read GPU info from opengl helper for device" << i;
            return {};
        }

        const auto incomingArray = document.array();
        for (const auto &device : incomingArray) {
            array.append(device);
        }
    }

    return gpusFromJsonArray(array);
}

std::optional<QStringList> gpus()
{
    if (deviceCount() == 0) {
        return {};
    }

    if (auto optionalGpus = vulkanGPUs(); optionalGpus.has_value()) {
        return optionalGpus;
    }

    if (auto optionalGpus = openglGPUs(); optionalGpus.has_value()) {
        return optionalGpus;
    }

    return {};
}

QString fancyOpenGLRenderer()
{
    QOpenGLContext context;
    QOffscreenSurface surface;
    surface.create();
    if (!context.create()) {
        qWarning() << "Failed create QOpenGLContext";
        return {};
    }

    if (!context.makeCurrent(&surface)) {
        qWarning() << "Failed to make QOpenGLContext current";
        return {};
    }

    auto renderer = QString::fromUtf8(reinterpret_cast<const char *>(context.functions()->glGetString(GL_RENDERER)));
    context.doneCurrent();

    return FancyString::fromRenderer(renderer);
}
} // namespace

GPUEntry::GPUEntry()
    : Entry(ki18n("Graphics Processor:"), QString())
{
    if (auto optionalGpus = gpus(); optionalGpus.has_value()) {
        m_value = optionalGpus->join('\n');
    } else { // fall back to old logic of using the GL_RENDERER
        m_value = fancyOpenGLRenderer();
    }
}
