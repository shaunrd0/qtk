################################################################################
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
##                                                                            ##
## CMake function to update git submodules                                    ##
################################################################################
include_guard()

find_package(Git)

# _PATH: Path to git submodule location that we want to update
# + submodule_update(extern/assimp)
function(submodule_update _PATH)
  if (NOT UPDATE_SUBMODULES)
    return()
  endif()

  if (NOT GIT_FOUND)
    message(FATAL_ERROR "[Qtk] Error: No git executable found")
  endif()

  execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init "${_PATH}"
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      RESULT_VARIABLE result
  )

  if (NOT result EQUAL 0)
    message(FATAL_ERROR "[Qtk] Error: Unable to update git submodule at ${_PATH}")
  endif()
endfunction()
