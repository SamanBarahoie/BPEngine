cmake_minimum_required(VERSION 3.14)
project(tokenizer)

# تنظیمات C++ استاندارد
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# مسیر پایتون
set(PYTHON_INCLUDE_DIR "C:/Users/HPOMEN/AppData/Local/Programs/Python/Python313/include")
set(PYTHON_LIBRARY "C:/Users/HPOMEN/AppData/Local/Programs/Python/Python313/libs/python313.lib")

# اضافه کردن مسیرهای هدر و کتابخانه‌ها
include_directories(${PYTHON_INCLUDE_DIR})
link_directories("C:/Users/HPOMEN/AppData/Local/Programs/Python/Python313/libs")

# pybind11 از گیت
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.11.1
)
FetchContent_MakeAvailable(pybind11)

# اضافه کردن مسیر هدرها (مثلاً nlohmann)
include_directories(${PROJECT_SOURCE_DIR}/extern)

# فایل‌های cpp
file(GLOB SRC_FILES src/*.cpp)

# ساخت لایبرری tokenizer
add_library(tokenizer STATIC ${SRC_FILES})

# ساخت ماژول پایتونی با pybind11
pybind11_add_module(pytokenizer src/bindings.cpp)

# لینک کردن tokenizer با pytokenizer و پایتون
target_link_libraries(pytokenizer PRIVATE tokenizer ${PYTHON_LIBRARY})
