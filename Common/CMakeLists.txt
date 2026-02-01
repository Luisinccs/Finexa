cmake_minimum_required(VERSION 3.15)
project(FinexaCommon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES 
    "Core/src/*.cpp" 
    "Core/include/*.h"
    "ViewModels/src/*.cpp"
    "ViewModels/include/*.h"
)

add_library(FinexaCommon STATIC ${SOURCES})

# Definir la ruta al deploy de DcViewModels (Ajustar segun necesisdad)
set(DC_VM_DEPLOY "${CMAKE_CURRENT_SOURCE_DIR}/../../Deploys/DcViewModels/web")

target_include_directories(FinexaCommon PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ViewModels/include>
    $<BUILD_INTERFACE:${DC_VM_DEPLOY}/include>
)
