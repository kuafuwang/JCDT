
#ifndef JCDT_FindReferenceOption__Search_H_FLAG_
#define JCDT_FindReferenceOption__Search_H_FLAG_

#include <set>
#include <vector>
#include <map>
using std::set;
using std::wstring;

namespace Jikes {


namespace Search 
{
	class FindReferenceOption
	{
	public:

		enum RangeOpt
		{
			IN_LIMIT,
			OUT_LIMIT,
			ALL_RANGE
		};
		FindReferenceOption();
		virtual ~FindReferenceOption() {}
		bool IsCandidate(unsigned int number)
		{
			switch (opt)
			{
			case IN_LIMIT:
			{
				if (number <= count_uper)
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			case OUT_LIMIT:
			{
				if (count_lower <= number && number <= count_uper)
				{
					return false;
				}
				else
				{
					return true;
				}
			}

			default:
			{
				return true;
			}

			}
		}
		bool IsTheOne(unsigned int number)
		{
			if(IsZero())
			{
				return  true;
			}
			switch (opt)
			{
			case IN_LIMIT:
			{
				if (count_lower <= number && number <= count_uper)
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			case OUT_LIMIT:
			{
				if (count_lower <= number && number <= count_uper)
				{
					return false;
				}
				else
				{
					return true;
				}
			}

			default:
			{
				return true;
			}

			}
		}
		bool IsZero()
		{
			if(opt == IN_LIMIT)
			{
				if (count_lower == 0 && 0 == count_uper)
				{

					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			
		}
		unsigned int count_lower;
		unsigned int count_uper;
		RangeOpt opt;

		bool more_info;


		bool find_type_reference;
		bool find_method_reference;
		bool find_filed_reference;
		bool find_local_filed_reference;
	};

	inline FindReferenceOption::FindReferenceOption() : count_lower(0), count_uper(0), opt(IN_LIMIT), more_info(false),
	                                                    find_type_reference(false),
	                                                    find_method_reference(true),
	                                                    find_filed_reference(false),
		find_local_filed_reference(false)
	{
	}

	class InFilesFindReferenceOption : public FindReferenceOption
	{
	public:
		std::map < wstring, std::vector<wstring>  > files;

		~InFilesFindReferenceOption()
		{
			files.clear();
		}
	};
	class InBlockfindReferenceOption : public FindReferenceOption
	{
	public:
		wstring projectName;
		wstring filePath;

	};

	
}//namespace Search

} // Close namespace Jikes block


#endif // JCDT_FindReferenceOption__Search_H_FLAG_
