cmake_minimum_required(VERSION 3.16)
#############################################################
#############################################################
 SET (OSGeo4W_dir C:/OSGeo4W)
 SET (QT_dir ${OSGeo4W_dir}/apps/Qt5)
 SET (SOURCE_DIR_PATH .)
 SET (GENERAL_PATH ./libs)
 SET (LIB_PATH .)
#############################################################
#############################################################
project(CarRent LANGUAGES CXX)
  SET(QT_MIN_VERSION 5.9.0)
  FIND_PACKAGE(Qt5Core QUIET)
  FIND_PACKAGE(Qt5Gui REQUIRED)
  FIND_PACKAGE(Qt5Widgets REQUIRED)
  FIND_PACKAGE(Qt5Network REQUIRED)
  FIND_PACKAGE(Qt5Xml REQUIRED)
  FIND_PACKAGE(Qt5Concurrent REQUIRED)
  FIND_PACKAGE(Qt5PrintSupport REQUIRED)
  FIND_PACKAGE(Qt5Positioning)
  FIND_PACKAGE(Qt5UiTools REQUIRED)
  FIND_PACKAGE(Qt5Sql REQUIRED)
  
  FIND_PROGRAM(QT_LRELEASE_EXECUTABLE
    NAMES lrelease
    PATHS ${QT_BINARY_DIR}
    NO_DEFAULT_PATH
  )
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

ADD_DEFINITIONS(-DQT_NO_CAST_TO_ASCII)
set(CMAKE_CXX_STANDARD 17)
# from QGIS project
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8 /std:c++17")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /utf-8 /std:c++17")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  # disable macros that offend std::numeric_limits<T>::min()/max()
  add_definitions(-DNOMINMAX)
      add_definitions(-D_USE_MATH_DEFINES)

      # Turn off deprecation warnings
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
      add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
      cmake_policy(SET CMP0020 NEW)

##find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

SET (SRCS

     CarRent.h
     CarRent.cpp
     ${LIB_PATH}/UserModel.h
     ${LIB_PATH}/UserModel.cpp
     ${LIB_PATH}/CarModel.h
     ${LIB_PATH}/CarModel.cpp
     ${LIB_PATH}/locationsModel.h
     ${LIB_PATH}/locationsModel.cpp
     ${LIB_PATH}/RentModel.h
     ${LIB_PATH}/RentModel.cpp
     ${LIB_PATH}/carTypesModel.h
     ${LIB_PATH}/carTypesModel.cpp
     main.cpp

#     ${GENERAL_PATH}/Base/Base_Base64.cpp 
#     ${GENERAL_PATH}/Base/Base_Base64.h 
#     ${GENERAL_PATH}/Base/Base_Crypto.cpp 
#     ${GENERAL_PATH}/Base/Base_Crypto.h 
#     ${GENERAL_PATH}/Base/Base_Trace.cpp 
#     ${GENERAL_PATH}/Base/Base_Trace.h 
#     ${GENERAL_PATH}/Base/Base_Utils.cpp 
#     ${GENERAL_PATH}/Base/Base_Utils.h 
#     ${GENERAL_PATH}/Base/ConfigFile.cpp 
#     ${GENERAL_PATH}/Base/ConfigFile.h 
#     ${GENERAL_PATH}/Base/BaseQt_Trace.cpp 
#     ${GENERAL_PATH}/Base/BaseQt_Trace.h 
#     ${GENERAL_PATH}/Base/BaseQt_Utils.cpp 
#     ${GENERAL_PATH}/Base/BaseQt_Utils.h 
#     ${GENERAL_PATH}/PostgresDB/JsonData.cpp      
#     ${GENERAL_PATH}/PostgresDB/JsonData.h 
#     ${GENERAL_PATH}/PostgresDB/DBConnectionManager.cpp 
#     ${GENERAL_PATH}/PostgresDB/DBConnectionManager.h 
#     ${GENERAL_PATH}/PostgresDB/DbManager.cpp 
#     ${GENERAL_PATH}/PostgresDB/DbManager.h 
#     ${GENERAL_PATH}/PostgresDB/DbSchema.cpp 
#     ${GENERAL_PATH}/PostgresDB/DbSchema.h 
#     ${GENERAL_PATH}/PostgresDB/KeyValueData.cpp 
#     ${GENERAL_PATH}/PostgresDB/KeyValueData.h 
#     ${GENERAL_PATH}/PostgresDB/TableData.cpp 
#     ${GENERAL_PATH}/PostgresDB/TableData.h 
    )
    
SET (RCCS 
	CarRent.qrc
)
    
SET (UIS 
	CarRent.ui
)

SET (MOC_HDRS
     CarRent.h
)

SET (RCCS  CarRent.qrc)

add_definitions(-D_USE_MATH_DEFINES -DNO_QGIS)

QT5_WRAP_UI (UIS_H  ${UIS})

QT5_WRAP_CPP (MOC_SRCS  ${MOC_HDRS})

QT5_ADD_RESOURCES(RCC_SRCS ${RCCS})

add_executable(CarRent WIN32 ${SRCS} ${MOC_SRCS} ${RCC_SRCS} ${UIS_H})

INCLUDE_DIRECTORIES(
     ${CMAKE_CURRENT_BINARY_DIR} 
     ${QT_dir}/include 
     ${QT_dir}/include/QtCore 
     ${QT_dir}/include/QtGui
     ${QT_dir}/include/QtSql
     ${QT_dir}/include/QtXml
     ${QT_dir}/include/QtNetwork
     ${QT_dir}/include/QtWidgets
     ${GENERAL_PATH} 
     ${GENERAL_PATH}/Base 
#     ${GENERAL_PATH}/OpenSSL_dyn/include              
     ${GENERAL_PATH_QT}/PostgresDB 
     ${SOURCE_DIR_PATH}/output
     ${SOURCE_DIR_PATH}
)

TARGET_LINK_LIBRARIES(CarRent

#  ${GENERAL_PATH}/OpenSSL_dyn/lib/debug/libeay32
#  ${GENERAL_PATH}/OpenSSL_dyn/lib/debug/ssleay32
  ${QT_dir}/lib/Qt5Core.lib
  ${QT_dir}/lib/Qt5Gui.lib
  ${QT_dir}/lib/Qt5Sql.lib
  ${QT_dir}/lib/Qt5Xml.lib
  ${QT_dir}/lib/Qt5Network.lib
  ${QT_dir}/lib/Qt5Widgets.lib
)

