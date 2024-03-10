# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2024 Shubham Arora <shubhamarora@protonmail.com>

find_package(pipewire)
set_package_properties(audio PROPERTIES TYPE RUNTIME)

add_definitions(-DTRANSLATION_DOMAIN=\"kcm_audio\")

kinfocenter_add_kcm(kcm_audio main.cpp)
target_link_libraries(kcm_audio PRIVATE KF6::CoreAddons KF6::KCMUtilsQuick KF6::I18n KInfoCenterInternal)