/**
 * @file Analyzer.h
 * ポリゴンデータ、もしくはボリュームデータ解析ユーティリティ
 */
#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <limits>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

namespace  {
	
inline std::string string_format(const std::string fmt, ...) {
	int size = 100;
	std::string str;
	va_list ap;
	while (1) {
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) {
			str.resize(n);
			return str;
		}
		if (n > -1)
			size = n + 1;
		else
			size *= 2;
	}
	return str;
}

/**
 * Histogram
 */
class Histogram {
public:
	Histogram(int numBins, double minVal, double maxVal)
	: minVal_(minVal), maxVal_(maxVal), totalCount_(0), invalid_(false) {
		double range = maxVal - minVal;
		histogram_.resize(numBins);

		for (size_t i = 0; i < histogram_.size(); i++) {
			histogram_[i] = 0;
		}
		
		assert(numBins >= 1);
		//assert(maxVal_ >= minVal_); // Allow invalid range input
		if (maxVal_ < minVal_) {
			fprintf(stderr, "Invalid histgram range: %f, %f\n", minVal_, maxVal_);
			invalid_ = true; 
		}
		
		if(range == 0.0) {
			range = 1.0;
		}
		invRange_ = 1.0 / range;
	}
	~Histogram() {};
	
	void SetRange(double minVal, double maxVal) {
		minVal_ = minVal;
		maxVal_ = maxVal;

		if (maxVal_ < minVal_) {
			fprintf(stderr, "Invalid histgram range: %f, %f\n", minVal_, maxVal_);
			invalid_ = true; 
		}
	}
	
	void Contribute(double val) {

		if (invalid_) return;

		double clampled = std::max(minVal_, std::min(maxVal_, val));
		
		int bin = (histogram_.size() - 1) * ((clampled - minVal_) * invRange_);
		if (bin < 0) bin = 0;
		if (bin >= histogram_.size()) bin = histogram_.size()-1;

		assert(bin >= 0);
		assert(bin < histogram_.size());
		
		histogram_[bin]++;
		
		totalCount_++;
	}
	
	// Get normalized histogram(data is in range [0.0, 1.0])
	void GetNormalizedHistogram(std::vector<float> &outHistogram) {
		outHistogram.resize(histogram_.size());
		
		// Find peak value
		size_t peakVal = 0;
		for (size_t i = 0; i < histogram_.size(); i++) {
			peakVal = std::max(peakVal, histogram_[i]);
		}
		
		assert(peakVal > 0);
		
		double invVal = 1.0 / peakVal;
		
		for (size_t i = 0; i < outHistogram.size(); i++) {
			outHistogram[i] = (double)histogram_[i] * invVal;
		}
	}
	
private:
	std::vector<size_t> histogram_;
	double minVal_;
	double maxVal_;
	double invRange_;
	size_t totalCount_;
	bool   invalid_;
};

/**
 * ボリュームデータ解析クラス
 */
class VolumeAnalyzerProc {
public:
	VolumeAnalyzerProc() {};
	~VolumeAnalyzerProc() {};
	
	// Analyzer volume and returns min/max value and histogram.
	template <typename T>
	void AnalyzeScalar(std::vector<float> &outHistogram, double &minVal,
							 double &maxVal, const T *vol, int res[3],
							 int numBins = 256) {
		
		// 1. find min and max value;
		minVal = std::numeric_limits<double>::max();
		maxVal = -std::numeric_limits<double>::max();
		
		for (size_t i = 0; i < res[0] * res[1] * res[2]; i++) {
			minVal = std::min((double)vol[i], minVal);
			maxVal = std::max((double)vol[i], maxVal);
		}
		
		// 2. compute histogram;
		Histogram hist(numBins, minVal, maxVal);
		for (size_t i = 0; i < res[0] * res[1] * res[2]; i++) {
			hist.Contribute(vol[i]);
		}
		
		hist.GetNormalizedHistogram(outHistogram);
	}

	template <typename T>
	void AnalyzeVector(std::vector<float> outHistograms[3],
							 double minVal[3], double maxVal[3], const T *vol,
							 int res[3], int numBins = 256, int component = 3) {
		
		for (int k = 0; k < 3; k++) { // TODO: component
			
			// 1. find min and max value;
			minVal[k] = std::numeric_limits<double>::max();
			maxVal[k] = -std::numeric_limits<double>::max();
			
			for (size_t i = 0; i < res[0] * res[1] * res[2]; i++) {
				minVal[k] = std::min((double)vol[component * i + k], minVal[k]);
				maxVal[k] = std::max((double)vol[component * i + k], maxVal[k]);
			}
			
			// 2. compute histogram;
			Histogram hist(numBins, minVal[k], maxVal[k]);
			for (size_t i = 0; i < res[0] * res[1] * res[2]; i++) {
				hist.Contribute(vol[component * i + k]);
			}
			
			hist.GetNormalizedHistogram(outHistograms[k]);
		}
	}
	
	// Dump information as JSON string.
	static std::string DumpScalar(std::vector<float> hist, double minVal,
								  double maxVal) {
		
		std::stringstream ss;
		
		ss << string_format("{\"type\":\"scalar\",\n");
		ss << string_format(" \"min\": %f,\n", minVal);
		ss << string_format(" \"max\": %f,\n", maxVal);
		ss << string_format(" \"histgram\": [\n");
		
		for (size_t i = 0; i < hist.size(); i++) {
			ss << string_format("%f ", hist[i]);
			if (i != (hist.size() - 1)) {
				ss << ", ";
			}
		}
		
		ss << " ]\n";
		ss << "}\n";
		
		return ss.str();
	}
	
	// Dump information as JSON string.
	static std::string DumpVector(const std::vector<float> hist[3], const double minVal[3],
								  const double maxVal[3]) {
		
		assert(hist[0].size() == hist[1].size());
		assert(hist[1].size() == hist[2].size());
		
		std::stringstream ss;
		
		ss << string_format("{\"type\":\"vector\",\n");
		ss << string_format(" \"min\": [%f, %f, %f],\n", minVal[0], minVal[1],
							minVal[2]);
		ss << string_format(" \"max\": [%f, %f, %f],\n", maxVal[0], maxVal[1],
							maxVal[2]);
		ss << string_format(" \"histgram\": [\n");
		
		for (size_t i = 0; i < hist[0].size(); i++) {
			ss << string_format("%f, %f, %f\n", hist[0][i], hist[1][i], hist[2][i]);
			if (i != (hist[0].size() - 1)) {
				ss << ", ";
			}
		}
		
		ss << " ]\n";
		ss << "}\n";
		
		return ss.str();
	}
};


/**
 * ポリゴンデータ解析クラス
 */
class PolygonAnalyzerProc {
public:
	PolygonAnalyzerProc() {};
	~PolygonAnalyzerProc() {};
	
	template <typename T>
	void AnalyzeVector(std::vector<float> outHistograms[3],
							 double minVal[3], double maxVal[3], const T *vol,
							 int size, int numBins = 256, int component = 3) {
		for (int k = 0; k < 3; k++) {
			// 1. find min and max value;
			minVal[k] = std::numeric_limits<double>::max();
			maxVal[k] = -std::numeric_limits<double>::max();
			
			for (size_t i = 0; i < size; i++) {
				minVal[k] = std::min((double)vol[component * i + k], minVal[k]);
				maxVal[k] = std::max((double)vol[component * i + k], maxVal[k]);
			}
		}
	}
	
	// Dump information as JSON string.
	static std::string DumpScalar(std::vector<float> hist, double minVal,
								  double maxVal) {
		
		std::stringstream ss;
		
		ss << string_format("{\"type\":\"scalar\",\n");
		ss << string_format(" \"min\": %f,\n", minVal);
		ss << string_format(" \"max\": %f,\n", maxVal);
		ss << string_format(" \"histgram\": [\n");
		
		for (size_t i = 0; i < hist.size(); i++) {
			ss << string_format("%f ", hist[i]);
			if (i != (hist.size() - 1)) {
				ss << ", ";
			}
		}
		
		ss << " ]\n";
		ss << "}\n";
		
		return ss.str();
	}
	
	// Dump information as JSON string.
	static std::string DumpVector(const std::vector<float> hist[3], const double minVal[3],
								  const double maxVal[3]) {
		
		assert(hist[0].size() == hist[1].size());
		assert(hist[1].size() == hist[2].size());
		
		std::stringstream ss;
		
		ss << string_format("{\"type\":\"vector\",\n");
		ss << string_format(" \"min\": [%f, %f, %f],\n", minVal[0], minVal[1],
							minVal[2]);
		ss << string_format(" \"max\": [%f, %f, %f],\n", maxVal[0], maxVal[1],
							maxVal[2]);
		ss << string_format(" \"histgram\": [\n");
		
		for (size_t i = 0; i < hist[0].size(); i++) {
			ss << string_format("%f, %f, %f\n", hist[0][i], hist[1][i], hist[2][i]);
			if (i != (hist[0].size() - 1)) {
				ss << ", ";
			}
		}
		
		ss << " ]\n";
		ss << "}\n";
		
		return ss.str();
	}
};
	
} // namespace
#endif // _ANALYZER_H_

