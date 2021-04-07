include(FetchContent)

FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG v0.8.6
)

FetchContent_GetProperties(httplib)
if (NOT httplib_POPULATED)
    FetchContent_Populate(httplib)
    add_subdirectory(${httplib_SOURCE_DIR} ${httplib_BINARY_DIR})
endif ()

package_add_test(FeedlyClient_unit_test FeedlyClient.cpp)
target_include_directories(FeedlyClient_unit_test PRIVATE ${httplib_SOURCE_DIR})
