//////////////////////////////////////////////////////////////////////
//
// shape.h:
//
// PWH
// 2017.07.20
// 2021.05.27
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/*

	1. CShapeObject ���� ���� ���� :

		class name :

			CShapeObject	-> s_shape
			CShapeLine		-> s_line
			CShapePolyLine	-> s_polyline
			...

		Container :

			TList -> boost::ptr_deque

		Point / Line : x, y, z 3�������� ����

			typedef CSize3d					size2d_t;
			typedef CPoint3d				point_t;
			typedef CRect3d					rect_t;
			typedef std::vector<s_line>		s_lines;

*/
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "gtl/iconv_wrapper.h"

#include "canvas.h"
#include "shape_primitives.h"
#include "shape_others.h"

//export module shape;

namespace gtl::shape {


}
