set(APP_NAME disfs)

add_executable(${APP_NAME} src/main.c)
target_link_libraries(${APP_NAME} PUBLIC disfslib)
