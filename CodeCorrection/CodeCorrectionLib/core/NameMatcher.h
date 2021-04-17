#pragma once
#include  <string>


namespace Jikes { // Open namespace Jikes block

	namespace Correction {
 class NameMatcher 
 {
 public:
	/**
	 * Returns a similarity value of the two names.
	 * The range of is from 0 to 256. no similarity is negative
	 */
	 static bool isSimilarName(const std::wstring& name1, const std::wstring& name2);

	/**
	 * Returns a similarity value of the two names.
	 * The range of is from 0 to 256. no similarity is negative
	 */
	 static int getSimilarity(std::wstring name1, std::wstring name2);

	 static bool isSimilarChar(int ch1, int ch2);
};

} // Close namespace Correction block

} // Close namespace Jikes block


