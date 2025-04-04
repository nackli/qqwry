# @author nackli <nackli@163.com>
# @version 1.0
# @copyright 2025 nackli. All rights reserved.
# @License: MIT (https://opensource.org/licenses/MIT)
# @Created: 2025-03-20
#


function(use_pack_fun PACKAGE_NAME PAGE_TYPE)
    message("build page = ${PACKAGE_NAME}")
    file(COPY "${CMAKE_SOURCE_DIR}/qqwry.dat" DESTINATION "${CMAKE_BINARY_DIR}/qqwry.datc")
    include(InstallRequiredSystemLibraries)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        install(TARGETS ${PROJECT_NAME} DESTINATION bin)
        install(FILES qqwry.dat DESTINATION bin)
       # set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
        set(CPACK_PACKAGE_VERSION_MAJOR "${${PACKAGE_NAME}_VERSION_MAJOR}")
        set(CPACK_PACKAGE_VERSION_MINOR "${${PACKAGE_NAME}_VERSION_MINOR}")
        set(CPACK_PACKAGE_VERSION_PATCH "${${PACKAGE_NAME}_VERSION_PATCH}")
        set(CPACK_GENERATOR "ZIP")
        set(CPACK_PACKAGE_NAME "${PACKAGE_NAME}")
        set(CPACK_PACKAGE_FILE_NAME "${PACKAGE_NAME}")
        include(CPack)
    endif()
endfunction(use_pack_fun)
