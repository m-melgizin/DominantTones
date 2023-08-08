#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <vector>

#include "BaseEstimator.hpp"
#include "Image.hpp"

class KMeans : public BaseEstimator
{
public:
    KMeans(int nClusters = 10, int maxIter = 300, double tol = 1e-4, unsigned int randomState = 0);
    ~KMeans() = default;

    KMeans(const KMeans&) = delete;
    KMeans& operator=(const KMeans&) = delete;

    KMeans(KMeans&& other) noexcept;
    KMeans& operator=(KMeans&& other) noexcept;

public:
    void Fit(const Image& image) override;
    std::vector<Image::Color> GetColors();
    const std::vector<int>& GetLabels();

private:
    int m_nClusters;
    int m_maxIter;
    double m_tol;
    unsigned int m_randomState;

    std::vector<Image::Pixel> m_clusterCenters;
    std::vector<int> m_labels;
};

#endif // !KMEANS_HPP