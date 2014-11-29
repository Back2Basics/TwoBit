/*
 Copyright 2014 Arjan van der Velde, vandervelde.ag [at] gmail

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <memory>

#include "TwoBitFile.hpp"
// #include "Region.hpp"

namespace TwoBit
{

class TwoBitSequence
{
private:

	class Region
	{
	public:
		uint32_t pos_;
		int action_; // start or end
		Region(const uint32_t& pos, const int& action) :
				pos_(pos), action_(action)
		{
		}
	};

	static const uint32_t BUFFER_SIZE = 0x10000; // 64k

	const std::string name_;
	const uint32_t offset_;
	const std::string filename_;
	const bool swapped_;
	std::ifstream file_;
	uint32_t dnaSize_;
	uint32_t dnaBytes_;
	uint32_t packedpos_;

	std::vector<Region> nRegions; // N-s
	std::vector<Region> mRegions; // Masked

	char buffer_[BUFFER_SIZE];

	void readRegions(std::vector<Region>& out);
	void open();
	void close();

public:

	TwoBitSequence(const std::string& name, const uint32_t offset,
			TwoBitFile& file);

	virtual ~TwoBitSequence();

	TwoBitSequence(const TwoBitSequence& other) = delete;
	TwoBitSequence() = delete;
	TwoBitSequence& operator=(const TwoBitSequence& other) = delete;

	void test();
	void getSequence(std::vector<char>& buffer, uint32_t start, uint32_t end);

};

} // namespace TwoBit
