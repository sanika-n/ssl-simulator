# must include kshetra.h also so that auto moc compiler works
add_library(yodha src/yodha.cpp include/yodha/yodha.h
    include/yodha/mantri.h
    include/yodha/mantri.cpp
    )

# Find SDL2 package
find_package(SDL2 REQUIRED)

target_link_libraries(yodha
    Qt5::Core
    Qt5::Widgets
    # Qt5::Gamepad  # Remove Qt Gamepad dependency
    SDL2::SDL2     # Add SDL2 dependency
    katha::shunya
)

# this will allow the linker to find .h files in other packages which are linked
target_include_directories(yodha
    INTERFACE include
    PRIVATE include/yodha
    PRIVATE ${SDL2_INCLUDE_DIRS}  # Add SDL2 include directories
)
add_library(katha::yodha ALIAS yodha)
