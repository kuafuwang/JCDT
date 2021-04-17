// $Id: option.h,v 1.43 2004/10/10 23:17:44 elliott-oss Exp $ -*- c++ -*-
//
// This software is subject to the terms of the IBM Jikes Compiler
// License Agreement available at the following URL:
// http://ibm.com/developerworks/opensource/jikes.
// Copyright (C) 1996, 1998, 1999, 2000, 2001, 2002 International Business
// Machines Corporation and others.  All Rights Reserved.
// You must accept the terms of that agreement to use this software.
//

#ifndef PositionNode_INCLUDED
#define PositionNode_INCLUDED


namespace Jikes { // Open namespace Jikes block


	class PositionNode
	{
	public:
		static const int NO_FOUND = -1;
		virtual ~PositionNode()
		{
		}
		enum 
		{
			LEFT_HAND_LESS = -1,
			EQUAL = 0,
			LEFT_HAND_LARGE = 1
		};
		virtual int Compare(unsigned int pos) = 0;
	};

	template< typename T>
	int BinarySearch(T** pData, int n, int key)
	{
		int center, left = 0, right = n - 1;

		while (left <= right)
		{
			center = (left + right) / 2;


			int compare_ret = pData[center]->Compare(key);
			if (compare_ret == PositionNode::LEFT_HAND_LARGE) { // pData[center] > key
				right = center - 1;
			}
			else if (compare_ret == PositionNode::LEFT_HAND_LESS)// // pData[center] < key
			{
				left = center + 1;
			}
			else
				return center;

		}
		return PositionNode::NO_FOUND;
	}
} // Close namespace Jikes block


#endif // PositionNode_INCLUDED

