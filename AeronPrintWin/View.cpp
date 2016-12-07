#include "stdafx.h"
#include "resource.h"
#include "View.h"



View::View()
	: CDialog(IDD_MAINVIEW)
{
}


View::~View()
{
}

void View::AddOrder(const Order & order)
{
	m_orders.push_back(order);
}

void View::SetOrders(const std::vector<Order>& orders)
{
	m_orders = orders;
}

void View::Update()
{
	m_list.DeleteAllItems();

	for (auto& o : m_orders)
	{
		InsertItem(o);
	}
}

void View::ClearOrders()
{
	m_orders.erase(m_orders.begin(), m_orders.end());
}

void View::CanCheckOrders(bool flag)
{
	m_enableControlla = flag;

	::EnableWindow(m_btnControlla, m_enableControlla);
	::EnableWindow(m_list, m_enableControlla);
	::EnableWindow(m_cmbPagine, m_enableControlla);
}

bool View::GetSelectedOrder(Order& order) const
{
	if (m_list.GetSelectedCount() > 0)
	{
		int sel = m_list.GetSelectionMark();
		auto optr = (Order*)m_list.GetItemData(sel);
		order = *optr;

		return true;
	}

	return false;
}

std::wstring View::GetSearchText() const
{
	CString search = m_txtCerca.GetWindowTextW();
	search.Trim();
	return search.c_str();
}

void View::SetPages(int pages)
{
	m_pages = pages;
	auto sel = m_cmbPagine.GetCurSel();
	
	m_cmbPagine.ResetContent();

	for (int p = 1; p <= m_pages; p++)
	{
		m_cmbPagine.AddString(std::to_wstring(p).c_str());
	}

	m_cmbPagine.SetCurSel(sel >= 0 ? sel : 0);
}

int View::GetSelectedPage()
{
	try
	{
		return std::stoi(m_cmbPagine.GetWindowText().c_str());
	}
	catch (const std::exception&)
	{
		return 1;
	}
}

//void View::SetSelectedPage(int page)
//{
//	LPCTSTR pp = std::to_wstring(page).c_str();
//	int p = m_cmbPagine.FindStringExact(0, pp);
//	if (p >= 0)
//	{
//		m_cmbPagine.SetCurSel(p);
//	}
//}

HWND View::Create(HWND hParent)
{
	return DoModeless(hParent);
}

BOOL View::OnInitDialog()
{
	AttachItem(IDC_LIST1, m_list);
	AttachItem(IDC_CONTROLLA, m_btnControlla);
	AttachItem(IDC_CERCA, m_btnCerca);
	AttachItem(IDC_TXTCERCA, m_txtCerca);
	AttachItem(IDC_CANCCERCA, m_btnCancCerca);
	AttachItem(IDC_STCERCA, m_lblCerca);
	AttachItem(IDC_STPAGINE, m_lblPagine);
	AttachItem(IDC_CMBPAG, m_cmbPagine);

	/*HICON icon = GetApp().LoadIcon(IDI_REFRESH);
	m_btnControlla.SetIcon(icon);*/
	//::EnableWindow(m_cmbPagine, false);

	m_resizer.Initialize(*this, CRect(0, 0, 300, 200));
	m_resizer.AddChild(m_btnCancCerca, topright, 0);
	m_resizer.AddChild(m_btnCerca, topright, 0);
	m_resizer.AddChild(m_txtCerca, topright, 0);
	m_resizer.AddChild(m_lblCerca, topright, 0);
	m_resizer.AddChild(m_list, Alignment::topleft, RD_STRETCH_WIDTH | RD_STRETCH_HEIGHT);
	m_resizer.AddChild(m_btnControlla, Alignment::bottomleft, 0);
	m_resizer.AddChild(m_cmbPagine, Alignment::bottomright, 0);
	m_resizer.AddChild(m_lblPagine, bottomright, 0);

	// colonne lista
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, _T("N"), 0, 50);
	m_list.InsertColumn(1, _T("Cliente"), 0, 400);
	m_list.InsertColumn(2, _T("Data"), 0, 150);
	m_list.InsertColumn(3, _T("Prodotti"), 0, 100);

	return true;
}

INT_PTR View::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_resizer.HandleMessage(uMsg, wParam, lParam);
	
	return DialogProcDefault(uMsg, wParam, lParam);
}

BOOL View::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if ((HIWORD(wParam) == EN_CHANGE) && (LOWORD(wParam) == IDC_TXTCERCA))
	{
		if(m_txtCerca.GetWindowTextLengthW() == 0)
			return GetParent().SendNotifyMessage(WM_COMMAND, IDC_CANCCERCA, lParam);
	}

	if ((HIWORD(wParam) == CBN_SELCHANGE) && (LOWORD(wParam) == IDC_CMBPAG))
		return GetParent().SendNotifyMessage(WM_COMMAND, IDC_CMBPAG, lParam);

	switch (LOWORD(wParam))
	{
	case IDC_LIST1:
	case IDC_CMBPAG:
	case IDC_TXTCERCA:
		break;
	default:	// lo rimbalzo al frame (il controller)
		return GetParent().SendNotifyMessage(WM_COMMAND, wParam, lParam);
	}

	return false;
}

LRESULT View::OnNotify(WPARAM wParam, LPARAM lParam)
{
	if (((LPNMHDR)lParam)->code == NM_DBLCLK && LOWORD(wParam) == IDC_LIST1)
	{
		//MessageBox(_T("doppio click!"), _T("bravo"), MB_ICONINFORMATION);
		return GetParent().SendNotifyMessage(WM_COMMAND, wParam, lParam);
	}

	if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == IDC_CMBPAG)
	{
		// cambio pagina
		return GetParent().SendNotifyMessage(WM_COMMAND, wParam, lParam);
	}

	return CDialog::OnNotify(wParam, lParam);
}

void View::InsertItem(const Order & order)
{
	
	int index = m_list.GetItemCount();
	LVITEM it;
	it.iItem = index;
	it.mask = LVIF_TEXT | LVIF_IMAGE;
	
	it.iSubItem = 0;
	std::wstring id = std::to_wstring(order.GetRemoteId());
	it.pszText = (LPWSTR)id.c_str();
	m_list.InsertItem(it);

	it.iSubItem = 1;
	std::wstring customername = order.GetCustomerName();
	it.pszText = (LPWSTR)customername.c_str();
	m_list.SetItem(it);

	it.iSubItem = 2;
	std::wstring date = order.GetCreationDate();
	it.pszText = (LPWSTR)date.c_str();
	m_list.SetItem(it);

	it.iSubItem = 3;
	std::wstring nprodotti = std::to_wstring(order.Items.size());
	it.pszText = (LPWSTR)nprodotti.c_str();
	m_list.SetItem(it);

	m_list.SetItemData(index, (DWORD_PTR)&order);
}

LRESULT CSearchEdit::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_UNICHAR:
		return OnKeyDown(uMsg, wParam, lParam);
	default:
		break;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

LRESULT CSearchEdit::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_RETURN:
	{
		return GetParent().SendNotifyMessageW(WM_COMMAND, IDC_CERCA, lParam);
	}
	case VK_ESCAPE:
	{
		SetWindowText(_T(""));
		return GetParent().SendNotifyMessageW(WM_COMMAND, IDC_CANCCERCA, lParam);
	}
	default:
		break;
	}

	return FinalWindowProc(uMsg, wParam, lParam);
}