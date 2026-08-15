file(GLOB CDF_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_library(cdf_static STATIC ${CDF_SOURCES})
add_library(cdf_shared SHARED ${CDF_SOURCES})

foreach(tgt cdf_static cdf_shared)
    target_include_directories(${tgt} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${tgt} PRIVATE cdf_warnings)
    set_target_properties(${tgt} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endforeach()

set_target_properties(cdf_static PROPERTIES OUTPUT_NAME cdf)
set_target_properties(cdf_shared PROPERTIES OUTPUT_NAME cdf)
