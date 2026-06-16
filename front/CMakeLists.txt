cmake_minimum_required(VERSION 3.19)
project(front LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH
    "D:/AAAJove/Qt/6.11.1/msvc2022_64"
    "D:/AAAJove/3dslicer/SDK/VTK-9.6.2-msvc2022-qt6"
)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql OpenGLWidgets)

find_package(VTK REQUIRED COMPONENTS
    CommonCore
    FiltersSources
    GUISupportQt
    InteractionStyle
    RenderingCore
    RenderingOpenGL2
    CommonDataModel
    ImagingCore
    ImagingColor
)

qt_standard_project_setup()

qt_add_executable(front
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    app/AppController.h
    app/AppController.cpp
    core/VolumeNode.h
    core/VolumeNode.cpp
    viewer/MedicalViewportWidget.h
    viewer/MedicalViewportWidget.cpp
    viewer/ViewLayoutWidget.h
    viewer/ViewLayoutWidget.cpp
    viewer/VtkImageDataFactory.h
    viewer/VtkImageDataFactory.cpp
    widgets/RibbonBarWidget.h
    widgets/RibbonBarWidget.cpp
    widgets/ProjectPanelWidget.h
    widgets/ProjectPanelWidget.cpp
    widgets/LogPanelWidget.h
    widgets/LogPanelWidget.cpp
    database/DatabaseManager.h
    database/DatabaseManager.cpp
    database/DicomRepository.h
    database/DicomRepository.cpp
)

target_include_directories(front
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(front
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
        Qt::OpenGLWidgets
        ${VTK_LIBRARIES}
)

vtk_module_autoinit(
    TARGETS front
    MODULES ${VTK_LIBRARIES}
)

if (MSVC)
    set_property(TARGET front PROPERTY VS_DEBUGGER_ENVIRONMENT
        "PATH=D:/AAAJove/Qt/6.11.1/msvc2022_64/bin;D:/AAAJove/3dslicer/SDK/VTK-9.6.2-msvc2022-qt6/bin;%PATH%"
    )
endif()

install(TARGETS front
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET front
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
