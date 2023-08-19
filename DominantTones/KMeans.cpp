#include "KMeans.hpp"

#include <random>
#include <cmath>

namespace
{
    // Helper function to calculate Euclidean distance between two pixels
    double distance(Image::Pixel p1, Image::Pixel p2)
    {
        return std::sqrt(
            static_cast<double>((p1[0] - p2[0]) * (p1[0] - p2[0])) +
            static_cast<double>((p1[1] - p2[1]) * (p1[1] - p2[1])) +
            static_cast<double>((p1[2] - p2[2]) * (p1[2] - p2[2]))
        );
    }
} // namespace


KMeans::KMeans(int nClusters, int maxIter, double tol, unsigned int randomState)
    : m_nClusters(nClusters)
    , m_maxIter(maxIter)
    , m_tol(tol)
    , m_randomState(randomState)
    , m_clusterCenters(nClusters)
{
}

KMeans::KMeans(KMeans&& other) noexcept
{
    m_nClusters = other.m_nClusters;
    m_maxIter = other.m_maxIter;
    m_tol = other.m_tol;
    m_randomState = other.m_randomState;

    m_clusterCenters = std::move(other.m_clusterCenters);
    m_labels = std::move(other.m_labels);
}

KMeans& KMeans::operator=(KMeans&& other) noexcept
{
    if (&other == this)
        return *this;

    m_nClusters = other.m_nClusters;
    m_maxIter = other.m_maxIter;
    m_tol = other.m_tol;
    m_randomState = other.m_randomState;

    m_clusterCenters = std::move(other.m_clusterCenters);
    m_labels = std::move(other.m_labels);

    return *this;
}

void KMeans::Fit(const Image& image)
{
    std::mt19937 rng(m_randomState);
    std::uniform_int_distribution<size_t> distribution(0, image.GetSize());

    m_labels = std::vector<int>(image.GetSize());

    // Step 1: Initialize centroids randomly
    for (int i = 0; i < m_nClusters; ++i)
        m_clusterCenters[i] = image[distribution(rng)];

    // Repeat until convergence or max iterations reached
    int iter = 0;
    while (iter < m_maxIter)
    {
        iter++;

        // Step 2: Assign each pixel to the nearest centroid
        std::vector<std::vector<Image::Pixel>> clusters(m_nClusters);
        for (size_t pixelIdx = 0; pixelIdx < image.GetSize(); pixelIdx++)
        {
            const auto& pixel = image[pixelIdx];
            double minDistance = std::numeric_limits<double>::max();
            int closestCentroid = -1;
            for (int i = 0; i < m_nClusters; i++)
            {
                double distance = ::distance(pixel, m_clusterCenters[i]);
                if (distance < minDistance)
                {
                    minDistance = distance;
                    closestCentroid = i;
                }
            }
            clusters[closestCentroid].push_back(pixel);
            m_labels[pixelIdx] = closestCentroid;
        }

        // Step 3: Update centroid colors
        std::vector<Image::Pixel> newCentroids(m_nClusters);
        for (int i = 0; i < m_nClusters; i++)
        {
            if (clusters[i].empty()) continue;
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;
            const auto clusterSize = clusters[i].size();
            for (const auto& pixel : clusters[i])
            {
                sumR += pixel[0];
                sumG += pixel[1];
                sumB += pixel[2];
            }
            newCentroids[i][0] = static_cast<uint8_t>(sumR / clusterSize);
            newCentroids[i][1] = static_cast<uint8_t>(sumG / clusterSize);
            newCentroids[i][2] = static_cast<uint8_t>(sumB / clusterSize);
        }

        // Check for convergence
        bool converged = true;
        for (int i = 0; i < m_nClusters; i++)
        {
            if (::distance(newCentroids[i], m_clusterCenters[i]) > m_tol)
            {
                converged = false;
                break;
            }
        }
        if (converged) break;

        // Update centroids
        m_clusterCenters = newCentroids;
    }
}

std::vector<Image::Color> KMeans::GetColors()
{
    return m_clusterCenters;
}

const std::vector<int>& KMeans::GetLabels()
{
    return m_labels;
}