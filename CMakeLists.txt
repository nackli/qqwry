

# @author nackli <nackli@163.com>
# @version 1.0
# @copyright 2025 Your Company Name. All rights reserved.
# @License: MIT (https://opensource.org/licenses/MIT)
# @Created: 2025-03-20
#


#version
cmake_minimum_required(VERSION 3.16)
# project name
project(qqWryDemo VERSION 1.0.0)
set(BUILD_NUMBER "" CACHE STRING "Build number")
# set C++ standard
set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

if(MSVC) 
	set(CMAKE_GENERATOR_PLATFORM x64)
  add_compile_options(/W3 /utf-8 /bigobj /MP)
	#pdb 
	#set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /DEBUG /OPT:REF /OPT:ICF")
	#set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
else()
	 set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall -O3 -fPIC")
	 set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -fPIC -g")
endif()	


#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -ggdb3")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
# set msvc info
macro(add_msvc_options target)
  if(MSVC)
    target_compile_options(${target} PRIVATE
    /W4                # 设置警告级别为 4
    /WX                # 将警告视为错误
    /MP                # 启用多处理器编译
    /permissive-       # 禁用不严格的语言 conformance
    /Zc:__cplusplus    # 启用正确的 __cplusplus 宏值
    /Zc:inline         # 移除未使用的函数
    /Gm-               # 禁用最小生成（minimal rebuild）
    #/EHsc              # 指定异常处理模型
    )
  endif()
endmacro()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")


if(MSVC)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "qqWryDemo")
endif()


file(GLOB DEV_SOURCES_EXE ./*.cpp)

add_executable(${PROJECT_NAME} ${DEV_SOURCES_EXE})
set_target_properties(qqWryDemo PROPERTIES LINKER_LANGUAGE CXX)

include(BuildPage)
use_pack_fun(${PROJECT_NAME} "ZIP")