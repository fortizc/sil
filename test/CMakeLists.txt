add_executable(basic_getters basic_getters.c)
add_executable(zero_new zero_new.c)
add_executable(get_set_pixel get_set_pixel.c)

target_link_libraries(zero_new sil)
target_link_libraries(basic_getters sil)
target_link_libraries(get_set_pixel sil)
