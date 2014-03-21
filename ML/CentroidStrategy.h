#pragma once

#include "DataPoint.h"

namespace Centroid
{
	/*
	*	
	*/
	template <typename T>
	class MeanPolicy
	{
	public:
		DataSet<T> calculateCentroids( DataSet<T> &dataset, int num_clusters)
		{
			DataSet<T> ret_ds = DataSet<T>(num_clusters);

			// Get dimensionality	
			int dimm = dataset.begin()->data().size();

			for (int cl_idx = 0; cl_idx < num_clusters; cl_idx++)
			{
				// sum and count points from cluster
				std::vector<double> sums(dimm, 0.0);
				int counter = 0;
				for (auto &dp : dataset)
				{
					if (dp.label() == cl_idx)
					{
						for (decltype(sums.size()) i = 0; i < sums.size(); i++)
							sums[i] += (dp.data()[i]);
						counter++;
					}
				}

				if (counter>0)
				{
					// calculate mean
					for (decltype(sums.size()) i = 0; i < sums.size(); i++)
						sums[i] = sums[i] / counter;
				}

				// dp is our centroid
				auto dp = DataPoint<T>(sums);

				// set centroid id/label
				dp.setLabel(cl_idx);

				// add centroid to return set
				ret_ds[cl_idx] = dp;				
			}
			return ret_ds;
		}

	};

	template<typename T>
	class MedianPolicy
	{
	public:
		DataSet<T> calculateCentroids( DataSet<T> &dataset, int num_clusters);
	};
}
