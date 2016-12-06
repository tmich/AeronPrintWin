#include "stdafx.h"
#include "SettingsDialog.h"



SettingsDialog::SettingsDialog()
	:CDialog(IDD_SETTINGS)
{
	m_time = 15000;
}


SettingsDialog::~SettingsDialog()
{
}

bool SettingsDialog::Show()
{
	DoModal();

	return m_choosen;
}


CString SettingsDialog::GetApiAddress() const
{
	return m_api;
}

unsigned long SettingsDialog::GetPollingTime() const
{
	return m_time;
}

void SettingsDialog::SetApiAddress(CString apiAddress)
{
	m_api = apiAddress;
}

void SettingsDialog::SetPollingTime(unsigned long pollingTime)
{
	m_time = pollingTime;
}

void SettingsDialog::OnOK()
{
	m_choosen = true;
	m_api = m_txtApi.GetWindowTextW();
	m_time = _tstoi(m_txtTime.GetWindowTextW());

	EndDialog(true);
}

BOOL SettingsDialog::OnInitDialog()
{
	AttachItem(IDC_API, m_txtApi);
	AttachItem(IDC_TIME, m_txtTime);

	m_txtApi.SetWindowTextW(m_api);

	CString pt;
	pt.AppendFormat(_T("%u"), m_time);
	m_txtTime.SetWindowTextW(pt);

	return true;
}

INT_PTR SettingsDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DialogProcDefault(uMsg, wParam, lParam);
}

int SettingsDialog::OnCreate(CREATESTRUCT & cs)
{
	return CDialog::OnCreate(cs);
}
