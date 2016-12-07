#pragma once
#include "View.h"
#include "SettingsDialog.h"
#include "aeron_client.h"
#include "Pager.h"

class CMainFrame :
	public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();
protected:
	virtual int OnCreate(CREATESTRUCT& cs) override;
	virtual void SetupToolBar() override;
	virtual void OnInitialUpdate() override;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	//virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam) override;

	virtual void OnCheckOrders();
	virtual void OnSearch();
	virtual void OnCancelSearch();
	virtual void OnSettings();
	virtual void OnPrint();
	virtual void OnChangePage();
	virtual void OnSelectedOrder();
	virtual BOOL LoadRegistrySettings(LPCTSTR szKeyName) override;
	virtual BOOL SaveRegistrySettings() override;
private:
	std::wstring tolower(std::wstring& in) const;

	View m_view;
	CString m_apiAddress;
	unsigned long m_pollingTime;
	SettingsDialog dlgSettings;
	AeronClient m_client;
	std::vector<Order> m_orders;
	OrderPager *pager = nullptr;
};

