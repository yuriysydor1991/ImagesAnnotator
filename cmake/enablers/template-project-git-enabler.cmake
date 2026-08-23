cmake_minimum_required(VERSION 3.13)

find_package(Git REQUIRED)

# ---------------------------------------------------------------------------
# What the produced binaries are stamped with: project-global-decls.h.in reads
# PROJECT_GIT_COMMIT into project_decls::PROJECT_BUILD_COMMIT, which is the
# "git commit" line the version printer application writes out.
#
# Both questions below are asked of the source tree rather than of wherever the
# build directory happens to be. execute_process() runs its command in the
# binary directory when it is told no other one, so a build tree configured
# outside the checkout - cmake -B /somewhere/else - used to run git where there
# is no repository at all.
#
# The status of the command is read for the very same reason: the
# OUTPUT_VARIABLE of a command which failed is simply left unset, so the
# configure carried on and stamped an empty string into the binary, and the
# version printer said "git commit" with nothing after it. A failure is a
# legitimate outcome here - an exported tarball carries no repository, and a
# container which bind mounts the sources sees them owned by another user and
# refuses to read them until that directory is named as a safe one - but it is
# an outcome the configure has to say out loud instead of passing an empty line
# on to whoever reads the version. What git itself complained about is what is
# printed, since that message already names the way out of it.
# ---------------------------------------------------------------------------

# @brief What is stamped in when git could not answer. Anything but an empty
# string, so that a build which never reached its repository says so rather
# than printing a blank.
set(PROJECT_GIT_UNKNOWN "unknown")

# @brief Asks git the question given after DESCRIPTION about the source tree of
# this project and hands the answer back through OUT_VAR, or PROJECT_GIT_UNKNOWN
# together with the reason when git could not answer it.
function(template_project_git_ask OUT_VAR DESCRIPTION)
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" ${ARGN}
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_ANSWER
    ERROR_VARIABLE GIT_COMPLAINT
    RESULT_VARIABLE GIT_STATUS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
  )

  if (NOT GIT_STATUS EQUAL 0)
    if (GIT_COMPLAINT STREQUAL "")
      set(GIT_COMPLAINT "git exited with the status ${GIT_STATUS}")
    endif()

    message(STATUS "The ${DESCRIPTION} stays unknown: ${GIT_COMPLAINT}")

    set(GIT_ANSWER "${PROJECT_GIT_UNKNOWN}")
  elseif (GIT_ANSWER STREQUAL "")
    # git succeeded and had nothing to say, which is what a detached HEAD
    # answers about the branch name it is not on.
    message(STATUS "The ${DESCRIPTION} stays unknown: git has none to give here")

    set(GIT_ANSWER "${PROJECT_GIT_UNKNOWN}")
  endif()

  set(${OUT_VAR} "${GIT_ANSWER}" PARENT_SCOPE)
endfunction()

template_project_git_ask(
  PROJECT_GIT_COMMIT
  "commit hash the binaries are stamped with"
  rev-parse HEAD
)

template_project_git_ask(
  PROJECT_CURRENT_BRANCH_NAME
  "branch name the docker image tags are derived from"
  branch --show-current
)

message(STATUS "Project git commit: ${PROJECT_GIT_COMMIT}")

file(
  GLOB_RECURSE 
  ALLSOURCES 
  LIST_DIRECTORIES false 
  CONFIGURE_DEPENDS
  "src/*.h" "src/*.cpp"
)
