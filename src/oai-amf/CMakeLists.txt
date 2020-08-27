cmake_minimum_required (VERSION 3.0.2)

project(OpenAirInterface)

macro(add_boolean_option name val helpstr)
  if(DEFINED ${name})
    set(value ${${name}})
  else(DEFINED ${name})
    set(value ${val})
  endif()
  set(${name} ${value} CACHE STRING "${helpstr}")
  set_property(CACHE ${name} PROPERTY TYPE BOOL)
  if (${value})
    add_definitions("-D${name}=1")
  else (${value})
    add_definitions("-D${name}=0")
  endif (${value})
endmacro(add_boolean_option)

macro(add_list_string_option name val helpstr)
  if(DEFINED ${name})
    set(value ${${name}})
  else(DEFINED ${name})
    set(value ${val})
  endif()
  set(${name} ${value} CACHE STRING "${helpstr}")
  set_property(CACHE ${name} PROPERTY STRINGS ${ARGN})
  if(NOT "${value}" STREQUAL "False")
    add_definitions("-D${name}=\"${value}\"")
  endif()
endmacro(add_list_string_option)

SET(STATIC_LINKING FALSE CACHE BOOL "Build a static binary?")

IF(STATIC_LINKING)
  SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
ENDIF(STATIC_LINKING)

add_list_string_option(CMAKE_BUILD_TYPE "RelWithDebInfo" "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." Debug Release
	RelWithDebInfo MinSizeRel)
Message("Build type is ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  add_boolean_option(LOG_OAI True         "Thread safe logging API")
  add_boolean_option(LOG_OAI_MINIMAL True "Thread safe logging API, log only levels above NOTICE")
  SET(ASAN asan)
endif()
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_boolean_option(LOG_OAI True "Thread safe logging API")
  SET(ASAN asan)
endif()
if (CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "MinSizeRel")
endif()
Message("Build type is ${CMAKE_BUILD_TYPE}")

Message("Architecture is ${CMAKE_SYSTEM_PROCESSOR}")
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7l")
  set(C_FLAGS_PROCESSOR "-gdwarf-2 -mfloat-abi=hard -mfpu=neon -lgcc -lrt")
else (CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7l")
  set(C_FLAGS_PROCESSOR "-msse4.2")
endif()

set(CMAKE_C_FLAGS
	  "${CMAKE_C_FLAGS} ${C_FLAGS_PROCESSOR} -std=gnu99 -Wall -Wstrict-prototypes -fno-strict-aliasing -rdynamic -funroll-loops -Wno-packed-bitfield-compat -fPIC ")
  # add autoTOOLS definitions that were maybe used!
set(CMAKE_C_FLAGS
	    "${CMAKE_C_FLAGS} -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_FCNTL_H=1 -DHAVE_ARPA_INET_H=1 -DHAVE_SYS_TIME_H=1 -DHAVE_SYS_SOCKET_H=1 -DHAVE_STRERROR=1 -DHAVE_SOCKET=1 -DHAVE_MEMSET=1 -DHAVE_GETTIMEOFDAY=1 -DHAVE_STDLIB_H=1 -DHAVE_MALLOC=1 -DHAVE_LIBSCTP")

add_definitions(-DCMAKER)
add_definitions(-DBSTRLIB_CAN_USE_STL=1 -DBSTRLIB_CAN_USE_IOSTREAM=1 -DBSTRLIB_THROWS_EXCEPTIONS=1)

if(STATIC_LINKING)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -fstack-protector-all -DMALLOC_CHECK_=3 -DINFO_IS_ON=1 -DDEBUG_IS_ON=1 -DTRACE_IS_ON=1 -O0  -fno-omit-frame-pointer")
else (STATIC_LINKING)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -fstack-protector-all -DMALLOC_CHECK_=3 -DINFO_IS_ON=1 -DDEBUG_IS_ON=1 -DTRACE_IS_ON=1 -O0 -fsanitize=address -fno-omit-frame-pointer")
endif(STATIC_LINKING)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2 -fno-omit-frame-pointer -s -DINFO_IS_ON=1 ")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS} -g  -O2 -DINFO_IS_ON=1 ")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS} -Os -s -DTRACE_IS_ON=1 -DDEBUG_IS_ON=1 -DINFO_IS_ON=1 -DAUTH_ALG_ON=0")

if(STATIC_LINKING)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++17  -g -fstack-protector-all -DMALLOC_CHECK_=3 -DINFO_IS_ON=1 -DDEBUG_IS_ON=1 -DTRACE_IS_ON=1 -O0 -fno-omit-frame-pointer -DBUPT=1 -DEURECOM=0")
else(STATIC_LINKING)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++17  -g -fstack-protector-all -DMALLOC_CHECK_=3 -DINFO_IS_ON=1 -DDEBUG_IS_ON=1 -DTRACE_IS_ON=1 -O0 -fsanitize=address -fno-omit-frame-pointer -DBUPT=1 -DEURECOM=0")
endif(STATIC_LINKING)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++17  -O2 -fno-omit-frame-pointer -s -DINFO_IS_ON=1 -DBUPT=1 -DEURECOM=0")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -std=c++17 -g -O2 -DINFO_IS_ON=1 -DBUPT=1 -DEURECOM=0")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS} -std=c++17 -Os -s -DTRACE_IS_ON=1 -DDEBUG_IS_ON=1 -DINFO_IS_ON=1")

set(CMAKE_MODULE_PATH "${OPENAIRCN_DIR}/build/cmake_modules" "${CMAKE_MODULE_PATH}")
include(FindPkgConfig)
message("${OPENAIRCN_DIR}/build/cmake_modules")


include_directories(${SRC_TOP_DIR}/oai-amf)
include_directories(${SRC_TOP_DIR}/../build/ext/spdlog/include)

### for itti msg
include_directories(${SRC_TOP_DIR}/itti/msgs)
###end for itti msg

### for amf nas
include_directories(${SRC_TOP_DIR}/nas)
file(GLOB AMF_NAS_SRC
  ${SRC_TOP_DIR}/nas/nas_context.cpp
)
###end for
### for libnas
include_directories(${SRC_TOP_DIR}/nas/common)
include_directories(${SRC_TOP_DIR}/nas/common)
include_directories(${SRC_TOP_DIR}/nas/ies)
include_directories(${SRC_TOP_DIR}/nas/msgs)
include_directories(${SRC_TOP_DIR}/nas/utils)
file(GLOB NAS_SRC
  ${SRC_TOP_DIR}/nas/common/*.cpp
  ${SRC_TOP_DIR}/nas/ies/*.cpp
  ${SRC_TOP_DIR}/nas/msgs/*.cpp
  ${SRC_TOP_DIR}/nas/utils/*.cpp
  ${SRC_TOP_DIR}/nas/utils/*.c
)
###end for

### for amf_app
include_directories(${SRC_TOP_DIR}/amf-app)
file(GLOB AMF_APP_SRC 
  ${SRC_TOP_DIR}/amf-app/amf_app.cpp
  ${SRC_TOP_DIR}/amf-app/amf_n2.cpp
  ${SRC_TOP_DIR}/amf-app/amf_n1.cpp
  ${SRC_TOP_DIR}/amf-app/amf_n11.cpp
  ${SRC_TOP_DIR}/amf-app/amf_statistics.cpp
)
### end for amf_app

### for common
include_directories(${SRC_TOP_DIR}/common)
include_directories(${SRC_TOP_DIR}/common/unicode)
file(GLOB COMMON_SRC ${SRC_TOP_DIR}/common/conversions.cpp)
### end for common

### for utils/bstr
include_directories(${SRC_TOP_DIR}/utils/bstr)
include_directories(${SRC_TOP_DIR}/utils)
file(GLOB MULTI_PARSER_SRC
  ${SRC_TOP_DIR}/utils/multipartparser.cpp
  ${SRC_TOP_DIR}/utils/http_multi_parser.cpp
)
file(GLOB BSTR_SRC 
  ${SRC_TOP_DIR}/utils/bstr/bstrlib.c
  ${SRC_TOP_DIR}/utils/thread_sched.cpp
)
file(GLOB UTILS_SRC
  ${SRC_TOP_DIR}/utils/string.cpp
  ${SRC_TOP_DIR}/utils/if.cpp
  ${SRC_TOP_DIR}/utils/backtrace.c
  ${SRC_TOP_DIR}/utils/hex_string_convert.cpp
)
##end for utils/bstr

### for sctp
include_directories(${SRC_TOP_DIR}/sctp)
file(GLOB SCTP_SRC ${SRC_TOP_DIR}/sctp/sctp_server.cpp)
#end for sctp

### for ngap
include_directories(${SRC_TOP_DIR}/ngap/ngap_app)
file(GLOB NGAP_SRC 
  ${SRC_TOP_DIR}/ngap/ngap_app/ngap_app.cpp
)
###end for ngap

### for generated libngap
include_directories(${SRC_TOP_DIR}/ngap/libngap)
file(GLOB LIB_NGAP_SRC ${SRC_TOP_DIR}/ngap/libngap/*.c)
###end

### for encapsulated ngap message
include_directories(${SRC_TOP_DIR}/ngap/ngapIEs)
include_directories(${SRC_TOP_DIR}/ngap/ngapMsgs)
file(GLOB NGAP_MSGS_IES_SRC 
  ${SRC_TOP_DIR}/ngap/ngapIEs/*.cpp
  ${SRC_TOP_DIR}/ngap/ngapMsgs/*.cpp
)
### end for

### for itti
include_directories(${SRC_TOP_DIR}/itti)
file(GLOB ITTI_SRC 
  ${SRC_TOP_DIR}/itti/itti.cpp
  ${SRC_TOP_DIR}/itti/itti_msg.cpp
)
###end for itti

### for authentication 
include_directories(${SRC_TOP_DIR}/secu_algorithms/5gaka)
include_directories(${SRC_TOP_DIR}/secu_algorithms/5gaka/test)
include_directories(${SRC_TOP_DIR}/secu_algorithms/5gaka/algorithm_from_spec)
file(GLOB 5GAKA_SRC ${SRC_TOP_DIR}/secu_algorithms/5gaka/*.cpp)
###end for

### for nas algorithms
include_directories(${SRC_TOP_DIR}/secu_algorithms/nas_enc_int)
file(GLOB NAS_ALG_SRC 
  ${SRC_TOP_DIR}/secu_algorithms/nas_enc_int/*.cpp
  ${SRC_TOP_DIR}/secu_algorithms/nas_enc_int/*.c
)
###end

###  for log
add_library( LOG STATIC
  ${SRC_TOP_DIR}/common/logger.cpp)
add_library( OPTIONS STATIC
  ${SRC_TOP_DIR}/oai-amf/options.cpp)
###end for log

### for config
add_library( CONFIGS STATIC
  ${SRC_TOP_DIR}/amf-app/amf_config.cpp
  ${SRC_TOP_DIR}/amf-app/amf_module_from_config.cpp
)

pkg_search_module(CONFIG REQUIRED libconfig++)
include_directories(${CONFIG_INCLUDE_DIRS})
### end for config

### for context
include_directories(${SRC_TOP_DIR}/contexts)
file(GLOB CTXS_SRC ${SRC_TOP_DIR}/contexts/*.cpp)
###end 

###for N11(smf client)
include_directories(${SRC_TOP_DIR}/sbi/smf_client)
include_directories(${SRC_TOP_DIR}/sbi/smf_client/api)
include_directories(${SRC_TOP_DIR}/sbi/smf_client/model)
file(GLOB SMF_CLIENT_SRC 
  ${SRC_TOP_DIR}/sbi/smf_client/*.cpp
  ${SRC_TOP_DIR}/sbi/smf_client/api/*.cpp
  ${SRC_TOP_DIR}/sbi/smf_client/model/*.cpp
)
###end

###for amf server
include_directories(${SRC_TOP_DIR}/sbi/amf_server)
include_directories(${SRC_TOP_DIR}/sbi/amf_server/api)
include_directories(${SRC_TOP_DIR}/sbi/amf_server/impl)
include_directories(${SRC_TOP_DIR}/sbi/amf_server/model)
file(GLOB AMF_SERVER_SRC 
  ${SRC_TOP_DIR}/sbi/amf_server/*.cpp
  ${SRC_TOP_DIR}/sbi/amf_server/api/*.cpp
  ${SRC_TOP_DIR}/sbi/amf_server/impl/*.cpp
  ${SRC_TOP_DIR}/sbi/amf_server/model/*.cpp
)
###end

### for mysqp handlers
file(GLOB MYSQL_HANDLERS_SRC 
  ${SRC_TOP_DIR}/amf-app/mysql_db.cpp
)
###end

### for sha-256 algorithms
### end

### for ue_gnb_simu
file(GLOB UE_GNB_SRC 
  ${SRC_TOP_DIR}/test/ue_gnb.cpp
)


pkg_search_module(CRYPTO libcrypto REQUIRED)
include_directories(${CRYPTO_INCLUDE_DIRS})

pkg_search_module(OPENSSL openssl REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})

pkg_search_module(NETTLE nettle)
if(NOT ${NETTLE_FOUND})
  message("PACKAGE nettle not found: some targets will fail")
else()
  include_directories(${NETTLE_INCLUDE_DIRS})
endif()

pkg_search_module(NETTLE nettle)
if(NOT ${NETTLE_FOUND})
  message( FATAL_ERROR "PACKAGE nettle not found: some targets will fail. Run SCRIPTS/build_mme -i or SCRIPTS/build_hss -i  again!")
else()
  include_directories(${NETTLE_INCLUDE_DIRS})
endif()

message ("NETTLE VERSION_INSTALLED  = ${NETTLE_VERSION}")

string(REGEX REPLACE "([0-9]+).*" "\\1" NETTLE_VERSION_MAJOR ${NETTLE_VERSION})
string(REGEX REPLACE "[0-9]+\\.([0-9]+).*" "\\1" NETTLE_VERSION_MINOR ${NETTLE_VERSION})
message ("NETTLE_VERSION_MAJOR = ${NETTLE_VERSION_MAJOR}")
message ("NETTLE_VERSION_MINOR = ${NETTLE_VERSION_MINOR}")

if ("${NETTLE_VERSION_MAJOR}" STREQUAL "" OR "${NETTLE_VERSION_MINOR}" STREQUAL "")
  message( FATAL_ERROR "The nettle version not detected properly. Try to run SCRIPTS/build_mme -i or SCRIPTS/build_hss -i again" )
endif()

add_definitions("-DNETTLE_VERSION_MAJOR=${NETTLE_VERSION_MAJOR}")
add_definitions("-DNETTLE_VERSION_MINOR=${NETTLE_VERSION_MINOR}")

INCLUDE(FindMySQL)
IF( NOT MySQL_FOUND )
  MESSAGE( SEND_ERROR "MySQL Client is required" )
ENDIF( NOT MySQL_FOUND )

add_executable(amf
	${SRC_TOP_DIR}/oai-amf/main.cpp 
        ${COMMON_SRC} ${BSTR_SRC} ${SCTP_SRC} ${NGAP_SRC} ${ITTI_SRC} ${AMF_APP_SRC} ${UTILS_SRC} ${LIB_NGAP_SRC} ${AMF_NAS_SRC}	
        ${NGAP_MSGS_IES_SRC} ${CTXS_SRC} ${5GAKA_SRC} ${NAS_SRC} ${SMF_CLIENT_SRC} 
        ${MYSQL_HANDLERS_SRC} ${NAS_ALG_SRC} ${AMF_SERVER_SRC} ${UE_GNB_SRC} ${MULTI_PARSER_SRC} 
)

# LOG from oai-amf/CMakeLists.txt
target_link_libraries(amf -Wl,--start-group LOG OPTIONS CONFIGS config++ sctp pthread ${NETTLE_LIBRARIES} ${MySQL_LIBRARY} ${CRYPTO_LIBRARIES} ${OPENSSL_LIBRARIES} boost_system ssl crypt cpprest gmp pistache curl)
