# BDD package library
set(LIB_HEADERS
  ManagerInterface.h
  Manager.h
)
set(LIB_SOURCES
  ManagerInterface.cpp
  Manager.cpp
)
set(APP_SOURCES
  main.cpp
)

# Define library
set(LIBRARY_NAME bddPkg)
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})

# Set lib properties
set_target_properties(
  ${LIBRARY_NAME}
  PROPERTIES
     VERSION ${PKG_VERSION}
     SUFFIX ".a"
     ARCHIVE_OUTPUT_DIRECTORY "../build/lib"
     LIBRARY_OUTPUT_DIRECTORY "../build/lib"
)

# Define application
set(APPLICATION_NAME bddPkgMain)
add_executable(${APPLICATION_NAME} ${APP_SOURCES})
# Set app suffix to .bin
set_target_properties (
   ${APPLICATION_NAME}
   PROPERTIES
      VERSION ${PKG_VERSION}
      SUFFIX ".bin"
      RUNTIME_OUTPUT_DIRECTORY "../build/bin"
)

# Links the target exe (bddpkgMain) against the library (bddPkg)
target_link_libraries(${APPLICATION_NAME} ${LIBRARY_NAME})
