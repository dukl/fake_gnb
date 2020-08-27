include_directories(${SRC_TOP_DIR}/common)
include_directories(${SRC_TOP_DIR}/../build/ext/spdlog/include)
add_library( LOG STATIC
  ${SRC_TOP_DIR}/common/logger.cpp
)
