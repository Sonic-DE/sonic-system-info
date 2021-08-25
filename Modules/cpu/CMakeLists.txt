# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2021 Harald Sitter <sitter@kde.org>

find_package(lscpu)
set_package_properties(lscpu PROPERTIES TYPE RUNTIME)

add_definitions(-DTRANSLATION_DOMAIN=\"kcm_cpu\")

kcoreaddons_add_plugin(kcm_cpu SOURCES main.cpp INSTALL_NAMESPACE "plasma/kcms/kinfocenter")
target_link_libraries(kcm_cpu KF5::CoreAddons KF5::QuickAddons KF5::I18n KInfoCenterInternal)

kcoreaddons_desktop_to_json(kcm_cpu "kcm_cpu.desktop")

kpackage_install_package(package kcm_cpu kcms)
