add_library(cdf_warnings INTERFACE)

set(CDF_WARNINGS_COMMON
    -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion
    -Wcast-qual -Wwrite-strings -Wformat=2 -Wundef -Wstrict-prototypes
    -Wold-style-definition -Wimplicit-fallthrough -Wcast-align -Wvla
    -Wnull-dereference -Wdouble-promotion -Wformat-overflow=2
    -Wformat-truncation=2 -Warray-bounds=2 -Wstringop-overflow=4
    -Wswitch-enum -Wpointer-arith -Winit-self
    -Werror
)

set(CDF_WARNINGS_GCC_ONLY
    -Wlogical-op -Walloc-zero -Wstrict-overflow=5
)

target_compile_options(cdf_warnings INTERFACE ${CDF_WARNINGS_COMMON})

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(cdf_warnings INTERFACE ${CDF_WARNINGS_GCC_ONLY})
endif()
