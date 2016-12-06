#pragma once
#include "stdafx.h"
#include "Order.h"
#include <fenv.h>

template<typename T> 
class CPager
{
public:
	CPager(int pagesize) 
		: m_pagesize{ pagesize } {};
	virtual ~CPager() {};

	long Pages()
	{
		fesetround(FE_UPWARD);
		return lrint(ceil(m_elements.size() / m_pagesize));
	}

	std::vector<T> GetPage(int n)
	{
		std::vector<T> page;
		size_t min = (m_pagesize*(n - 1));
		size_t max = min + m_pagesize;
		if (max > m_elements.size()) 
			max = m_elements.size();
		while (min < max)
		{
			page.push_back(m_elements[min++]);
		}

		return page;
	}
protected:
	std::vector<T> m_elements;
	int m_pagesize;
};

class OrderPager : public CPager<Order>
{
public:
	OrderPager(std::vector<Order> data, int pagesize);
	virtual ~OrderPager() {};
};