#include "stdafx.h"
#include "Pager.h"

OrderPager::OrderPager(std::vector<Order> data, int pagesize)
	:CPager(pagesize)
{
	m_elements = data;
}
