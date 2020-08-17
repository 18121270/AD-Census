/* -*-c++-*- AD-Census - Copyright (C) 2020.
* Author	: Ethan Li <ethan.li.whu@gmail.com>
* https://github.com/ethan-li-coding/AD-Census
* Describe	: implement of adcensus_util
*/

#include "adcensus_util.h"
#include <cassert>
#include <algorithm>

void adcensus_util::census_transform_9x7(const uint8* source, uint64* census, const sint32& width,
	const sint32& height)
{
	if (source == nullptr || census == nullptr || width <= 9 || height <= 7) {
		return;
	}

	// �����ؼ���censusֵ
	for (sint32 i = 4; i < height - 4; i++) {
		for (sint32 j = 3; j < width - 3; j++) {

			// ��������ֵ
			const uint8 gray_center = source[i * width + j];

			// ������СΪ5x5�Ĵ������������أ���һ�Ƚ�����ֵ����������ֵ�ĵĴ�С������censusֵ
			uint64 census_val = 0u;
			for (sint32 r = -4; r <= 4; r++) {
				for (sint32 c = -3; c <= 3; c++) {
					census_val <<= 1;
					const uint8 gray = source[(i + r) * width + j + c];
					if (gray < gray_center) {
						census_val += 1;
					}
				}
			}

			// �������ص�censusֵ
			census[i * width + j] = census_val;
		}
	}
}


uint8 adcensus_util::Hamming64(const uint64& x, const uint64& y)
{
	uint64 dist = 0, val = x ^ y;

	// Count the number of set bits
	while (val) {
		++dist;
		val &= val - 1;
	}

	return static_cast<uint8>(dist);
}

void adcensus_util::CostAggregateLeftRight(const uint8* img_left, const uint8* img_right, const sint32& width, const sint32& height, const sint32& min_disparity, const sint32& max_disparity,
	const float32& p1, const float32& p2, const sint32& tso, const float32* cost_init, float32* cost_aggr, bool is_forward)
{
	assert(width > 0 && height > 0 && max_disparity > min_disparity);

	// �ӲΧ
	const sint32 disp_range = max_disparity - min_disparity;

	// ����(��->��) ��is_forward = true ; direction = 1
	// ����(��->��) ��is_forward = false; direction = -1;
	const sint32 direction = is_forward ? 1 : -1;

	// �ۺ�
	for (sint32 y = 0u; y < height; y++) {
		// ·��ͷΪÿһ�е���(β,dir=-1)������
		auto cost_init_row = (is_forward) ? (cost_init + y * width * disp_range) : (cost_init + y * width * disp_range + (width - 1) * disp_range);
		auto cost_aggr_row = (is_forward) ? (cost_aggr + y * width * disp_range) : (cost_aggr + y * width * disp_range + (width - 1) * disp_range);
		auto img_row = (is_forward) ? (img_left + y * width * 3) : (img_left + y * width * 3 + 3 * (width - 1));
		const auto img_row_r = img_right + y * width * 3;
		sint32 x = (is_forward) ? 0 : width - 1;

		// ·���ϵ�ǰ��ɫֵ����һ����ɫֵ
		ADColor color(img_row[0], img_row[1], img_row[2]);
		ADColor color_last = color;

		// ·�����ϸ����صĴ������飬������Ԫ����Ϊ�˱���߽��������β����һ����
		std::vector<float32> cost_last_path(disp_range + 2, Large_Float);

		// ��ʼ������һ�����صľۺϴ���ֵ���ڳ�ʼ����ֵ
		memcpy(cost_aggr_row, cost_init_row, disp_range * sizeof(float32));
		memcpy(&cost_last_path[1], cost_aggr_row, disp_range * sizeof(float32));
		cost_init_row += direction * disp_range;
		cost_aggr_row += direction * disp_range;
		img_row += direction * 3;
		x += direction;

		// ·�����ϸ����ص���С����ֵ
		float32 mincost_last_path = Large_Float;
		for (auto cost : cost_last_path) {
			mincost_last_path = std::min(mincost_last_path, cost);
		}

		// �Է����ϵ�2�����ؿ�ʼ��˳��ۺ�
		for (sint32 j = 0; j < width - 1; j++) {
			color = ADColor(img_row[0], img_row[1], img_row[2]);
			float32 min_cost = Large_Float;
			for (sint32 d = 0; d < disp_range; d++) {
				const sint32 xr = x - d;
				if (xr <= 0 || xr >= width - 1) {
					cost_aggr_row[d] = Large_Float;
					continue;
				}
				const ADColor color_r = ADColor(img_row_r[3 * xr], img_row_r[3 * xr + 1], img_row_r[3 * xr + 2]);
				const ADColor color_last_r = ADColor(img_row_r[3 * (xr - direction)], img_row_r[3 * (xr - direction) + 1], img_row_r[3 * (xr - direction) + 2]);
				const uint8 d1 = std::max(abs(color.r - color_last.r), std::max(abs(color.g - color_last.g), abs(color.b - color_last.b)));
				const uint8 d2 = std::max(abs(color_r.r - color_last_r.r), std::max(abs(color_r.g - color_last_r.g), abs(color_r.b - color_last_r.b)));
				
				float32 P1(0.0f), P2(0.0f);
				if (d1 < tso && d2 < tso) {
					P1 = p1; P2 = p2;
				}
				else if (d1 < tso && d2 > tso) {
					P1 = p1 / 4; P2 = p2 / 4;
				}
				else if (d1 > tso && d2 < tso) {
					P1 = p1 / 4; P2 = p2 / 4;
				}
				else if (d1 > tso && d2 > tso) {
					P1 = p1 / 10; P2 = p2 / 10;
				}

				// Lr(p,d) = C(p,d) + min( Lr(p-r,d), Lr(p-r,d-1) + P1, Lr(p-r,d+1) + P1, min(Lr(p-r))+P2 ) - min(Lr(p-r))
				const float32  cost = cost_init_row[d];
				const float32 l1 = cost_last_path[d + 1];
				const float32 l2 = cost_last_path[d] + P1;
				const float32 l3 = cost_last_path[d + 2] + P1;
				const float32 l4 = mincost_last_path + P2;

				const float32 cost_s = cost + static_cast<float32>(std::min(std::min(l1, l2), std::min(l3, l4)) - mincost_last_path);

				cost_aggr_row[d] = cost_s;
				min_cost = std::min(min_cost, cost_s);
			}

			// �����ϸ����ص���С����ֵ�ʹ�������
			mincost_last_path = min_cost;
			memcpy(&cost_last_path[1], cost_aggr_row, disp_range * sizeof(float32));

			// ��һ������
			cost_init_row += direction * disp_range;
			cost_aggr_row += direction * disp_range;
			img_row += direction * 3;
			x += direction;

			// ����ֵ���¸�ֵ
			color_last = color;
		}
	}
}

void adcensus_util::CostAggregateUpDown(const uint8* img_left, const uint8* img_right, const sint32& width, const sint32& height, const sint32& min_disparity, const sint32& max_disparity,
	const float32& p1, const float32& p2, const sint32& tso, const float32* cost_init, float32* cost_aggr, bool is_forward)
{
	assert(width > 0 && height > 0 && max_disparity > min_disparity);

	// �ӲΧ
	const sint32 disp_range = max_disparity - min_disparity;

	// ����(��->��) ��is_forward = true ; direction = 1
	// ����(��->��) ��is_forward = false; direction = -1;
	const sint32 direction = is_forward ? 1 : -1;

	// �ۺ�
	for (sint32 x = 0; x < width; x++) {
		// ·��ͷΪÿһ�е���(β,dir=-1)������
		auto cost_init_col = (is_forward) ? (cost_init + x * disp_range) : (cost_init + (height - 1) * width * disp_range + x * disp_range);
		auto cost_aggr_col = (is_forward) ? (cost_aggr + x * disp_range) : (cost_aggr + (height - 1) * width * disp_range + x * disp_range);
		auto img_col = (is_forward) ? (img_left + 3 * x) : (img_left + (height - 1) * width * 3 + 3 * x);
		sint32 y = (is_forward) ? 0 : height - 1;

		// ·���ϵ�ǰ�Ҷ�ֵ����һ���Ҷ�ֵ
		ADColor color(img_col[0], img_col[1], img_col[2]);
		ADColor color_last = color;

		// ·�����ϸ����صĴ������飬������Ԫ����Ϊ�˱���߽��������β����һ����
		std::vector<float32> cost_last_path(disp_range + 2, Large_Float);

		// ��ʼ������һ�����صľۺϴ���ֵ���ڳ�ʼ����ֵ
		memcpy(cost_aggr_col, cost_init_col, disp_range * sizeof(float32));
		memcpy(&cost_last_path[1], cost_aggr_col, disp_range * sizeof(float32));
		cost_init_col += direction * width * disp_range;
		cost_aggr_col += direction * width * disp_range;
		img_col += direction * width * 3;
		y += direction;

		// ·�����ϸ����ص���С����ֵ
		float32 mincost_last_path = Large_Float;
		for (auto cost : cost_last_path) {
			mincost_last_path = std::min(mincost_last_path, cost);
		}

		// �Է����ϵ�2�����ؿ�ʼ��˳��ۺ�
		for (sint32 i = 0; i < height - 1; i++) {
			color = ADColor(img_col[0], img_col[1], img_col[2]);
			float32 min_cost = Large_Float;
			for (sint32 d = 0; d < disp_range; d++) {
				const sint32 xr = x - d;
				if (xr < 0 || xr >= width) {
					cost_aggr_col[d] = Large_Float;
					continue;
				}
				const ADColor color_r = ADColor(img_right[y*width * 3 + 3 * xr], img_right[y*width * 3 + 3 * xr + 1], img_right[y*width * 3 + 3 * xr + 2]);
				const ADColor color_last_r = ADColor(img_right[(y - direction)*width * 3 + 3 * xr], img_right[(y - direction)*width * 3 + 3 * xr + 1], img_right[(y - direction)*width * 3 + 3 * xr + 2]);
				const uint8 d1 = std::max(abs(color.r - color_last.r), std::max(abs(color.g - color_last.g), abs(color.b - color_last.b)));
				const uint8 d2 = std::max(abs(color_r.r - color_last_r.r), std::max(abs(color_r.g - color_last_r.g), abs(color_r.b - color_last_r.b)));

				float32 P1(0.0f), P2(0.0f);
				if (d1 < tso && d2 < tso) {
					P1 = p1; P2 = p2;
				}
				else if (d1 < tso && d2 > tso) {
					P1 = p1 / 4; P2 = p2 / 4;
				}
				else if (d1 > tso && d2 < tso) {
					P1 = p1 / 4; P2 = p2 / 4;
				}
				else if (d1 > tso && d2 > tso) {
					P1 = p1 / 10; P2 = p2 / 10;
				}
				
				// Lr(p,d) = C(p,d) + min( Lr(p-r,d), Lr(p-r,d-1) + P1, Lr(p-r,d+1) + P1, min(Lr(p-r))+P2 ) - min(Lr(p-r))
				const float32  cost = cost_init_col[d];
				const float32 l1 = cost_last_path[d + 1];
				const float32 l2 = cost_last_path[d] + P1;
				const float32 l3 = cost_last_path[d + 2] + P1;
				const float32 l4 = mincost_last_path + P2;

				const float32 cost_s = cost + static_cast<float32>(std::min(std::min(l1, l2), std::min(l3, l4)) - mincost_last_path);

				cost_aggr_col[d] = cost_s;
				min_cost = std::min(min_cost, cost_s);
			}

			// �����ϸ����ص���С����ֵ�ʹ�������
			mincost_last_path = min_cost;
			memcpy(&cost_last_path[1], cost_aggr_col, disp_range * sizeof(float32));

			// ��һ������
			cost_init_col += direction * width * disp_range;
			cost_aggr_col += direction * width * disp_range;
			img_col += direction * width * 3;
			y += direction;

			// ����ֵ���¸�ֵ
			color_last = color;
		}
	}
}