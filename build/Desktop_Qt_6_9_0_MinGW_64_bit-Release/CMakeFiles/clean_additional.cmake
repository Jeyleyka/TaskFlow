# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\todo_app_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\todo_app_autogen.dir\\ParseCache.txt"
  "todo_app_autogen"
  )
endif()
