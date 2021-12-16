# CMakeList.txt : 最上位の CMake プロジェクト ファイル。グローバル構成を行います
# また、サブプロジェクトをここに含めます。
#
cmake_minimum_required (VERSION 3.8)

# C++23を要求
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project ("DesignPatterns")

# サブプロジェクトを含めます。
add_subdirectory ("EventDispatcher")
add_subdirectory ("Strategy")