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
}