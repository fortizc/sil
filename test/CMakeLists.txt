add_executable(basic_getters basic_getters.c)
add_executable(zero_new zero_new.c)

target_link_libraries(zero_new sil)
target_link_libraries(basic_getters sil)
