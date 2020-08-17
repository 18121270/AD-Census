/* -*-c++-*- AD-Census - Copyright (C) 2020.
* Author	: Ethan Li <ethan.li.whu@gmail.com>
*			  https://github.com/ethan-li-coding/AD-Census
* Describe	: header of ad-census stereo class
*/
#pragma once

#include "adcensus_types.h"
#include "cross_aggregator.h"

class ADCensusStereo {	
public:
	ADCensusStereo();
	~ADCensusStereo();

	/**
	* \brief ��ĳ�ʼ�������һЩ�ڴ��Ԥ���䡢������Ԥ���õ�
	* \param width		���룬�������Ӱ���
	* \param height		���룬�������Ӱ���
	* \param option		���룬�㷨����
	*/
	bool Initialize(const sint32& width, const sint32& height, const ADCensusOption& option);

	/**
	* \brief ִ��ƥ��
	* \param img_left	���룬��Ӱ������ָ�룬3ͨ����ɫ����
	* \param img_right	���룬��Ӱ������ָ�룬3ͨ����ɫ����
	* \param disp_left	�������Ӱ���Ӳ�ͼָ�룬Ԥ�ȷ����Ӱ��ȳߴ���ڴ�ռ�
	*/
	bool Match(const uint8* img_left, const uint8* img_right, float32* disp_left);

	/**
	* \brief ����
	* \param width		���룬�������Ӱ���
	* \param height		���룬�������Ӱ���
	* \param option		���룬�㷨����
	*/
	bool Reset(const uint32& width, const uint32& height, const ADCensusOption& option);

private:
	/** \brief ����Ҷ����� */
	void ComputeGray() const;

	/** \brief Census�任 */
	void CensusTransform() const;

	/** \brief ���ۼ���	 */
	void ComputeCost() const;

	/** \brief ���۾ۺ�	 */
	void CostAggregation();

	/** \brief ɨ�����Ż�	 */
	void ScanlineOptimize() const;

	/** \brief �ಽ���Ӳ��Ż�	 */
	void MultiStepRefine();

	/** \brief �Ӳ���㣨����ͼ�� */
	void ComputeDisparity() const;

	/** \brief �Ӳ���㣨����ͼ�� */
	void ComputeDisparityRight() const;

	/** \brief �ڴ��ͷ�	 */
	void Release();

private:
	//------5С���Ӳ��Ż�------//
	/** \brief ��Ⱥ����	 */
	void OutlierDetection();
	/** \brief �����ֲ�ͶƱ */
	void IterativeRegionVoting();
	/** \brief �ڲ���� */
	void ProperInterpolation();
	/** \brief ��ȷ��������Ӳ���� */
	void DepthDiscontinuityAdjustment();
	/** \brief �������Ż� */
	void SubpixelEnhancement();

private:
	/** \brief �㷨����	 */
	ADCensusOption option_;

	/** \brief Ӱ���	 */
	sint32 width_;

	/** \brief Ӱ���	 */
	sint32 height_;

	/** \brief ��Ӱ�����ݣ�3ͨ����ɫ����	 */
	const uint8* img_left_;
	/** \brief ��Ӱ������	��3ͨ����ɫ���� */
	const uint8* img_right_;

	/** \brief ��Ӱ��Ҷ�����	 */
	uint8* gray_left_;
	/** \brief ��Ӱ��Ҷ�����	 */
	uint8* gray_right_;

	/** \brief ��Ӱ��censusֵ	*/
	uint64* census_left_;
	/** \brief ��Ӱ��censusֵ	*/
	uint64* census_right_;

	/** \brief ��ʼƥ�����	*/
	float32* cost_init_;
	/** \brief �ۺ�ƥ�����	*/
	float32* cost_aggr_;

	/** \brief ���۾ۺ���	*/
	CrossAggregator aggregator_;

	/** \brief ��Ӱ���Ӳ�ͼ	*/
	float32* disp_left_;
	/** \brief ��Ӱ���Ӳ�ͼ	*/
	float32* disp_right_;

	/** \brief �ڵ������ؼ�	*/
	std::vector<std::pair<int, int>> occlusions_;
	/** \brief ��ƥ�������ؼ�	*/
	std::vector<std::pair<int, int>> mismatches_;

	/** \brief �Ƿ��ʼ����־	*/
	bool is_initialized_;
};
