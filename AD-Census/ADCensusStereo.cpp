/* -*-c++-*- AD-Census - Copyright (C) 2020.
* Author	: Ethan Li <ethan.li.whu@gmail.com>
*			  https://github.com/ethan-li-coding
* Describe	: implement of ad-census stereo class
*/
#include "ADCensusStereo.h"

ADCensusStereo::ADCensusStereo()
{
	
}

ADCensusStereo::~ADCensusStereo()
{
	Release();
	is_initialized_ = false;
}

bool ADCensusStereo::Initialize(const sint32& width, const sint32& height, const ADCensusOption& option)
{
	// ������ ��ֵ

	// Ӱ��ߴ�
	width_ = width;
	height_ = height;
	// �㷨����
	option_ = option;

	if (width <= 0 || height <= 0) {
		return false;
	}

	//������ �����ڴ�ռ�
	const sint32 img_size = width * height;
	const sint32 disp_range = option.max_disparity - option.min_disparity;
	if (disp_range <= 0) {
		return false;
	}

	// �Ҷ�����
	gray_left_ = new uint8[img_size];
	gray_right_ = new uint8[img_size];
	// ��������
	cost_init_ = new float32[img_size*disp_range];
	cost_aggr_ = new float32[img_size*disp_range];
	// �Ӳ�ͼ
	disp_left_ = new float32[img_size];
	disp_right_ = new float32[img_size];

	is_initialized_ = gray_left_ && gray_right_ && cost_init_ && cost_aggr_  && disp_left_ && disp_right_;

	return is_initialized_;
}

bool ADCensusStereo::Match(const uint8* img_left, const uint8* img_right, float32* disp_left)
{
	if (!is_initialized_) {
		return false;
	}
	if (img_left == nullptr || img_right == nullptr) {
		return false;
	}

	img_left_ = img_left;
	img_right_ = img_right;

	return true;
}

bool ADCensusStereo::Reset(const uint32& width, const uint32& height, const ADCensusOption& option)
{
	// �ͷ��ڴ�
	Release();

	// ���ó�ʼ�����
	is_initialized_ = false;

	// ��ʼ��
	return Initialize(width, height, option);
}

void ADCensusStereo::CensusTransform() const
{
	
}

void ADCensusStereo::ComputeCost() const
{
	
}

void ADCensusStereo::CostAggregation() const
{
	
}

void ADCensusStereo::ScanlineOptimize() const
{
	
}

void ADCensusStereo::MultiStepRefine() const
{
	
}

void ADCensusStereo::Release()
{
	SAFE_DELETE(gray_left_);
	SAFE_DELETE(gray_right_);
	SAFE_DELETE(cost_init_);
	SAFE_DELETE(cost_aggr_);
	SAFE_DELETE(disp_left_);
	SAFE_DELETE(disp_right_);
}

void ADCensusStereo::OutlierDetection()
{
	
}

void ADCensusStereo::IterativeRegionVoting()
{
	
}

void ADCensusStereo::ProperInterpolation()
{
	
}

void ADCensusStereo::DepthDiscontinuityAdjustment()
{
	
}

void ADCensusStereo::SubpixelEnhancement()
{
	
}

