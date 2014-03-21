#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <vector>	//
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <fstream>


template<typename DataType>
DataSet<DataType> load_data(std::string filename)
{
	DataSet<DataType> ret_val;

	std::ifstream file;
	file.open(filename);

	std::string line;
	auto i = 0;
	while (std::getline(file, line))
	{
		std::vector<std::string> splited;
		boost::algorithm::split(splited, line, boost::is_any_of(","));

		auto open = boost::lexical_cast<double>(splited[2]);
		auto high = boost::lexical_cast<double>(splited[3]);
		auto low = boost::lexical_cast<double>(splited[4]);
		auto close = boost::lexical_cast<double>(splited[5]);
		//auto time = boost::posix_time::from_time_t(i * 15 * 60);
		DataType dt{ open, high, low, close };

		DataPoint<DataType> dp(dt);
		ret_val.push_back(dp);
		i++;
	}

	return ret_val;
}