/* -*-c++-*- AD-Census - Copyright (C) 2020.
* Author	: Ethan Li<ethan.li.whu@gmail.com>
* https://github.com/ethan-li-coding/AD-Census
* Describe	: implement of class MultiStepRefiner
*/
#ifndef AD_CENSUS_MULTISTEP_REFINER_H_
#define AD_CENSUS_MULTISTEP_REFINER_H_

#include "adcensus_types.h"
#include "cross_aggregator.h"

class MultiStepRefiner
{
public:
	MultiStepRefiner();
	~MultiStepRefiner();

	/**
	 * \brief ��ʼ��
	 * \param width		Ӱ���
	 * \param height	Ӱ���
	 * \return true:��ʼ���ɹ�
	 */
	bool Initialize(const sint32& width, const sint32& height);

	/**
	 * \brief ���öಽ�Ż�������
	 * \param cost				// ��������
	 * \param cross_arms		// ʮ�ֽ��������
	 * \param disp_left			// ����ͼ�Ӳ�����
	 * \param disp_right		// ����ͼ�Ӳ�����
	 */
	void SetData(const float32* cost, const vector<CrossArm>* cross_arms, float32* disp_left, float32* disp_right);


	/**
	 * \brief ���öಽ�Ż��Ĳ���
	 * \param min_disparity	// ��С�Ӳ�
	 * \param max_disparity	// ����Ӳ�
	 * \param irv_ts		// Iterative Region Voting����ts
	 * \param irv_th		// Iterative Region Voting����th
	 * \param lrcheck_thres	// һ���Լ����ֵ
	 */
	void SetParam(const sint32& min_disparity, const sint32& max_disparity, const sint32& irv_ts, const float32& irv_th, const float32& lrcheck_thres);

	void Refine();

private:
	//------4С���Ӳ��Ż�------//
	/** \brief ��Ⱥ����	 */
	void OutlierDetection();
	/** \brief �����ֲ�ͶƱ */
	void IterativeRegionVoting();
	/** \brief �ڲ���� */
	void ProperInterpolation();
	/** \brief ��ȷ��������Ӳ���� */
	void DepthDiscontinuityAdjustment();

private:
	/** \brief ͼ��ߴ� */
	sint32	width_;
	sint32	height_;

	/** \brief �������� */
	const float32* cost_;
	/** \brief ��������� */
	const vector<CrossArm>* vec_cross_arms_;

	/** \brief ����ͼ�Ӳ����� */
	float* disp_left_;
	/** \brief ����ͼ�Ӳ����� */
	float* disp_right_;

	/** \brief ��С�Ӳ�ֵ */
	sint32 min_disparity_;
	/** \brief ����Ӳ�ֵ */
	sint32 max_disparity_;

	/** \brief Iterative Region Voting����ts */
	sint32	irv_ts_;
	/** \brief Iterative Region Voting����th */
	float32 irv_th_;

	/** \brief һ���Լ����ֵ */
	float32 lrcheck_thres_;

	/** \brief �ڵ������ؼ�	*/
	std::vector<std::pair<int, int>> occlusions_;
	/** \brief ��ƥ�������ؼ�	*/
	std::vector<std::pair<int, int>> mismatches_;
};
#endif
