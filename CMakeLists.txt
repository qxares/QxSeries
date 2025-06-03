cmake_minimum_required(VERSION 3.10)
project(QxCentreDE C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(WNCK REQUIRED libwnck-3.0)
pkg_check_modules(NOTIFY REQUIRED libnotify)

include_directories(${GTK3_INCLUDE_DIRS} ${WNCK_INCLUDE_DIRS} ${NOTIFY_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS} ${WNCK_LIBRARY_DIRS} ${NOTIFY_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER} ${WNCK_CFLAGS_OTHER} ${NOTIFY_CFLAGS_OTHER})

add_executable(QxCentreDE
    src/main.c
    src/bricks/taskbarbrick.c
    src/bricks/taskbarguibrick.c
    src/bricks/taskbarhandlerbrick.c
    src/bricks/mainwindowbrick.c
)

target_link_libraries(QxCentreDE ${GTK3_LIBRARIES} ${WNCK_LIBRARIES} ${NOTIFY_LIBRARIES})
