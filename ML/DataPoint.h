#pragma once

namespace ML
{
	// DATA POINT
	/*
	*
	*/
	template <typename T, typename ValType = double>
	class DataPoint
	{
	public:
		DataPoint() = default;
		DataPoint(const DataPoint&) = default;
		DataPoint& operator=(const DataPoint&) = default;

		DataPoint(T& dp)
			:m_datapoint(dp)
		{}

		T& data() {
			return m_datapoint;
		}

		ValType& operator[](int idx){
			return m_datapoint[idx];
		}

		int label() const {
			return m_label;
		}

		void setLabel(int l){
			this->m_label = l;
		}

		double dist(T& other){
			return Distance::dist(*this, other);
		}

		int size() const {
			return std::distance(std::begin(m_datapoint), std::end(m_datapoint));
		}

		std::string toString(){
			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2);

			for (auto &datapoint : m_datapoint)
				ss << datapoint << ";";

			auto ret_str = ss.str().pop_back();
			return ret_str;
		}

	private:
		T m_datapoint;
		int m_label;
	};

}