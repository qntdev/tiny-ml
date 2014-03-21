#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using DP = std::vector<double>;

namespace ML_Tests
{
	TEST_CLASS(KMeans_UnitTests)
	{
	public:
		
		TEST_METHOD(SettingInitCentroidsLenghtDifferentThrowsException)
		{
			KMeans<DP> k_means;
			DataSet<DP> dataset;

			Assert::ExpectException<std::exception>([&]{
				k_means.setInitCentroids(dataset);
			});
		}
		

		TEST_METHOD(FitOnEmptyDataThrowsException)
		{
			KMeans<DP> k_means;
			DataSet<DP> dataset;

			Assert::ExpectException<std::exception>([&]{
				k_means.fit(dataset); 
			});
		}



	};
}