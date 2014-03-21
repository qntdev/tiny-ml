#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../ML/KMeans.h"

#include "utils.h"

using DP = std::vector<double>;

namespace ML_Tests
{		
	TEST_CLASS(FullTests)
	{
	public:
		static DataSet<DP> _data;

		TEST_CLASS_INITIALIZE(LoadData)
		{
			_data = load_data<DP>("../TestData/EURUSD15.csv");
		}
		
		TEST_METHOD(TestOnFullData)
		{
			KMeans<std::vector<double>> k_means;
			k_means.setMaxIter(10000);
			k_means.fit(_data);
		}

	};

	DataSet<DP> FullTests::_data = DataSet<DP>();
}