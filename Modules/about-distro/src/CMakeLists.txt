configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/Version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(kcm_SRCS
    Module.cpp
    Entry.cpp
    PlasmaEntry.cpp
    KernelEntry.cpp
    SectionLabel.cpp
    MemoryEntry.cpp
    GraphicsPlatformEntry.cpp
    CPUEntry.cpp
    GPUEntry.cpp
    FancyString.cpp
)

ki18n_wrap_ui(kcm_SRCS Module.ui)

kcoreaddons_add_plugin(kcm_about_distro SOURCES ${kcm_SRCS} INSTALL_NAMESPACE "plasma/kcms")
kcoreaddons_desktop_to_json(kcm_about_distro "about-distro.desktop")

target_link_libraries(kcm_about_distro
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::I18n
    KF5::Solid)

