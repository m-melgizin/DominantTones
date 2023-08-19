#ifndef BASE_ESTIMATOR_HPP
#define BASE_ESTIMATOR_HPP

#include "Image.hpp"

class BaseEstimator
{
public:
    virtual void Fit(const Image& image) = 0;
};

#endif // !BASE_ESTIMATOR_HPP