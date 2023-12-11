#include "stdafx.h"
#include "GetTagOnlineDlg.h"
#include "Resource.h"
#include "Player.h"
#include "MusicPlayer2.h"

CGetTagOnlineDlg::CGetTagOnlineDlg(const SongInfo& song, CWnd * pParent)
    : CCoverDownloadDlg(pParent), m_song_info(song)
{
}

CGetTagOnlineDlg::~CGetTagOnlineDlg()
{
}

CString CGetTagOnlineDlg::GetDialogName() const
{
    return _T("GetTagOnlineDlg");
}

void CGetTagOnlineDlg::OnBnClickedDownloadSelected()
{
    //MessageBox(_T("Downloan button clicked!"));
    if (m_item_selected < 0 || m_item_selected >= static_cast<int>(m_down_list.size()))
        return;
    m_item_info = m_down_list[m_item_selected];
    CPlayer::GetInstance().SetRelatedSongID(m_item_info.id);		//��ѡ����Ŀ�ĸ���ID����������

    if (m_pParentWnd != nullptr)
    {
        m_pParentWnd->SendMessage(WM_PORPERTY_ONLINE_INFO_ACQUIRED, (WPARAM)&m_item_info);
        OnCancel();
    }
}

SongInfo CGetTagOnlineDlg::GetSongInfo() const
{
    return m_song_info;
}


BOOL CGetTagOnlineDlg::OnInitDialog()
{
    CCoverDownloadDlg::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    SetIcon(theApp.m_icon_set.info.GetIcon(true), FALSE);

    SetWindowText(theApp.m_str_table.LoadText(L"TITLE_GET_TAG_ONLINE").c_str());

    CWnd* pGetInfoBtn = GetDlgItem(IDC_DOWNLOAD_SELECTED);
    if (pGetInfoBtn != nullptr)
        pGetInfoBtn->SetWindowText(theApp.m_str_table.LoadText(L"TXT_GET_TAG_ONLINE_GET_THIS").c_str());

    ShowDlgCtrl(IDC_DOWNLOAD_OPTION_GROUPBOX, false);
    ShowDlgCtrl(IDC_COVER_LOCATION_STATIC, false);
    ShowDlgCtrl(IDC_SAVE_TO_SONG_FOLDER2, false);
    ShowDlgCtrl(IDC_SAVE_TO_ALBUM_FOLDER2, false);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // �쳣: OCX ����ҳӦ���� FALSE
}
