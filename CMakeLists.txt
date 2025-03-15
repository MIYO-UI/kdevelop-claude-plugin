cmake_minimum_required(VERSION 3.16)
project(kdevelop-claude-plugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(ECM 5.60.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMQtDeclareLoggingCategory)
include(ECMAddTests)
include(ECMSetupVersion)
include(FeatureSummary)

find_package(KDevPlatform 5.0 REQUIRED)
find_package(KF5 REQUIRED COMPONENTS I18n CoreAddons ConfigWidgets TextEditor ThreadWeaver)
find_package(Qt5 COMPONENTS Core Widgets WebEngineWidgets Network Concurrent REQUIRED)

set(PLUGIN_VERSION_MAJOR 0)
set(PLUGIN_VERSION_MINOR 1)
set(PLUGIN_VERSION_PATCH 0)
set(PLUGIN_VERSION "${PLUGIN_VERSION_MAJOR}.${PLUGIN_VERSION_MINOR}.${PLUGIN_VERSION_PATCH}")

set(kdevclaudeplugin_SRCS
    src/claudeplugin.cpp
    src/claudeview.cpp
    src/googleauth.cpp
    src/claudeapi.cpp
    settings/claudesettings.cpp
    settings/claudeconfigpage.cpp
)

ki18n_wrap_ui(kdevclaudeplugin_SRCS
    settings/claudeconfig.ui
    src/claudeview.ui
)

kdevplatform_add_plugin(kdevclaudeplugin
    JSON kdevclaudeplugin.json
    SOURCES ${kdevclaudeplugin_SRCS}
)

target_link_libraries(kdevclaudeplugin
    KF5::I18n
    KF5::CoreAddons
    KF5::ConfigWidgets
    KF5::TextEditor
    KF5::ThreadWeaver
    KDev::Interfaces
    KDev::Shell
    KDev::Language
    Qt5::Core
    Qt5::Widgets
    Qt5::WebEngineWidgets
    Qt5::Network
    Qt5::Concurrent
)

install(FILES settings/kdevclaudeplugin.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})

# Opcjonalna kompilacja samodzielnego programu testowego
option(BUILD_STANDALONE_TEST "Build standalone test application" OFF)
if(BUILD_STANDALONE_TEST)
    add_executable(claude-test src/main.cpp)
    target_compile_definitions(claude-test PRIVATE STANDALONE_TEST)
    target_link_libraries(claude-test
        Qt5::Core
        Qt5::Widgets
        Qt5::WebEngineWidgets
        Qt5::Network
    )
endif()