cmake_minimum_required (VERSION 3.10)
project (brainflow)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_VERBOSE_MAKEFILE ON)
set (BRAINFLOW_VERSION 1.0.0)
set (INSTALL_NAME brainflow_installed)

macro (configure_msvc_runtime)
    if (MSVC)
        # Default to statically-linked runtime.
        if ("${MSVC_RUNTIME}" STREQUAL "")
            set (MSVC_RUNTIME "static")
        endif ()
        # Set compiler options.
        set (variables
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
        )
        if (${MSVC_RUNTIME} STREQUAL "static")
            message(STATUS
                "MSVC -> forcing use of statically-linked runtime."
            )
            foreach (variable ${variables})
                if (${variable} MATCHES "/MD")
                    string (REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
                endif ()
            endforeach ()
        else ()
            message (STATUS
                "MSVC -> forcing use of dynamically-linked runtime."
            )
            foreach (variable ${variables})
                if (${variable} MATCHES "/MT")
                    string (REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
                endif ()
            endforeach ()
        endif ()
    endif ()
endmacro ()

if (MSVC)
    configure_msvc_runtime()
endif ()

set (LIBRARY_SRC
    "src/utils/timestamp.cpp"
    "src/utils/data_buffer.cpp"
    "src/board_controller/openbci/openbci_board.cpp"
    "src/board_controller/openbci/cyton.cpp"
    "src/board_controller/board_controller.cpp"
    "src/board_controller/board.cpp"
)

add_library (
    BoardController SHARED
    ${LIBRARY_SRC}
)

target_include_directories (
    BoardController PUBLIC
    inc
    src/utils/
    src/board_controller/inc
    src/board_controller/openbci/inc
)

set_target_properties (BoardController
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/compiled"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/compiled"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/compiled"
)

if (UNIX)
    target_link_libraries (BoardController -lpthread)
endif (UNIX)

add_subdirectory (cpp-package)

if (MSVC)
    add_custom_command (TARGET BoardController POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_HOME_DIRECTORY}/compiled/$<CONFIG>/BoardController.dll" "${CMAKE_HOME_DIRECTORY}/python-package/brainflow/lib/BoardController.dll"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_HOME_DIRECTORY}/compiled/$<CONFIG>/BoardController.dll" "${CMAKE_HOME_DIRECTORY}/matlab-package/brainflow/lib/BoardController.dll"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_HOME_DIRECTORY}/src/board_controller/inc/board_controller.h" "${CMAKE_HOME_DIRECTORY}/matlab-package/brainflow/inc/board_controller.h"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_HOME_DIRECTORY}/compiled/$<CONFIG>/BoardController.dll" "${CMAKE_HOME_DIRECTORY}/java-package/brainflow/src/main/resources/BoardController.dll"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_HOME_DIRECTORY}/compiled/$<CONFIG>/BoardController.dll" "${CMAKE_HOME_DIRECTORY}/csharp-package/brainflow/brainflow/BoardController.dll"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_HOME_DIRECTORY}/compiled/$<CONFIG>/BoardController.dll" "${CMAKE_HOME_DIRECTORY}/python-package/brainflow/lib/BoardController.dll"
    )
endif (MSVC)
if (UNIX)
    add_custom_command (TARGET BoardController POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_HOME_DIRECTORY}/compiled/libBoardController.so" "${CMAKE_HOME_DIRECTORY}/python-package/brainflow/lib/libBoardController.so"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_HOME_DIRECTORY}/compiled/libBoardController.so" "${CMAKE_HOME_DIRECTORY}/matlab-package/brainflow/lib/libBoardController.so"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_HOME_DIRECTORY}/src/board_controller/inc/board_controller.h" "${CMAKE_HOME_DIRECTORY}/matlab-package/brainflow/inc/board_controller.h"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_HOME_DIRECTORY}/compiled/libBoardController.so" "${CMAKE_HOME_DIRECTORY}/java-package/brainflow/src/main/resources/libBoardController.so"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_HOME_DIRECTORY}/compiled/libBoardController.so" "${CMAKE_HOME_DIRECTORY}/csharp-package/brainflow/brainflow/libBoardController.so"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_HOME_DIRECTORY}/compiled/libBoardController.so" "${CMAKE_HOME_DIRECTORY}/python-package/brainflow/lib/libBoardController.so"
    )
endif (UNIX)

set (version_config "${CMAKE_HOME_DIRECTORY}/compiled/brainflowConfigVersion.cmake")
set (config_install_dir "lib/cmake/brainflow")
set (project_config "${CMAKE_HOME_DIRECTORY}/compiled/brainflowConfig.cmake")
set (targets_export_name brainflowTargets)

include (CMakePackageConfigHelpers)

write_basic_package_version_file (
    "${version_config}"
    VERSION ${BRAINFLOW_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file (
    "cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

install (
    FILES
    "src/board_controller/inc/board_controller.h"
    "cpp-package/src/data_handler.h"
    "cpp-package/src/board_shim.h"
    DESTINATION inc
)

install (
    TARGETS BoardController GetData
    EXPORT ${targets_export_name}
    RUNTIME DESTINATION lib
    LIBRARY DESTINATION lib
    INCLUDES DESTINATION inc
)

install (
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

install (
    EXPORT ${targets_export_name}
    NAMESPACE brainflow::
    DESTINATION "${config_install_dir}"
)