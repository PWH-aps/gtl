/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MatHelper.h : cv::Mat, CDC,...
//
// PWH.
//
// 2021.06.11 from Mocha xMathUtil
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "gtl/win_util/win_util.h"
#include "opencv2/opencv.hpp"


namespace gtl::win_util {
#pragma pack(push, 8)	// default align. (8 bytes)


	//-----------------------------------------------------------------------------
	// Mat Load/Store
	template < class ARCHIVE >
	bool LoadMat(ARCHIVE& ar, cv::Mat& mat) {
		CStringA str;
		ar >> str;
		if (str != "mat")
			return FALSE;

		int rows, cols, type;
		ar >> rows >> cols >> type;
		if ( (rows < 0) || (cols < 0) )
			return FALSE;
		if ( (rows == 0) || (cols == 0) ) {
			mat.release();
			return TRUE;
		}
		mat = cv::Mat::zeros(rows, cols, type);
		if (mat.empty())
			return FALSE;
		for (int i = 0; i < rows; i++) {
			ar.read((char*)mat.ptr(i), (UINT)(mat.cols*mat.elemSize()));	// step -> (cols*elemSize())
		}
		return TRUE;
	}
	template < class ARCHIVE >
	bool StoreMat(ARCHIVE& ar, cv::Mat const& mat) {
		gtl::CStringA str("mat");
		ar << str;

		ar << mat.rows;
		ar << mat.cols;
		ar << mat.type();
		for (int i = 0; i < mat.rows; i++) {
			ar.write((char*)mat.ptr(i), mat.cols*mat.elemSize());	// step -> (cols*elemSize())
		}

		return true;
	}
	static inline bool ReadMat(std::istream& file, cv::Mat& mat) {
		auto ReadVar = [&file, &mat] (auto& var) -> bool {
			return (bool)file.read((char*)&var, sizeof(var));
		};

		do {
			int rows{}, cols{}, type{};
			// read sign
			{
				uint8_t buf[3];
				uint8_t b = (uint8_t)sizeof(buf);
				if (!ReadVar(b))
					break;
				if (b != sizeof(buf))
					break;
				if (!ReadVar(buf))
					break;
				if (memcmp(buf, "mat", b) != 0)
					break;
			}

			if (!ReadVar(rows)) break;
			if (!ReadVar(cols)) break;
			if (!ReadVar(type)) break;

			if ( (rows < 0) || (cols < 0) )
				break;
			if ( (rows == 0) || (cols == 0) ) {
				mat.release();
				return true;
			}

			mat = cv::Mat::zeros(rows, cols, type);
			if (mat.empty())
				break;

			for (int i = 0; i < rows; i++)
				file.read((char*)mat.ptr(i), (mat.cols*mat.elemSize()));	// step -> (cols*elemSize())

		} while (false);

		return true;
	}
	static inline bool SaveMat(std::ostream& file, cv::Mat const& mat) {
		uint8_t buf[4] = { 3, 'm', 'a', 't' };
		file.write((char const*)buf, sizeof(buf));
		file.write((char const*)&mat.rows, sizeof(mat.rows));
		file.write((char const*)&mat.cols, sizeof(mat.cols));
		int type = mat.type();
		file.write((char const*)&type, sizeof(type));
		for (int i = 0; i < mat.rows; i++) {
			file.write((char*)mat.ptr(i), mat.cols*mat.elemSize());	// step -> (cols*elemSize())
		}

		return true;
	}


	//-----------------------------------------------------------------------------
	// �ܰ��� ����
	template <class TSTR >
	void putTextC(cv::InputOutputArray img, TSTR& psz, cv::Point org,
				  int fontFace, double fontScale, cv::Scalar color,
				  int thickness = 1, int lineType = 8,
				  bool bottomLeftOrigin = false, bool bOutline = true)
	{
		CStringA str(psz);
		cv::Size size = cv::getTextSize((LPCSTR)str, fontFace, fontScale, thickness, NULL);

		if (bOutline) {
			cv::Scalar crBkgnd;
			crBkgnd = cv::Scalar(255, 255, 255) - color;
			//if (crBkgnd == Scalar(0, 0, 0))
			//	crBkgnd = Scalar(1, 1, 1);
			int iShift = std::max(1, thickness/2);
			cv::putText(img, (LPCSTR)str, CPoint2i(org)-CSize2i(size)/2+CPoint2i(0, iShift), fontFace, fontScale, crBkgnd, thickness, lineType, bottomLeftOrigin);
			cv::putText(img, (LPCSTR)str, CPoint2i(org)-CSize2i(size)/2+CPoint2i(iShift, 0), fontFace, fontScale, crBkgnd, thickness, lineType, bottomLeftOrigin);
			cv::putText(img, (LPCSTR)str, CPoint2i(org)-CSize2i(size)/2+CPoint2i(-iShift, 0), fontFace, fontScale, crBkgnd, thickness, lineType, bottomLeftOrigin);
			cv::putText(img, (LPCSTR)str, CPoint2i(org)-CSize2i(size)/2+CPoint2i(0, -iShift), fontFace, fontScale, crBkgnd, thickness, lineType, bottomLeftOrigin);
		}

		cv::putText(img, (LPCSTR)str, CPoint2i(org)-CSize2i(size)/2, fontFace, fontScale, color, thickness, lineType, bottomLeftOrigin);
	}


	//-----------------------------------------------------------------------------
	// Mat 
	GTL_WINUTIL_API bool CheckGPU(bool bUse);
	GTL_WINUTIL_API bool IsGPUEnabled();
	GTL_WINUTIL_API bool ConvertColor(cv::Mat const& imgSrc, cv::Mat& imgDest, int eCode);
	GTL_WINUTIL_API bool ResizeImage(cv::Mat const& imgSrc, cv::Mat& imgDest, double dScale, int eInterpolation = cv::INTER_LINEAR/*, int eScaleDownMethod = cv::INTER_AREA*/);
	GTL_WINUTIL_API bool MatchTemplate(cv::Mat const& img, cv::Mat const& imgTempl, cv::Mat& matResult, int method);
	GTL_WINUTIL_API bool MatchTemplate(cv::Mat const& img, cv::Mat const& imgTempl, int method, CPoint2d& ptBest, double& dMinMax, double& dRate, double dScale = 0.0, int eInterpolation = cv::INTER_LINEAR);

	//-----------------------------------------------------------------------------
	// Mat to DC
	template < typename TRECT >
	bool CalcViewPosition(cv::Size const& sizeView, TRECT const& rectView, TRECT& rectImage/* out */, TRECT& rectDst/* out */)	// image size -> display rect and source rect
	{
		if (rectView.IsRectEmpty()) {
			rectImage.SetRectEmpty();
			rectDst.SetRectEmpty();
			return false;
		}
		auto wDest = rectView.Width();
		auto hDest = rectView.Height();

		if (wDest >= sizeView.width) {
			rectDst.left = rectView.left + (wDest - sizeView.width) / 2;
			rectDst.right = rectDst.left + sizeView.width;
			rectImage.left = 0;
			rectImage.right = sizeView.width;
		}
		else if (wDest < sizeView.width) {
			rectDst.left = rectView.left;
			rectDst.right = rectView.right;
			rectImage.left = (sizeView.width - wDest) / 2;
			rectImage.right = rectImage.left + wDest;
		}

		if (hDest >= sizeView.height) {
			rectDst.top = rectView.top + (hDest - sizeView.height) / 2;
			rectDst.bottom = rectDst.top + sizeView.height;
			rectImage.top = 0;
			rectImage.bottom = sizeView.height;
		}
		else if (hDest < sizeView.height) {
			rectDst.top = rectView.top;
			rectDst.bottom = rectView.bottom;
			rectImage.top = (sizeView.height - hDest) / 2;
			rectImage.bottom = rectImage.top + hDest;
		}

		return true;

	}
	GTL_WINUTIL_API bool MatToDC            (cv::Mat const& img, cv::Size const& sizeView, CDC& dc, CRect const& rect, std::span<RGBQUAD> palette = {});
	GTL_WINUTIL_API bool MatToDC            (cv::Mat const& img, cv::Size const& sizeView, CDC& dc, CRect const& rect, CBitmap const& mask);	// mask : monochrome bitmap, background of img value must be zero.
	GTL_WINUTIL_API bool MatToDCTransparent (cv::Mat const& img, cv::Size const& sizeView, CDC& dc, CRect const& rect, COLORREF crTransparent);
	GTL_WINUTIL_API bool MatToDCAlphaBlend  (cv::Mat const& img, cv::Size const& sizeView, CDC& dc, CRect const& rect, BLENDFUNCTION blend);
	GTL_WINUTIL_API bool MatToMatTransparent(cv::Mat const& imgSource, cv::Mat& imgTarget, cv::Mat const& matMask);
	GTL_WINUTIL_API bool MatToMatTransparent(cv::Mat const& imgSource, cv::Mat& imgTarget, cv::Scalar const& crTransparent);

	GTL_WINUTIL_API bool SaveBitmap(std::filesystem::path const& path, cv::Mat const& img, int nBPP, std::span<RGBQUAD> palette);
	inline cv::Mat LoadImage(std::filesystem::path const& path) {
		cv::Mat img;
		auto buf = FileToBuffer<uint8_t>(path);
		if (!buf)
			return img;

		img = cv::imdecode(*buf, 0);
		return img;
	}


#pragma pack(pop)
}
