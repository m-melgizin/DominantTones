
include_directories(../DominantTones)

add_subdirectory(pybind11)
pybind11_add_module(dominant_tones_bindings bindings.cpp)

target_link_libraries(dominant_tones_bindings PRIVATE DominantTones)

# DOMINANT_TONES_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(dominant_tones_bindings
                           PRIVATE VERSION_INFO=${DOMINANT_TONES_VERSION_INFO})