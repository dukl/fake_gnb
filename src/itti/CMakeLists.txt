cmake_minimum_required (VERSION 3.0.2)

project(OpenAirInterface)

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
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS} -Os -s -DTRACE_IS_ON=1 -DDEBUG_IS_ON=1 -DINFO_IS_ON=1")

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
include_directories(${SRC_TOP_DIR}/amf-app)
include_directories(${SRC_TOP_DIR}/../build/ext/spdlog/include)

### for common
include_directories(${SRC_TOP_DIR}/common)
file(GLOB COMMON_SRC ${SRC_TOP_DIR}/common/conversions.cpp)
### end for common

### for utils/bstr
include_directories(${SRC_TOP_DIR}/utils/bstr)
include_directories(${SRC_TOP_DIR}/utils)
file(GLOB BSTR_SRC 
  ${SRC_TOP_DIR}/utils/bstr/bstrlib.c
  ${SRC_TOP_DIR}/utils/thread_sched.cpp
)
##end for utils/bstr

### for sctp
include_directories(${SRC_TOP_DIR}/sctp)
file(GLOB SCTP_SRC ${SRC_TOP_DIR}/sctp/sctp_server.cpp)
#end for sctp

### for ngap
include_directories(${SRC_TOP_DIR}/ngap/ngap_app)
file(GLOB NGAP_SRC ${SRC_TOP_DIR}/ngap/ngap_app/ngap_app.cpp)
###end for ngap

### for itti
include_directories(${SRC_TOP_DIR}/itti)
file(GLOB ITTI_SRC 
  ${SRC_TOP_DIR}/itti/itti.cpp
  ${SRC_TOP_DIR}/itti/itti_msg.cpp
)
###end for itti

###  for log
add_library( LOG STATIC
  ${SRC_TOP_DIR}/common/logger.cpp)
add_library( OPTIONS STATIC
  ${SRC_TOP_DIR}/oai-amf/options.cpp)
###end for log

### for config
add_library( CONFIGS STATIC
  ${SRC_TOP_DIR}/amf-app/amf_config.cpp)

pkg_search_module(CONFIG REQUIRED libconfig++)
include_directories(${CONFIG_INCLUDE_DIRS})
### end for config

add_executable(amf
	#${SRC_TOP_DIR}/oai-amf/main.cpp 
	#${SRC_TOP_DIR}/ngap/main.cpp 
	${SRC_TOP_DIR}/itti/main.cpp 
        ${COMMON_SRC} ${BSTR_SRC} ${SCTP_SRC} ${NGAP_SRC} ${ITTI_SRC}	
  )

# LOG from oai-amf/CMakeLists.txt
target_link_libraries(amf -Wl,--start-group LOG OPTIONS CONFIGS config++ sctp pthread)
