message("-- [INFO] Building Native BLE")
include_directories(${PROJECT_DIR_PATH}/src-ble)

IF (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include_directories(${PROJECT_DIR_PATH}/src-ble/windows)
    file(GLOB_RECURSE SRC_BLE_FILES "src-ble/NativeBleController.cpp" "src-ble/windows/*.cpp"  "src-ble/windows/*.cc")
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include_directories(${PROJECT_DIR_PATH}/src-ble/macos)
    file(GLOB_RECURSE SRC_BLE_FILES "src-ble/NativeBleController.cpp" "src-ble/macos/*.cpp"  "src-ble/macos/*.cc" "src-ble/macos/*.mm")
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include_directories(${PROJECT_DIR_PATH}/src-ble/linux)
    file(GLOB_RECURSE SRC_BLE_FILES "src-ble/NativeBleController.cpp" "src-ble/linux/*.cpp"  "src-ble/linux/*.cc")
ENDIF()

add_library(nativeble-static STATIC ${SRC_BLE_FILES})
target_link_libraries(nativeble-static ${BLE_LIBS})
add_library(nativeble SHARED ${SRC_BLE_FILES})
target_link_libraries(nativeble ${BLE_LIBS})

file(COPY "src-ble/NativeBleController.h" "src-ble/NativeBleControllerTypes.h" DESTINATION ${OUTPUT_DIR})

install(FILES "src-ble/NativeBleController.h" "src-ble/NativeBleControllerTypes.h" DESTINATION include)
install(TARGETS nativeble-static nativeble)

IF(ENABLE_LIB_BLE_C_API)
    message("-- [INFO] Building Native BLE C Wrapper")
    file(GLOB_RECURSE SRC_BLE_C_API_FILES "src-ble-c-api/*.cpp" "src-ble-c-api/*.h")
    add_library(nativeble_c SHARED ${SRC_BLE_C_API_FILES})
    target_link_libraries(nativeble_c nativeble-static)

    install(FILES "src-ble-c-api/ble_c_api.h" DESTINATION include)
    install(TARGETS nativeble_c)
ENDIF()

IF(ENABLE_APP_BLE_TEST)
    message("-- [INFO] Building Native BLE Test Application")
    file(GLOB_RECURSE SRC_BLE_TEST_FILES "src-ble-test/*.cpp" "src-ble-test/*.h" "src-ble-test/*.cc")
    add_executable(nativeble_tester ${SRC_BLE_TEST_FILES})
    target_link_libraries(nativeble_tester nativeble-static)
ENDIF()
