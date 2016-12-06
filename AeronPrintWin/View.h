#pragma once
#include "Order.h"

class CSearchEdit : public CEdit
{
public:
	CSearchEdit() {}
	virtual ~CSearchEdit() {}
protected:
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class View :
	public CDialog
{
public:
	View();
	virtual ~View();
	
	void AddOrder(const Order& order);
	void SetOrders(const std::vector<Order>& orders);
	void Update();
	void UpdateOrders();
	void ClearOrders();
	void CanCheckOrders(bool flag);
	bool GetSelectedOrder(Order& order) const;
	std::wstring GetSearchText() const;
	void SetPages(int pages);
	int GetSelectedPage() { return std::stoi(m_cmbPagine.GetWindowText().c_str()); }
protected:
	virtual HWND Create(HWND hParent) override;
	virtual BOOL OnInitDialog() override;
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	virtual void OnCancel() override {};
	virtual void OnOK() override {};
private:
	void InsertItem(const Order& order);
	std::vector<Order> m_orders;
	CResizer m_resizer;
	CListView m_list;
	CButton m_btnControlla, m_btnCerca, m_btnCancCerca;
	CStatic m_lblCerca, m_lblPagine;
	CSearchEdit m_txtCerca;
	CComboBox m_cmbPagine;
	bool m_enableControlla;
	int m_pages;
};