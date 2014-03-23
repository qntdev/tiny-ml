#pragma once

#include <random>
#include <map>
#include <algorithm>

#include "DataSet.h"
#include "CentroidStrategy.h"

static const int default_num_of_clusters = 8;

namespace ML
{
	// K-MEANS
	template<typename T, typename CentroidPolicy = MeanPolicy<T>>
	class KMeans
		: public CentroidPolicy
	{
	public:

		/*
		*
		*/
		void fit(DataSet<T> &data, int num_clusters = default_num_of_clusters, DataSet<T>& init_clusters = DataSet<T>())
		{
			//
			if (data.empty())
				throw std::invalid_argument("Data set cannot be empty");

			//
			if (num_clusters != default_num_of_clusters)
				this->m_num_of_clusters = num_clusters;

			// Randomize intial centroids
			if (init_clusters.empty())
				m_centroids = Extensions::randomClusters(data, num_clusters, data[0].size());
			else
				m_centroids = init_clusters;

			// Calculate clusters
			find_clusters(data);
		}

		/*
		*
		*/
		void setNumClusters(int num_clusters){
			this->m_cluster_count = num_clusters;
		}

		/*
		*
		*/
		void setMaxIter(int max_iter){
			this->m_max_iter = max_iter;
		}

		/*
		*
		*/
		void setInitCentroids(DataSet<T> &init_centroids){
			if (init_centroids.size() != m_num_of_clusters)
				throw std::invalid_argument("Centroid set size differs from number of clusters");

			this->m_centroids = init_centroids;
		}

		/*
		*
		*/
		int predict(DataPoint<T> &dp)
		{
			calculate_distances(dp, m_centroids);
			int argmin = std::distance(distances.begin(), std::min_element(distances.begin(), distances.end()));

			assert(argmin <= m_num_of_clusters);
			assert(m_num_of_clusters >= 0);

			return argmin;
		}

	private:

		void find_clusters(DataSet<T> &data)
		{
			//
			keep_working = true;

			for (m_iter_counter = 0; m_iter_counter < m_max_iter; m_iter_counter++)
			{
				if (!keep_working) break;

				for (auto &data_point : data)
				{
					auto distances = calculate_distances(data_point, m_centroids);
					// closest centroid
					int arg_min = std::distance(distances.begin(), std::min_element(distances.begin(), distances.end()));
					data_point.setLabel(arg_min);
				}

				m_centroids = calculateCentroids(data, m_num_of_clusters);

				cluster_count_diff(data);
			}
		}

		int cluster_count(DataSet<T> &data, int cluster_id)
		{
			int count = std::count_if(data.begin(), data.end(), [cluster_id](DataPoint<T> &dp){return dp.label() == cluster_id; });
			return count;
		}

		std::vector<double> calculate_distances(DataPoint<T> &dp, DataSet<T> &ds)
		{
			std::vector<double> distances(ds.size(), 0.0);
			int i = 0;
			for (auto &centroid : m_centroids)
			{
				double dist = Extensions::dist(centroid.data(), dp.data());
				distances[i++] = dist;
			}

			return distances;
		}

		bool cluster_count_diff(DataSet<T> &data)
		{
			keep_working = false;

			std::map<int, int> new_cc;
			for (int c_id = 0; c_id < m_num_of_clusters; c_id++)
				new_cc[c_id] = cluster_count(data, c_id);

			bool equal = (new_cc.size() == m_cluster_count.size() && std::equal(new_cc.begin(), new_cc.end(), m_cluster_count.begin()));
			if (!equal)
			{
				m_cluster_count = new_cc;
				keep_working = true;
			}

			return equal;
		}

	private:
		// Algorithm setup and parameters
		DataSet<T> m_centroids;
		DataSet<T> m_data;
		int m_num_of_clusters = default_num_of_clusters;
		int m_max_iter = 1000;

		// additional/helpers
		bool keep_working = true;
		int m_iter_counter = 0;
		std::map<int, int> m_cluster_count; // counts the number of points in each cluster

	};


	/*
	*
	*/
	class Extensions
	{
	public:
		template <typename T>
		static DataSet<T> randomClusters(DataSet<T> &data, int num_clusters, int dimm)
		{
			std::random_device rd;
			std::mt19937 engine(rd());
			std::vector<DataPoint<T>> ret_vec;

			for (int i = 0; i < num_clusters; i++)
			{
				std::vector<double> v(dimm);

				for (int d = 0; d < dimm; d++)
				{
					auto min = Extensions::minElement(data, d);
					auto max = Extensions::maxElement(data, d);
					std::uniform_real_distribution<double> udist(min, max);
					v[d] = udist(engine);
				}

				DataPoint<T> dp(v);
				ret_vec.push_back(dp);
			}

			return ret_vec;
		}

		static double dist(const std::vector<double> &v1, const std::vector<double> &v2)
		{
			if (v1.size() != v2.size())
				throw std::exception("Data points of different size");

			double distance = 0.0;
			for (unsigned int i = 0; i < v1.size(); i++)
			{
				distance += pow(v1[i] - v2[i], 2);
			}

			return sqrt(distance);
		}

		template<typename T>
		static double maxElement(DataSet<T> &data, int column){
			auto max_el = std::max_element(data.begin(), data.end(), [column](DataPoint<T> &l, DataPoint<T> &r){ return l.data()[column] < r.data()[column]; });
			return max_el->data()[column];
		}

		template<typename T>
		static double minElement(DataSet<T> &data, int column){
			auto min_el = std::min_element(data.begin(), data.end(), [column](DataPoint<T> &l, DataPoint<T> &r){ return l.data()[column] < r.data()[column]; });
			return min_el->data()[column];
		}

		template<typename T>
		static int argMin(T& range){
			int argmin = std::distance(std::begin(range), std::min_element(std::begin(range), std::end(range)));
			return argmin;
		}

		template <typename T>
		static int argMax(T& range){
			int argmax = std::distance(std::begin(range), std::max_element(std::begin(range), std::end(range)));
			return argmax;
		}
	};

}