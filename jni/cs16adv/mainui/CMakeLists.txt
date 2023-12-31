#
# Copyright (c) 2015 Pavlo Lavrenenko
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

cmake_minimum_required(VERSION 2.6.0)
project (MAINUI)

if (APPLE)
	set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -stdlib=libc++")
elseif(MSVC)
	set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
else()
	set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -static-libstdc++")
endif()

if(PANDORA)
	add_definitions(-DPANDORA)
endif()

set (MAINUI_LIBRARY xashmenu)

file (GLOB MAINUI_SOURCES *.cpp legacy/*.cpp)
include_directories (../common ../engine ../pm_shared ../mainui)
add_library (${MAINUI_LIBRARY} SHARED ${MAINUI_SOURCES})

if(MSVC)
	add_definitions(
	-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE
		)
endif()

set_target_properties (${MAINUI_SHARED} PROPERTIES
	VERSION ${XASH3D_VERSION} SOVERSION ${XASH3D_VERSION}
	POSITION_INDEPENDENT_CODE 1)

if(NOT WIN32)
	set_target_properties(${MAINUI_LIBRARY} PROPERTIES
			    OUTPUT_NAME "xashmenu")
else()
	set_target_properties(${MAINUI_LIBRARY} PROPERTIES
			OUTPUT_NAME "menu")
endif()


