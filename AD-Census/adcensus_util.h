/* -*-c++-*- AD-Census - Copyright (C) 2020.
* Author	: Ethan Li<ethan.li.whu@gmail.com>
* https://github.com/ethan-li-coding/AD-Census
* Describe	: header of adcensus_util
*/

#pragma once
#include <algorithm>
#include "adcensus_types.h"


namespace adcensus_util
{
	/**
	* \brief census�任
	* \param source	���룬Ӱ������
	* \param census	�����censusֵ����
	* \param width	���룬Ӱ���
	* \param height	���룬Ӱ���
	*/
	void census_transform_9x7(const uint8* source, uint64* census, const sint32& width, const sint32& height);
	// Hamming����
	uint8 Hamming64(const uint64& x, const uint64& y);

	/**
	* \brief ����·���ۺ� �� ��
	* \param img_left			���룬��Ӱ������
	* \param img_right			���룬��Ӱ������
	* \param width				���룬Ӱ���
	* \param height				���룬Ӱ���
	* \param min_disparity		���룬��С�Ӳ�
	* \param max_disparity		���룬����Ӳ�
	* \param p1					���룬�ͷ���P1
	* \param p2					���룬�ͷ���P2_Init
	* \param tso				���룬�ͷ�����ֵ
	* \param cost_init			���룬��ʼ��������
	* \param cost_aggr			�����·���ۺϴ�������
	* \param is_forward			���룬�Ƿ�Ϊ������������Ϊ�����ң�������Ϊ���ҵ���
	*/
	void CostAggregateLeftRight(const uint8* img_left, const uint8* img_right, const sint32& width, const sint32& height, const sint32& min_disparity, const sint32& max_disparity,
		const float32& p1, const float32& p2, const sint32& tso, const float32* cost_init, float32* cost_aggr, bool is_forward = true);

	/**
	* \brief ����·���ۺ� �� ��
	* \param img_left			���룬��Ӱ������
	* \param img_right			���룬��Ӱ������
	* \param width				���룬Ӱ���
	* \param height				���룬Ӱ���
	* \param min_disparity		���룬��С�Ӳ�
	* \param max_disparity		���룬����Ӳ�
	* \param p1					���룬�ͷ���P1
	* \param p2					���룬�ͷ���P2_Init
	* \param tso				���룬�ͷ�����ֵ
	* \param cost_init			���룬��ʼ��������
	* \param cost_aggr			�����·���ۺϴ�������
	* \param is_forward			���룬�Ƿ�Ϊ������������Ϊ���ϵ��£�������Ϊ���µ��ϣ�
	*/
	void CostAggregateUpDown(const uint8* img_left, const uint8* img_right, const sint32& width, const sint32& height, const sint32& min_disparity, const sint32& max_disparity,
		const float32& p1, const float32& p2, const sint32& tso, const float32* cost_init, float32* cost_aggr, bool is_forward = true);

	/** \brief ������ɫ���� */
	inline sint32 ColorDist(const ADColor& c1, const ADColor& c2) {
		return std::max(abs(c1.r - c2.r), std::max(abs(c1.g - c2.g), abs(c1.b - c2.b)));
	}
}