cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME HttpsServer)
project(${TARGET_NAME} LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_definitions(-D GLOG_NO_ABBREVIATED_SEVERITIES)


#Qt
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network REQUIRED)

#Zlib 
#find_package(Qt${QUAZIP_QT_MAJOR_VERSION} OPTIONAL_COMPONENTS Zlib)
#if (Qt${QUAZIP_QT_MAJOR_VERSION}Zlib_FOUND)
#    set(QUAZIP_LIB_LIBRARIES ${QUAZIP_LIB_LIBRARIES} Qt${QUAZIP_QT_MAJOR_VERSION}::Zlib)
#    set(QUAZIP_QT_ZLIB_USED ON)
#endif()

set(UIHEADERFOLD ${CMAKE_CURRENT_SOURCE_DIR}/UI)
file(GLOB SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/*.c		
        ${CMAKE_CURRENT_SOURCE_DIR}/HttpOperator/*.cpp
        )

file(GLOB HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/HttpOperator/*.h		
        )
		
file(GLOB_RECURSE STYLESHEETS  *.css)
file(GLOB_RECURSE RESOURCES *.qrc
                                ${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.qrc)
file(GLOB_RECURSE OTHERRESOURCES *.qml)

QT5_WRAP_UI(FORMS_HEADERS ${FORMS})
QT5_ADD_RESOURCES(RESOURCES_RCC ${RESOURCES})	

##添加目录分类
foreach(FILE ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES} ${STYLESHEETS})
   # Get the directory of the source file
   get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
   # Remove common directory prefix to make the group
   string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
   # Make sure we are using windows slashes
   string(REPLACE "/" "\\" GROUP "${GROUP}")
       string(REPLACE ".." "" GROUP "${GROUP}")
       string(REPLACE "Resources" "" GROUP "${GROUP}")
   # Group into "Source Files" and "Header Files"
   if ("${FILE}" MATCHES ".*\\.cpp")
      set(GROUP "Source Files${GROUP}")
   elseif("${FILE}" MATCHES ".*\\.h")
      set(GROUP "Header Files${GROUP}")
       elseif("${FILE}" MATCHES ".*\\.ui")
      set(GROUP "Forms${GROUP}")
   elseif("${FILE}" MATCHES ".*\\.qrc" OR "${FILE}" MATCHES ".*\\.qml" OR "${FILE}" MATCHES ".*\\.jpg")
      set(GROUP "Resources")
       elseif("${FILE}" MATCHES ".*\\.ts")
      set(GROUP "Translator")
   elseif("${FILE}" MATCHES ".*\\.css")
      set(GROUP "StyleSheets")
       else()
               set(GROUP "Other")
       endif()
   #MESSAGE(STATUS ${GROUP} " " ${FILE})
   source_group("${GROUP}" FILES "${FILE}")
endforeach()
include_directories( "${DEMO_SDK}" )
include_directories( "${CMAKE_SOURCE_DIR}" )
include_directories( "${UIHEADERFOLD}" )

LINK_DIRECTORIES(${3RD_PARTY}/glog/lib)
LINK_DIRECTORIES(${3RD_PARTY}/libevent/lib)
#LINK_DIRECTORIES(${3RD_PARTY}/openssl/lib)

if(IDE)
         MESSAGE(STATUS "IDE")
        add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS} ${RESOURCES_RCC})
else()
        MESSAGE(STATUS "NOIDE")
        add_executable(${TARGET_NAME} WIN32 ${SOURCES} ${HEADERS} ${RESOURCES_RCC})
endif()

target_include_directories(${TARGET_NAME} PRIVATE ${3RD_PARTY}/glog/include)
target_include_directories(${TARGET_NAME} PRIVATE ${3RD_PARTY}/libevent/include)
#target_include_directories(${TARGET_NAME} PRIVATE ${3RD_PARTY}/openssl/include)
target_include_directories(${TARGET_NAME} PRIVATE ${DEMO})

target_link_libraries( ${TARGET_NAME} PRIVATE
                                            Qt5::Core
                                            Qt5::Gui
                                            Qt5::Widgets
											Qt5::CorePrivate
											Qt5::GuiPrivate
											Qt5::Network)
											

target_link_libraries( ${TARGET_NAME} PRIVATE  glogd)
target_link_libraries( ${TARGET_NAME} PRIVATE  libevent
											   libevent_core
                                               libevent_extras
											   libevent_openssl
											   libcrypto
											   libssl
											   ws2_32
											   wsock32
											   Iphlpapi
											   )
#target_link_libraries( ${TARGET_NAME} PRIVATE  libeay32
#											   ssleay32
#											   )
#add_custom_command(TARGET ${TARGET_NAME} POST_BUILD        
#   COMMAND ${CMAKE_COMMAND} -E copy_directory             
#        "${3RD_PARTY}/glog/bin/" 
#		"${3RD_PARTY}/openssl/bin/" 
#        $<TARGET_FILE_DIR:${TARGET_NAME}>)    