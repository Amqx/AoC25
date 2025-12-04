cmake_minimum_required(VERSION 4.0)
set(DAY 4)

project("day${DAY}")

set(CMAKE_CXX_STANDARD 20)

add_executable("day${DAY}p1"
        "day${DAY}p1.cpp"
)

add_executable("day${DAY}p2"
        "day${DAY}p2.cpp"
)
