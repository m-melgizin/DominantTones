
set(
    DOMINANT_TONES_SOURCES
    Image.cpp
    Image.hpp
    KMeans.cpp
    KMeans.hpp
    stb_image.h
)

add_library(
    DominantTones
    STATIC
    ${DOMINANT_TONES_SOURCES}
)
