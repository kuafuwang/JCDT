// $Id: spell.h,v 1.11 2002/12/05 21:56:13 ericb Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef spell_INCLUDED
#define spell_INCLUDED

#include "JCDT_Lib/internal/env/platform.h"
#include "JCDT_Lib/internal/util/case.h"


namespace Jikes { // Open namespace Jikes block


//
// This is a helper class which hueristically determines the probability
// that one string is a match for another.  The static function Index
// returns an integer from 0 to 10; for example, a return of 6 indicates
// a 60% chance that one string is a misspelled match of the other.
//
class Spell
{
    static inline int Min(int x, int y) { return (x < y ? x : y); }

public:

#define SpellMIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

	static	int levenshtein(const char *s1, const char *s2) {
		unsigned int s1len, s2len, x, y, lastdiag, olddiag;
		s1len = strlen(s1);
		s2len = strlen(s2);
		unsigned int* column = new unsigned int[s1len + 1];
		for (y = 1; y <= s1len; y++)
			column[y] = y;
		for (x = 1; x <= s2len; x++) {
			column[0] = x;
			for (y = 1, lastdiag = x - 1; y <= s1len; y++) {
				olddiag = column[y];
				column[y] = SpellMIN3(column[y] + 1, column[y - 1] + 1, lastdiag + (s1[y - 1] == s2[x - 1] ? 0 : 1));
				lastdiag = olddiag;
			}
		}
		auto _ret = (column[s1len]);
		delete[] column;
		return _ret;
	}
	static	int levenshtein(const wchar_t *s1, const wchar_t *s2) {
		unsigned int s1len, s2len, x, y, lastdiag, olddiag;
		s1len = wcslen(s1);
		s2len = wcslen(s2);
		unsigned int* column = new unsigned int[s1len + 1];
		for (y = 1; y <= s1len; y++)
			column[y] = y;
		for (x = 1; x <= s2len; x++) {
			column[0] = x;
			for (y = 1, lastdiag = x - 1; y <= s1len; y++) {
				olddiag = column[y];
				column[y] = SpellMIN3(column[y] + 1, column[y - 1] + 1, lastdiag + (s1[y - 1] == s2[x - 1] ? 0 : 1));
				lastdiag = olddiag;
			}
		}
		auto _ret = (column[s1len]);
		delete[] column;
		return _ret;
	}
	/*
	情况一;
	       left = "pnt";
		   right = "pdddddddddddddddnt"
		   这样的情况下left在right中
	情况二;
		   left = "pnt";
		   right = "pntddddddddddddddnt"
		   这样的情况下left也在right中

		   这个函数要那个判断情况二的选择优于情况一
		    

	所以这个问题能概括为
	 right 有 h个点, 
	 left  有 n个点
	查找left 的所有点是否顺序在right中出现

	如果left在right中出现
	这个可以概括为一下:
	   n 个点 {  X0,...,Xi,...,Xn}  其中 任意的Xi是  Left 在right中 找到的位置对应下标
	 
	   我们情况 {  X0,...,Xi,...,Xn}  出现在 right 的越靠左侧的越是可能是 更优的匹配
	*/

	//static double  IsLeftStringOrderInRightString(const wchar_t* left, const wchar_t* right, bool isCaseSensitive)
	//{
	//	int leftLen = wcslen(left),
	//		rightLen = wcslen(right);
	//	if (leftLen > rightLen) {
	//		return 0;
	//	}
	//	unsigned int* matchLoc = new unsigned int[leftLen + 2];// 小标为0不适用

	//	int leftInex = 0, rightIndex = 0;
	//	if (isCaseSensitive) {
	//		while ((leftInex < leftLen) && (rightIndex < rightLen)) {
	//			if (left[leftInex] == right[rightIndex]) {
	//				matchLoc[leftInex] = rightIndex;
	//				++leftInex;
	//				++rightIndex;
	//			}
	//			else
	//			{
	//				++rightIndex;
	//			}
	//		}
	//	}
	//	else {
	//		while ((leftInex < leftLen) && (rightIndex < rightLen)) {
	//			if (tolower(left[leftInex]) == tolower(right[rightIndex])) {
	//				matchLoc[leftInex] = rightIndex;
	//				++rightIndex;
	//				++leftInex;
	//			}
	//			else
	//			{
	//				++rightIndex;
	//			}

	//		}

	//	}
	//	if (leftInex < leftLen) {
	//		delete[] matchLoc;
	//		return 0.0;
	//	}
	//	else {
	//		int sumMatchXi = 0;
	//		for (int i = 0; i < leftLen; ++i) {
	//			int loc = matchLoc[i];
	//			sumMatchXi += (rightLen - loc)*(rightLen - loc);
	//		}
	//		int sumXnToX0 = 0;
	//		for (int i = 0; i < rightLen; ++i) {
	//			sumXnToX0 += i*i;
	//		}	
	//		delete[] matchLoc;
	//		return  (sumMatchXi/ (sumXnToX0 *1.0));
	//	}
	//}
	static double  IsLeftStringOrderInRightStringHelper(const wchar_t* left, int leftLen, const wchar_t* right, int rightLen)
	{


		unsigned int* matchLoc = new unsigned int[leftLen + 2];// 小标为0不适用
		double* weight = new  double[leftLen + 2];// 小标为0不适用

		int leftInex = 0, rightIndex = 0;
	
		while ((leftInex < leftLen) && (rightIndex < rightLen)) {
			if (left[leftInex] == right[rightIndex]) {
				matchLoc[leftInex] = rightIndex;
				weight[leftInex] = 1;
				++leftInex;
				++rightIndex;
			}
			else if(tolower(left[leftInex]) == tolower(right[rightIndex]))
			{
				matchLoc[leftInex] = rightIndex;
				weight[leftInex] = 0.9;
				++leftInex;
				++rightIndex;
			}
			else
			{
				++rightIndex;
			}

		}
		if (leftInex < leftLen) {
			delete[] matchLoc;
			delete[] weight;
			return 0.0;
		}
		else {


			int Xn = matchLoc[leftLen - 1];
			int X0 = matchLoc[0];

			int n = leftLen;
			double doubleH = (rightLen) * 1.0;
			double w1 = 1;
			double  cond1_score = w1 * (n - (Xn - X0 - Xn*Xn / doubleH + X0*X0 / doubleH)*1.0 / (doubleH));

			double w2 = 1;
			double sumXnToX0 = 0;

			for (int i = 0; i < leftLen; ++i)
			{
				
				int loc = matchLoc[i];
			    int  value = (rightLen - loc);
				sumXnToX0 += value* value * weight[i];
			}


			double  cond2_score = sumXnToX0;
			delete[] matchLoc;
			delete[] weight;
			return  (cond2_score + cond1_score);
		}

	}
	static double  IsLeftStringOrderInRightString(const wchar_t* left, const wchar_t* right)
	{
		int leftLen = wcslen(left),
			rightLen = wcslen(right);
		if (leftLen > rightLen) {
			return 0;
		}


		double max;
		int Xn = rightLen - 1;
		int X0 = 0;
		int n = rightLen;
		double doubleH = (rightLen -1) * 1.0;
		double  cond1_score = (n - (Xn - X0 - Xn*Xn / doubleH + X0*X0 / doubleH)*1.0 / (doubleH));

		//double sumXnToX0 = 0;

		//for (int i = 0; i < rightLen; ++i) {
		//	sumXnToX0 += i*i;
		//}
		double sumXnToX0 = (n -1)*(n)*(2 *( n  -1)+ 1)/6.0;
		double  cond2_score = sumXnToX0;
		max = (cond2_score + cond1_score);
		auto test = IsLeftStringOrderInRightStringHelper(left, leftLen, right, rightLen);
		return  test / max;

	}

    static int Index(const wchar_t* str1, const wchar_t* str2)
    {
        int len1 = wcslen(str1),
            len2 = wcslen(str2);

        wchar_t *s1 = new wchar_t[len1 + 1],
                *s2 = new wchar_t[len2 + 1];

        for (int i = 0; i < len1; i++)
            s1[i] = Case::ToAsciiLower(str1[i]);
        s1[len1] = U_NULL;

        for (int j = 0; j < len2; j++)
            s2[j] = Case::ToAsciiLower(str2[j]);
        s2[len2] = U_NULL;

        if (len1 == 1 && len2 == 1)
        {
            //
            //  Singleton mispellings:
            //
            //  ;      <---->     ,
            //
            //  ;      <---->     :
            //
            //  .      <---->     ,
            //
            //  '      <---->     "
            //
            if ((s1[0] == U_SEMICOLON    &&  s2[0] == U_COMMA)  ||
                (s1[0] == U_COMMA        &&  s2[0] == U_SEMICOLON)  ||
                (s1[0] == U_SEMICOLON    &&  s2[0] == U_COLON)  ||
                (s1[0] == U_COLON        &&  s2[0] == U_SEMICOLON)  ||
                (s1[0] == U_DOT          &&  s2[0] == U_COMMA)  ||
                (s1[0] == U_COMMA        &&  s2[0] == U_DOT)  ||
                (s1[0] == U_SINGLE_QUOTE &&  s2[0] == U_DOUBLE_QUOTE)  ||
                (s1[0] == U_DOUBLE_QUOTE &&  s2[0] == U_SINGLE_QUOTE))
            {
				delete[] s1;
				delete[] s2;
				return 3;
            }
                    
        }

        //
        // Scan the two strings. Increment "match" count for each match.
        // When a transposition is encountered, increase "match" count
        // by two but count it as an error. When a typo is found, skip
        // it and count it as an error. Otherwise we have a mismatch; if
        // one of the strings is longer, increment its index, otherwise,
        // increment both indices and continue.
        //
        // This algorithm is an adaptation of a boolean misspelling
        // algorithm proposed by Juergen Uhl.
        //
        int count = 0,
            prefix_length = 0,
            num_errors = 0,
            i1 = 0,
            i2 = 0;

        while ((i1 < len1)  &&  (i2 < len2))
        {
            if (s1[i1] == s2[i2])
            {
                count++;
                i1++;
                i2++;
                if (num_errors == 0)
                    prefix_length++;
            }
            else if (s1[i1 + 1] == s2[i2]  &&  s1[i1] == s2[i2 + 1])
            {
                count += 2;
                i1 += 2;
                i2 += 2;
                num_errors++;
            }
            else if (s1[i1 + 1] == s2[i2 + 1])
            {
                i1++;
                i2++;
                num_errors++;
            }
            else
            {
                if ((len1 - i1) > (len2 - i2))
                     i1++;
                else if ((len2 - i2) > (len1 - i1))
                     i2++;
                else
                {
                    i1++;
                    i2++;
                }
                num_errors++;
            }
        }

        if (i1 < len1  ||  i2 < len2)
            num_errors++;

        if (num_errors > (Min(len1, len2) / 6 + 1))
             count = prefix_length;

        delete [] s1;
        delete [] s2;

        return (count * 10 / (len1 + num_errors));
    }
};


} // Close namespace Jikes block


#endif // spell_INCLUDED

