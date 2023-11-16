cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0057 NEW)
cmake_policy(SET CMP0077 NEW)

project(guix_test LANGUAGES C)

# Set build configurations
set(BUILD_CONFIGURATIONS
    default_build_coverage
    disable_error_check_build
    no_utf8_build_coverage
    no_utf8_no_checking_build
    ex_unicode_build
    ex_unicode_no_checking_build
    mouse_support_build
    font_kerning_support_build
    dynamic_bidi_text_build
    dynamic_bidi_text_no_checking_build
    _5_4_0_compatible_no_checking_build
    synergy_font_support_build
    thai_glyph_shaping_support_build
    palette_mode_aa_text_colors_16_build
    disable_deprecated_string_api_build
    partial_canvas_support_build
    partial_canvas_support_vertical_refresh_build
    partial_canvas_support_horizontal_refresh_build)
    
set(CMAKE_CONFIGURATION_TYPES
    ${BUILD_CONFIGURATIONS}
    CACHE STRING "list of supported configuration types" FORCE)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
                                             ${CMAKE_CONFIGURATION_TYPES})
list(GET CMAKE_CONFIGURATION_TYPES 0 BUILD_TYPE)
if((NOT CMAKE_BUILD_TYPE) OR (NOT ("${CMAKE_BUILD_TYPE}" IN_LIST
                                   CMAKE_CONFIGURATION_TYPES)))
  set(CMAKE_BUILD_TYPE
      "${BUILD_TYPE}"
      CACHE STRING "Build Type of the project" FORCE)
endif()

message(STATUS "Build for guix")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}.")

set(default_build_coverage "")
set(disable_error_checking_build -DGX_DISABLE_ERROR_CHECKING)
set(no_utf8_build_coverage -DGX_DISABLE_UTF8_SUPPORT)
set(no_utf8_no_checking_build -DGX_DISABLE_UTF8_SUPPORT
                              -DGX_DISABLE_ERROR_CHECKING)
set(ex_unicode_build -DGX_EXTENDED_UNICODE_SUPPORT)
set(ex_unicode_no_checking_build -DGX_EXTENDED_UNICODE_SUPPORT
                                 -DGX_DISABLE_ERROR_CHECKING)
set(mouse_support_build -DGX_MOUSE_SUPPORT)
set(font_kerning_support_build -DGX_FONT_KERNING_SUPPORT)
set(dynamic_bidi_text_build -DGX_DYNAMIC_BIDI_TEXT_SUPPORT
                            -DGX_DYNAMIC_ARABIC_SHAPING_SUPPORT)
set(dynamic_bidi_text_no_checking_build
    -DGX_DYNAMIC_BIDI_TEXT_SUPPORT -DGX_DYNAMIC_ARABIC_SHAPING_SUPPORT
    -DGX_DISABLE_ERROR_CHECKING)
set(_5_4_0_compatible_no_checking_build -DGX_DISABLED_ERROR_CHECKING
                                        -DGUIX_5_4_0_COMPATIBILITY)
set(synergy_font_support_build -DGX_SYNERGY_FONT_FORMAT_SUPPORT)
set(thai_glyph_shaping_support_build -DGX_THAI_GLYPH_SHAPING_SUPPORT)
set(palette_mode_aa_text_colors_16_build -DGX_PALETTE_MODE_AA_TEXT_COLORS=16)
set(disable_deprecated_string_api_build -DGX_DISABLE_DEPRECATED_STRING_API)
set(partial_canvas_support_build -DGX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER)
set(partial_canvas_support_vertical_refresh_build -DGX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER -DGX_CANVAS_REFRESH_DIRECTION_HORIZONTAL)
set(partial_canvas_support_horizontal_refresh_build -DGX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER -DGX_CANVAS_REFRESH_DIRECTION_VERTICAL)

if($ENV{ENABLE_64})
  message(STATUS "Building for 64bit")
else()
  add_compile_options(-m32)
  add_link_options(-m32)
  message(STATUS "Building for 32bit")
endif()
add_compile_options(
  -std=c99
  -ggdb
  -g3
  -gdwarf-2
  -fdiagnostics-color
  -DWQVGA_272_SCREEN
  -DCOLOR_DEPTH_16BPP
  -DGX_CURSOR_BLINK_INTERVAL=1
  -DGX_TARGET_LINUX
  -DGX_CUSTOM_FONTS=16
  -DGX_CUSTOM_PIXELMAPS=32
  -Dwin32_graphics_driver_setup_24bpp=gx_validation_display_driver_setup_24bpp
  -Dwin32_graphics_driver_setup_565rgb=gx_validation_display_driver_setup_565rgb
  -Dwin32_graphics_driver_setup_565rgb_rotated=gx_validation_display_driver_setup_565rgb_rotated
  -Dwin32_graphics_driver_setup_565bgr=gx_validation_display_driver_setup_565rgb
  -Dwin32_graphics_driver_setup_1555xrgb=gx_validation_display_driver_setup_1555xrgb
  -Dwin32_graphics_driver_setup_24xrgb=gx_validation_display_driver_setup_24xrgb
  -Dwin32_graphics_driver_setup_24xrgb_rotated=gx_validation_display_driver_setup_24xrgb_rotated
  -Dwin32_graphics_driver_setup_32argb=gx_validation_display_driver_setup_32argb
  -Dwin32_graphics_driver_setup_32argb_rotated=gx_validation_display_driver_setup_32argb_rotated
  -Dwin32_graphics_driver_setup_332rgb=gx_validation_display_driver_setup_332rgb
  -Dwin32_graphics_driver_setup_4444argb=gx_validation_display_driver_setup_4444argb
  -Dwin32_graphics_driver_setup_4bpp_grayscale=gx_validation_display_driver_setup_4bpp_grayscale
  -Dwin32_graphics_driver_setup_8bit_palette=gx_validation_display_driver_setup_8bit_palette
  -Dwin32_graphics_driver_setup_8bit_palette_rotated=gx_validation_display_driver_setup_8bit_palette_rotated
  -Dwin32_graphics_driver_setup_monochrome=gx_validation_display_driver_setup_monochrome
  -Dwin32_dave2d_graphics_driver_setup_24xrgb=gx_validation_dave2d_display_driver_setup_24xrgb
  -Dwin32_dave2d_graphics_driver_setup_565rgb=gx_validation_dave2d_display_driver_setup_565rgb
  -Dwin32_dave2d_graphics_driver_setup_565rgb_rotated=gx_validation_dave2d_display_driver_setup_565rgb_rotated
  -Dwin32_dave2d_graphics_driver_setup_8bit_palette_rotated=gx_validation_dave2d_display_driver_setup_8bit_palette_rotated
  -Dwin32_dave2d_graphics_driver_setup_24xrgb_rotated=gx_validation_dave2d_display_driver_setup_24xrgb_rotated
  ${${CMAKE_BUILD_TYPE}})

enable_testing()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../ guix)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/regression regression)

# Create output files
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/regression/output_files)

# Coverage
if(CMAKE_BUILD_TYPE MATCHES ".*_coverage")
  target_compile_options(guix PRIVATE -fprofile-arcs -ftest-coverage)
  target_link_options(guix PRIVATE -fprofile-arcs -ftest-coverage)
endif()

# Build ThreadX library once
execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run.sh build_libs)
add_custom_target(build_libs ALL COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run.sh
                                         build_libs)
add_dependencies(guix build_libs)
target_include_directories(guix PUBLIC ${CMAKE_BINARY_DIR}/../libs/inc)
add_library(threadx SHARED IMPORTED GLOBAL)
add_library("azrtos::threadx" ALIAS threadx)
set_target_properties(
  threadx PROPERTIES IMPORTED_LOCATION
                     ${CMAKE_BINARY_DIR}/../libs/threadx/libthreadx.so)

target_compile_options(
  guix
  PRIVATE -Werror
          -Wall
          -Wextra
          -pedantic
          -fmessage-length=0
          -fsigned-char
          -ffunction-sections
          -fdata-sections
          -Wunused
          -Wuninitialized
          -Wmissing-declarations
          -Wconversion
          -Wpointer-arith
          -Wshadow
          -Wlogical-op
          -Waggregate-return
          -Wfloat-equal)
