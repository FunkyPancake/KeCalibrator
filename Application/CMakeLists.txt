cmake_minimum_required(VERSION 3.13)


project(Application LANGUAGES CXX C)


if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_sources(${TARGET_NAME}
        PRIVATE
        App.cpp
        main.cpp
        )

    target_include_directories(
        ${TARGET_NAME}
        PUBLIC
        .
    )
elseif (CMAKE_BUILD_TYPE MATCHES Test)

endif ()


