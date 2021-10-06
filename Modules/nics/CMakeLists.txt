# KI18N Translation Domain for this library
add_definitions(-DTRANSLATION_DOMAIN=\"kcmnic\")

include(CheckStructHasMember)

# sa_len is only available on freebsd, not linux.
check_struct_has_member("struct sockaddr" "sa_len" "sys/socket.h" HAVE_STRUCT_SOCKADDR_SA_LEN) # networkmodel.cpp

configure_file(config-nic.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-nic.h)

########### next target ###############
set(kcm_nic_PART_SRCS
    nic.cpp
    networkmodel.cpp)

add_library(kcm_nic MODULE ${kcm_nic_PART_SRCS})

target_link_libraries(kcm_nic
    KF5::I18n
    KF5::CoreAddons
    KF5::KCMUtils
    KF5::QuickAddons)

target_compile_features(kcm_nic PUBLIC cxx_std_14)

kcoreaddons_desktop_to_json(kcm_nic "kcm_nic.desktop")

install(FILES kcm_nic.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(TARGETS kcm_nic DESTINATION ${KDE_INSTALL_PLUGINDIR}/kcms)

kpackage_install_package(package kcm_nic kcms)
