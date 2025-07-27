#include "stdafx.h"
#include "NeteaseLyricDownload.h"

std::wstring CNeteaseLyricDownload::GetSearchUrl(const std::wstring& key_words, int result_count)
{
	CString url;
	url.Format(L"http://music.163.com/api/search/get/?s=%s&limit=%d&type=1&offset=0", key_words.c_str(), result_count);
    return url.GetString();
}

void CNeteaseLyricDownload::DisposeSearchResult(vector<ItemInfo>& down_list, const wstring& search_result, int result_count)
{
	down_list.clear();
	ItemInfo item;
	int index1{}, index2{}, index3{}, index4{};
	for (int i{}; i < result_count; i++)
	{
		//��ȡ������ID
		if (i == 0)
		{
			index1 = search_result.find(L"\"songs\":[{\"id\":", index1 + 1);
			if (index1 == string::npos) break;
			index2 = search_result.find(L',', index1);
			item.id = search_result.substr(index1 + 15, index2 - index1 - 15);
		}
		else
		{
			index1 = search_result.find(L",{\"id\":", index1 + 1);
			if (index1 == string::npos) break;
			index2 = search_result.find(L',', index1 + 1);
			item.id = search_result.substr(index1 + 7, index2 - index1 - 7);
		}

		//��ȡ��������
		index2 = search_result.find(L"name", index1);
		if (index2 == string::npos) continue;
		index3 = search_result.find(L"\",\"", index2);
		wstring title = search_result.substr(index2 + 7, index3 - index2 - 7);
		if (search_result.substr(index3 + 3, 6) == L"picUrl")	//����ҵ��ġ�name��������ַ����ǡ�picUrl����˵������name��ֵ����
		{														//��һ�׸�ı��⣬������һ�׸�������ң���һ�׸��ж��������
			if (!down_list.empty())
			{
				down_list.back().artist += L'/';
				down_list.back().artist += title;
			}
			continue;
		}
		else
		{
			item.title = title;
		}

		//��ȡ������������
		index2 = search_result.find(L"artists", index1);
		if (index2 == string::npos) continue;
		index3 = search_result.find(L"name", index2);
		index4 = search_result.find(L"\",\"", index3);
		item.artist = search_result.substr(index3 + 7, index4 - index3 - 7);

		//��ȡ�����ĳ�Ƭ��
		index2 = search_result.find(L"\"album\"", index1);
		if (index2 == string::npos) continue;
		index3 = search_result.find(L"name", index2);
		index4 = search_result.find(L"\",\"", index3);
		item.album = search_result.substr(index3 + 7, index4 - index3 - 7);

		//��ȡʱ��
		index2 = search_result.find(L"\"duration\"", index1);
		if (index2 != string::npos)
		{
			index3 = search_result.find(L',', index2);
			wstring str_duration = search_result.substr(index2 + 11, index3 - index2 - 11);
			item.duration = _wtoi(str_duration.c_str());
		}

		CInternetCommon::DeleteStrSlash(item.title);
		CInternetCommon::DeleteStrSlash(item.artist);
		CInternetCommon::DeleteStrSlash(item.album);
		down_list.push_back(item);
	}
}

std::wstring CNeteaseLyricDownload::GetAlbumCoverURL(const wstring& song_id)
{
	if (song_id.empty())
		return wstring();
	//��ȡר������ӿڵ�URL
	wchar_t buff[256];
	swprintf_s(buff, L"http://music.163.com/api/song/detail/?id=%s&ids=%%5B%s%%5D&csrf_token=", song_id.c_str(), song_id.c_str());
	wstring contents;
	//��URL���ݱ��浽�ڴ�
	if (!CInternetCommon::GetURL(wstring(buff), contents))
		return wstring();
#ifdef _DEBUG
	ofstream out_put{ L".\\cover_down.log", std::ios::binary };
	out_put << CCommon::UnicodeToStr(contents, CodeType::UTF8);
	out_put.close();
#endif // _DEBUG

	size_t index;
	index = contents.find(L"\"album\"");
	if (index == wstring::npos)
		return wstring();
	index = contents.find(L"\"picUrl\"", index + 7);
	if (index == wstring::npos)
		return wstring();
	wstring url;
	size_t index1;
	index1 = contents.find(L'\"', index + 10);
	url = contents.substr(index + 10, index1 - index - 10);

	return url;
}

std::wstring CNeteaseLyricDownload::GetOnlineUrl(const wstring& song_id)
{
	std::wstring song_url{ L"http://music.163.com/#/song?id=" + song_id };
	return song_url;
}

int CNeteaseLyricDownload::RequestSearch(const std::wstring& url, std::wstring& result)
{
	return CInternetCommon::HttpPost(url, result);
}

bool CNeteaseLyricDownload::DownloadLyric(const wstring& song_id, wstring& result, bool download_translate)
{
	std::wstring lyric_url;
	if (!download_translate)
		lyric_url = L"http://music.163.com/api/song/media?id=" + song_id;
	else
		lyric_url = L"http://music.163.com/api/song/lyric?os=osx&id=" + song_id + L"&lv=-1&kv=-1&tv=-1";
	return CInternetCommon::GetURL(lyric_url, result);
}

bool CNeteaseLyricDownload::DisposeLryic(wstring& lyric_str, bool download_translate)
{
	size_t index1 = lyric_str.find('[');	//���ҵ�1���������ţ���Ϊ��ʿ�ʼ��λ��
	if (index1 == string::npos)
	{
		return false;
	}
	lyric_str = lyric_str.substr(index1, lyric_str.size() - index1 - 13);
	if (!lyric_str.empty() && lyric_str.back() == L'\"')
		lyric_str.pop_back();

	for (size_t i{}; i < lyric_str.size() - 1; i++)
	{
		//�������к����ַ�����\r\n����\n\n��������ת����Ϊ����ת���ַ�\r\n��ɾ������������ַ�
		if (i < lyric_str.size() - 3)
		{
			if ((lyric_str[i] == '\\' && lyric_str[i + 1] == 'r' && lyric_str[i + 2] == '\\' && lyric_str[i + 3] == 'n')
				|| (lyric_str[i] == '\\' && lyric_str[i + 1] == 'n' && lyric_str[i + 2] == '\\' && lyric_str[i + 3] == 'n'))
			{
				lyric_str[i] = '\r';
				lyric_str[i + 1] = '\n';
				lyric_str.erase(i + 2, 2);
			}
		}
		//�������к����ַ�����\r��������ת����Ϊ����ת���ַ�\r\n
		if (lyric_str[i] == '\\' && lyric_str[i + 1] == 'r')
		{
			lyric_str[i] = '\r';
			lyric_str[i + 1] = '\n';
		}
		//�������к����ַ�����\n��������ת����Ϊ����ת���ַ�\r\n
		if (lyric_str[i] == '\\' && lyric_str[i + 1] == 'n')	//������ı��еġ�\n��ת���ɻس���\r\n
		{
			lyric_str[i] = '\r';
			lyric_str[i + 1] = '\n';
		}
		//�������к����ַ�����\"������ɾ����б�ܡ�\��
		if (lyric_str[i] == '\\' && lyric_str[i + 1] == '\"')
		{
			lyric_str.erase(i, 1);
		}
	}

	return true;
}
