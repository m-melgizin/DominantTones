
set(
    DOMINANT_TONES_SOURCES
    Image.cpp
    Image.hpp
    stb_image.h
)

add_library(
    DominantTones
    STATIC
    ${DOMINANT_TONES_SOURCES}
)
