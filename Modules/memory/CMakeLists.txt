# KI18N Translation Domain for this library
add_definitions(-DTRANSLATION_DOMAIN=\"kcm_memory\")

########### next target ###############

set(KCM_MEMORY_PART_SRCS memory.cpp chartWidget.cpp totalMemoryChart.cpp physicalMemoryChart.cpp swapMemoryChart.cpp )

kcoreaddons_add_plugin(kcm_memory SOURCES ${KCM_MEMORY_PART_SRCS} INSTALL_NAMESPACE "plasma/kinfocenter")
kcoreaddons_desktop_to_json(kcm_memory "kcm_memory.desktop")

target_link_libraries(kcm_memory KF5::KCMUtils KF5::I18n KF5::CoreAddons)

if (${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
   target_link_libraries(kcm_memory kvm)
endif ()
