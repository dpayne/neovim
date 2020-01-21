include(ExternalProject)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(DEPS_BUILD_DIR "${CMAKE_BINARY_DIR}" CACHE PATH "Dependencies build directory.")

# ExternalProject_Add(project_fuzz_test
#   EXCLUDE_FROM_ALL TRUE
#   PREFIX ${DEPS_BUILD_DIR}
#   BINARY_DIR ${DEPS_BUILD_DIR}/src/project_fuzz_test
#   GIT_REPOSITORY "https://github.com/dpayne/fuzz_test.git"
#   GIT_TAG ""
#   CONFIGURE_COMMAND ""
#   BUILD_COMMAND PREFIX=${DEPS_BUILD_DIR} make
#   INSTALL_COMMAND PREFIX=${DEPS_BUILD_DIR} make install
# )

add_library(mylibnvim STATIC IMPORTED)
set_property(TARGET mylibnvim PROPERTY IMPORTED_LOCATION ${DEPS_BUILD_DIR}/lib/libnvim.a)

link_directories(${DEPS_BUILD_DIR}/lib)

function(project_add_fuzz_test TESTNAME TEST_SOURCES PROJECT_LIBRARY)
    add_executable(${TESTNAME} ${TEST_SOURCES})
    set_target_properties(${TESTNAME} PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(${TESTNAME} mylibnvim libnvim)
    target_compile_options(${TESTNAME} PRIVATE -fsanitize=address,fuzzer-no-link -fprofile-instr-generate -fcoverage-mapping)
    target_link_options(${TESTNAME} PRIVATE -fsanitize=address,fuzzer -fprofile-instr-generate -fcoverage-mapping)

    target_include_directories(${TESTNAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/fuzz_tests
        /usr/local/include
        ${DEPS_BUILD_DIR}/include
        )

    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endfunction()

set(PROJECT_FUZZ_TEST_NAME ${PROJECT_NAME}_fuzz_test)
set(PROJECT_FUZZ_TEST_LIBS ${PROJECT_NAME} "")

set(PROJECT_FUZZ_TEST_FILES
    ${CMAKE_SOURCE_DIR}/fuzz_tests/GenericTest.c
    ${CMAKE_SOURCE_DIR}/fuzz_tests/treesitter_c/treesitter_c_parser.c
)

project_add_fuzz_test(${PROJECT_FUZZ_TEST_NAME} "${PROJECT_FUZZ_TEST_FILES}" "${PROJECT_FUZZ_TEST_LIBS}")
