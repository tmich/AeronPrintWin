#include "stdafx.h"
#include "Order.h"



Order::Order(const Order & rhs)
{
	mId = rhs.mId;
	this->mCreationDate = rhs.mCreationDate;
	this->mCustomerCode = rhs.mCustomerCode;
	this->mCustomerName = rhs.mCustomerName;
	this->mNotes = rhs.mNotes;
	this->mRead = rhs.mRead;
	this->mRemoteId = rhs.mRemoteId;
	this->Items.assign(rhs.Items.begin(), rhs.Items.end());
}

Order::Order()
{
	mId = 0;
	mRemoteId = 0;
	mRead = false;
}


Order::~Order()
{
}

std::wstring Order::GetCreationDate() const
{
	std::wstring wd;
	std::wstring delimiter1 = _T(" ");
	std::wstring date = mCreationDate.substr(0, mCreationDate.find(delimiter1));
	std::wstring time = mCreationDate.substr(mCreationDate.rfind(delimiter1));
	std::vector<std::wstring> dateTokens, timeTokens;
	
	size_t pos = 0;
	std::wstring anno, mese, giorno;
	std::wstring delimiter2 = _T("-");

	while ((pos = date.find(delimiter2)) != std::wstring::npos)
	{
		auto token = date.substr(0, pos);
		dateTokens.push_back(token);
		date.erase(0, pos + delimiter2.length());
	}
	dateTokens.push_back(date);
	assert(dateTokens.size() == 3);
	anno = dateTokens[0];
	mese = dateTokens[1];
	giorno = dateTokens[2];

	wd.append(giorno).append(delimiter2).append(mese).append(delimiter2).append(anno);

	std::wstring ora, minuto, secondo;
	std::wstring delimiter3 = _T(":");
	while ((pos=time.find(delimiter3)) != std::wstring::npos)
	{
		auto token = time.substr(0, pos);
		timeTokens.push_back(token);
		time.erase(0, pos + delimiter3.length());
	}
	timeTokens.push_back(time);
	assert(timeTokens.size() == 3);
	ora = timeTokens[0];
	minuto = timeTokens[1];
	secondo = timeTokens[2];

	wd.append(_T(" ")).append(ora).append(delimiter3).append(minuto); //.append(delimiter3).append(secondo);

	return wd;
}

OrderItem::OrderItem()
{
	mId = 0;
}
