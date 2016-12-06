#include "stdafx.h"
#include "MainFrame.h"

CMainFrame::CMainFrame()
{
	SetView(m_view);
	LoadRegistrySettings(_T("AeronPrintWin"));
}


CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(CREATESTRUCT & cs)
{
	SetUseReBar(false);
	SetUseToolBar(false);
	ShowStatusBar(true);
	
	CFrame::OnCreate(cs);
	
	if (::IsWindow(GetStatusBar()))
	{
		GetStatusBar().SetSimple();
	}

	return 1;
}

void CMainFrame::SetupToolBar()
{
	//AddToolBarButton(IDM_FILE_NEW);
}

void CMainFrame::OnInitialUpdate()
{
	CFrame::OnInitialUpdate();
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//MessageBox(_T("CMainFrame::OnCommand"), _T("info"), MB_ICONINFORMATION);
	switch (LOWORD(wParam))
	{
		case ID_MODIFICA_IMPOSTAZIONI:	OnSettings();		return true;
		case ID_FILE_ESCI:				OnDestroy();		return true;
		case ID_FILE_STAMPA:			OnPrint();			return true;
		case IDC_CONTROLLA:				OnCheckOrders();	return true;
		case IDC_CERCA:					OnSearch();			return true;
		case IDC_CANCCERCA:				OnCancelSearch();	return true;
		case IDC_CMBPAG:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				OnChangePage();
			}
			return true;
		}
		default:
			break;
	}
	return false;
}


//LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
//{
//	MessageBox(_T("CMainFrame::OnNotify"), _T("info"), MB_ICONINFORMATION);
//	switch (LOWORD(wParam))
//	{
//	case IDC_CONTROLLA:	OnControlla();	break;
//	default:			break;
//	}
//	return LRESULT();
//}

void CMainFrame::OnCheckOrders()
{
	//MessageBox(_T("Bravo bravo, controlla!"), _T("CMainFrame::OnNotify"), MB_ICONINFORMATION);
	/*Order o;
	o.SetRemoteId(18);
	o.SetCustomerName(_T("O' Ristorante 'ngopp'o mare di Peppino Scapece detto O'Cinese"));
	m_view.AddOrder(o);*/

	m_view.CanCheckOrders(false);
	//m_view.Update();
	auto x = m_client.GetAll(0).then([&](std::vector<Order> orders) {
		m_orders = orders;
		/*for (auto &o : m_orders)
		{
			m_view.AddOrder(o);
		}*/
		OrderPager pager{ m_orders, 20 };
		m_view.SetPages(pager.Pages());

		/*for (auto &o : pager.GetPage(1))
		{
			m_view.AddOrder(o);
		}
		m_view.UpdateOrders();*/

		OnChangePage();
		SetStatusText(_T("Terminato"));
		m_view.CanCheckOrders(true);
		//m_view.Update();
	});

	SetStatusText(_T("Controllo in corso..."));
}

void CMainFrame::OnSearch()
{
	std::wstring search = m_view.GetSearchText();

	if (m_orders.size() == 0 || search.compare(L"") == 0)
		return;

	this->tolower(search);
	std::vector<Order> matches;

	m_view.CanCheckOrders(false);
	m_view.Update();

	std::copy_if(m_orders.begin(), m_orders.end(), 
		std::back_inserter(matches), [=](const Order& o) {
			wstring cname = o.GetCustomerName();
			this->tolower(cname);
			std::size_t found = cname.find(search);
		
			return found != std::wstring::npos;
		}
	);

	m_view.SetOrders(matches);
	m_view.UpdateOrders();

	m_view.CanCheckOrders(true);
	m_view.Update();
}

void CMainFrame::OnCancelSearch()
{
	m_view.SetOrders(m_orders);
	m_view.UpdateOrders();
	return;
}

void CMainFrame::OnSettings()
{
	dlgSettings.SetApiAddress(m_apiAddress);
	dlgSettings.SetPollingTime(m_pollingTime);

	if (dlgSettings.DoModal(m_hWnd))
	{
		m_apiAddress = dlgSettings.GetApiAddress();
		m_pollingTime = dlgSettings.GetPollingTime();
		SaveRegistrySettings();
	}
}

void CMainFrame::OnPrint()
{
	Order o;
	if (m_view.GetSelectedOrder(o))
	{
		CPrintDialog printDialog;
		if (printDialog.DoModal(*this) == IDOK)
		{
			auto dc = printDialog.GetPrinterDC();

			// Zero and then initialize the members of a DOCINFO structure.
			DOCINFO di = { 0 };
			di.cbSize = sizeof(DOCINFO);
			di.lpszDocName = _T("AeronPrint Document");
			di.lpszOutput = (LPTSTR)NULL;
			di.lpszDatatype = (LPTSTR)NULL;
			di.fwType = 0;

			// Begin a print job by calling the StartDoc function.
			StartDoc(dc, &di);

			// Inform the driver that the application is about to begin sending data.
			StartPage(dc);
			CRect rc{ 10, 10, 800, 400 };
			dc.FillRect(rc, (HBRUSH)::GetStockObject(BLACK_BRUSH));
			dc.DrawTextW(o.GetCustomerName().c_str(), o.GetCustomerName().size(), rc, 0);

			// Inform the driver that the page is finished.
			EndPage(dc);

			// Inform the driver that document has ended.
			EndDoc(dc);

		}
	}
}

void CMainFrame::OnChangePage()
{
	int pagenumber = 1;
	try
	{
		pagenumber = m_view.GetSelectedPage();
	}
	catch(std::exception&) {}
	OrderPager pager{ m_orders, 20 };
	
	m_view.ClearOrders();
	for (auto &o : pager.GetPage(pagenumber))
	{
		m_view.AddOrder(o);
	}
	m_view.UpdateOrders();
}

BOOL CMainFrame::LoadRegistrySettings(LPCTSTR szKeyName)
{
	BOOL bRet = FALSE;

	//CFrame::LoadRegistrySettings(szKeyName);

	CString KeyName = _T("Software\\") + CString(_T("AeronPrintWin"));
	CString SubKeyName = KeyName + _T("\\App Settings");

	CRegKey Key;
	if (ERROR_SUCCESS == Key.Open(HKEY_CURRENT_USER, SubKeyName, KEY_READ))
	{
		try
		{
			ULONG sz = 200;
			TCHAR szText[201] = { 0 };
			
			// Load a text string
			if (ERROR_SUCCESS != Key.QueryStringValue(_T("ApiAddress"), szText, &sz))
				throw CWinException(_T("QueryStringValue (ApiAddress) Failed"));
			m_apiAddress.Assign(szText, sz);

			if(ERROR_SUCCESS != Key.QueryDWORDValue(_T("PollingTime"), m_pollingTime))
				throw CWinException(_T("QueryDWORDValue (PollingTime) Failed"));

			m_client.SetApiPath(m_apiAddress.c_str());

			bRet = true;

		}
		catch (const CWinException& e)
		{
			TRACE("*** Failed to load values from registry. ***\n");
			TRACE(e.GetText()); TRACE("\n");
		}
	}
	else
	{
		TRACE("*** Trying to save default settings... ***\n");

		m_apiAddress = _T("http://93.150.25.179/api/v1.0/");
		m_pollingTime = 60;

		SaveRegistrySettings();
	}

	return bRet;
}

BOOL CMainFrame::SaveRegistrySettings()
{
	BOOL bRet = FALSE;
	CString KeyName = _T("Software\\") + CString(_T("AeronPrintWin"));

	try
	{
		CString SubKeyName = KeyName + _T("\\App Settings");
		CRegKey Key;

		// Create and open the registry subkey
		if (ERROR_SUCCESS != Key.Create(HKEY_CURRENT_USER, SubKeyName))
			throw CWinException(_T("Create Key failed"));
		if (ERROR_SUCCESS != Key.Open(HKEY_CURRENT_USER, SubKeyName))
			throw CWinException(_T("Open Key failed"));

		// store a text string in the registry
		if (ERROR_SUCCESS != Key.SetStringValue(_T("ApiAddress"), m_apiAddress))
			throw CWinException(_T("SetStringValue (ApiAddress) failed"));

		// store a DWORD value in the registry
		if (ERROR_SUCCESS != Key.SetDWORDValue(_T("PollingTime"), m_pollingTime))
			throw CWinException(_T("SetDWORDValue (PollingTime) failed"));

		bRet = TRUE;
	}
	catch (const CWinException& e)
	{
		TRACE("*** Failed to load values from registry. ***\n");
		TRACE(e.GetText()); TRACE("\n");
	}

	return bRet;
}

std::wstring CMainFrame::tolower(std::wstring& in) const
{
	std::locale::global(std::locale(""));  // (*)
	std::wcout.imbue(std::locale());
	auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());
	f.tolower(&in[0], &in[0] + in.size());
	return in;
}
