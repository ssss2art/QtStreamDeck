add_library(QtStreamDeck SHARED
    StreamDeckDevice.cpp
    StreamDeckManager.cpp
)

target_include_directories(QtStreamDeck PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(QtStreamDeck
    Qt5::Core
    Qt5::Gui
    Qt5::Svg
    hidapi::hidapi
)

set_target_properties(QtStreamDeck PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
