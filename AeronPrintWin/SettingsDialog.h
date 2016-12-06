#pragma once

class SettingsDialog :
	public CDialog
{
public:
	SettingsDialog();
	virtual ~SettingsDialog();
	
	virtual bool Show();

	virtual CString GetApiAddress() const;
	virtual unsigned long GetPollingTime() const;
	virtual void SetApiAddress(CString apiAddress);
	virtual void SetPollingTime(unsigned long pollingTime);
protected:
	virtual void OnOK() override;
	virtual BOOL OnInitDialog() override;
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual int OnCreate(CREATESTRUCT& cs) override;
	bool m_choosen = false;
private:
	CString m_api;
	unsigned long m_time;
	CEdit m_txtApi, m_txtTime;
};

