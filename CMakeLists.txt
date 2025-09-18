cmake_minimum_required(VERSION 3.10)
project(BladeOfTheForest)

# Usamos C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Carpeta de includes propios
include_directories(${CMAKE_SOURCE_DIR}/include)

# Archivos fuente
file(GLOB SOURCES "src/*.cpp")

# Crear ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})

# -----------------------------
# Detectar sistema operativo
# -----------------------------
if(WIN32)
    # Windows (asume SDL2 y SDL2_image en include/ y lib/)
    include_directories("C:/SDL2/include")
    link_directories("C:/SDL2/lib")
    target_link_libraries(${PROJECT_NAME} SDL2 SDL2main SDL2_image)
elseif(UNIX)
    # Linux/Ubuntu
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_image)
endif()

# -----------------------------
# Copiar assets automáticamente al directorio de compilación
# -----------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
