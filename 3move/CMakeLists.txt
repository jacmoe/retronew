set(TARGET_NAME move)

set(Sources
    ./main.cpp
)

set(Includes
    ../${IncludeDir}/olcPixelGameEngine.h
)

add_executable(${TARGET_NAME}
    ${Sources}
    ${Includes}
)

target_link_libraries(${TARGET_NAME}
        Threads::Threads
        ${OPENGL_LIBRARIES}
        )

if (UNIX)
    target_link_libraries(${TARGET_NAME} stdc++fs)
    target_link_libraries(${TARGET_NAME} GL)
    target_link_libraries(${TARGET_NAME} png)

    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    set(SFML_STATIC_LIBRARIES FALSE)
    target_link_libraries(${TARGET_NAME}
            ${X11_LIBRARIES}
            PNG::PNG
            )
endif (UNIX)
if (WIN32)
    target_link_libraries(${TARGET_NAME}
            ${WinSDK}
            )
endif (WIN32)
