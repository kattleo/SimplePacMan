# ASCII GFX library
# to be included in CMakeLists.txt like so:
# INCLUDE(ascii-gfx/ASCII-GFX.cmake)
SET(GFXLIB_NAME asciigfx)
SET(GFXLIB_DIR ${CMAKE_CURRENT_LIST_DIR}/gfx)
SET(GFXLIB_HDRS
   ${GFXLIB_DIR}/gfx.h
   ${GFXLIB_DIR}/math2d.h
   ${GFXLIB_DIR}/scrollarea.h
   ${GFXLIB_DIR}/gridfont.h
   ${GFXLIB_DIR}/gridarea.h
   ${GFXLIB_DIR}/gridmenu.h
   ${GFXLIB_DIR}/polygon.h
   ${GFXLIB_DIR}/sound.h
   ${GFXLIB_DIR}/joystick.h
   ${GFXLIB_DIR}/mouse.h
   ${GFXLIB_DIR}/util.h
   )
SET(GFXLIB_SRCS
   ${GFXLIB_DIR}/gfx.cpp
   ${GFXLIB_DIR}/math2d.cpp
   ${GFXLIB_DIR}/scrollarea.cpp
   ${GFXLIB_DIR}/gridfont.cpp
   ${GFXLIB_DIR}/gridarea.cpp
   ${GFXLIB_DIR}/gridmenu.cpp
   ${GFXLIB_DIR}/polygon.cpp
   ${GFXLIB_DIR}/sound.cpp
   ${GFXLIB_DIR}/joystick.cpp
   ${GFXLIB_DIR}/mouse.cpp
   ${GFXLIB_DIR}/util.cpp
   )
INCLUDE_DIRECTORIES(${GFXLIB_DIR})
ADD_LIBRARY(${GFXLIB_NAME} ${GFXLIB_SRCS} ${GFXLIB_HDRS})
