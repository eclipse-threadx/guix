CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
PROJECT(guix_demo_compile_test)
INCLUDE(DART)

ENABLE_TESTING()

#list test
SET(TEST_SLN_FILE_LIST "")
FILE(GLOB_RECURSE TEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../../../../samples/*.vcxproj")
LIST(APPEND TEST_SLN_FILE_LIST ${TEST_FILE})
FILE(GLOB_RECURSE TEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../../../../test/example_internal/*.vcxproj")
LIST(APPEND TEST_SLN_FILE_LIST ${TEST_FILE})
FILE(GLOB_RECURSE TEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../../../../tutorials/*.vcxproj")
LIST(APPEND TEST_SLN_FILE_LIST ${TEST_FILE})

#add test
FOREACH(TEST_FILE ${TEST_SLN_FILE_LIST})
    FILE(RELATIVE_PATH TEST_NAME "${CMAKE_CURRENT_SOURCE_DIR}/../../../../" ${TEST_FILE})
    ADD_TEST(${TEST_NAME} python ${CMAKE_CURRENT_SOURCE_DIR}/../test_main.py -p ${TEST_FILE} --compile_project --debug)
    SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES TIMEOUT 600)
ENDFOREACH()