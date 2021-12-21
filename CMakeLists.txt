# CMakeの最小バージョン
cmake_minimum_required (VERSION 3.8)

# C++23を要求
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# プロジェクトを追加
project ("DesignPatterns")

# サブプロジェクトを含める
add_subdirectory ("EventDispatcher")
add_subdirectory ("Pimpl")
add_subdirectory ("Strategy")