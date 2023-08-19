#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include "Image.hpp"
#include "BaseEstimator.hpp"
#include "KMeans.hpp"

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

namespace py = pybind11;


PYBIND11_MODULE(dominant_tones_bindings, m)
{
    py::class_<Image>(m, "Image")
        .def(py::init<const std::string&>())
        .def(py::init<const std::vector<uint8_t>&>())
        .def(py::init<const std::vector<std::vector<Image::Pixel>>&>())
        .def(py::init<const std::vector<std::vector<std::vector<uint8_t>>>&>())
        .def(py::init<const std::vector<Image::Pixel>&, int, int, bool>())
        .def_property_readonly("width", &Image::GetWidth)
        .def_property_readonly("height", &Image::GetHeight)
        .def_property_readonly("channels", &Image::GetChannels)
        .def_property_readonly("size", &Image::GetSize)
        .def("get_image", &Image::GetImage)
        .def("__getitem__", [](const Image& img, size_t idx) { return img[idx]; })
        .def("__getitem__", [](const Image& img, int x, int y) { return img(x, y); })
        .def("__setitem__", [](Image& img, size_t idx, const Image::Pixel& pixel) { img[idx] = pixel; })
        .def("__setitem__", [](Image& img, int x, int y, const Image::Pixel& pixel) { img(x, y) = pixel; })
        .def("__iter__", [](const Image& img) { return py::make_iterator(img.begin(), img.end()); }, py::keep_alive<0, 1>())
        .def("__len__", &Image::GetSize);

    py::class_<BaseEstimator, std::shared_ptr<BaseEstimator>>(m, "BaseEstimator")
        .def("fit", &BaseEstimator::Fit);

    py::class_<KMeans, BaseEstimator, std::shared_ptr<KMeans>>(m, "KMeans")
        .def(py::init<int, int, double, unsigned int>())
        .def("fit", &KMeans::Fit)
        .def_property_readonly("colors", &KMeans::GetColors)
        .def_property_readonly("labels", &KMeans::GetLabels);

#ifdef VERSION_INFO
    m.attr("__version__") = STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}