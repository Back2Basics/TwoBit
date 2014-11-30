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

// see http://genome.ucsc.edu/FAQ/FAQformat#format7
#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include <memory>

#include "TBMeta.hpp"

namespace TwoBit
{

class TwoBitSequenceMeta;

class TwoBitFile
{
private:

	static const uint32_t MAGIC_NUMBER = 0x1A412743;
	static const uint32_t REVERSE_MAGIC_NUMBER = 0x4327411A;
	static const uint32_t VERSION = 0;
	static const uint32_t RESERVED = 0;
	static const uint32_t SEQNAME_MAX_LEN = 256; // represented by one byte so, ...

	bool swapped_;
	uint32_t magic_;
	uint32_t version_;
	uint32_t sequenceCount_;
	uint32_t reserved_;
	std::string filename_;
	std::ifstream file_;
	std::unordered_map<std::string, SequenceMeta> sequences_;

	// read 16 header bytes from file.
	void readTwoBitHeader();

	// create sequences from file
	void createSequenceMeta();
	void populateSequenceMeta(SequenceMeta& meta);
	void readRegions(std::vector<SequenceMeta::Region>& out);


	friend class TwoBitSequenceMeta;

public:
	TwoBitFile(const std::string& filename);

	// no-copy
	TwoBitFile(const TwoBitFile& other) = delete;
	TwoBitFile() = delete;
	TwoBitFile& operator=(const TwoBitFile& other) = delete;

	void init();
	void test();
};

} // namespace TwoBit
