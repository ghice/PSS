#include "MainConfig.h"


CMainConfig::CMainConfig()
{
	m_u4BuffSize = MAX_BUFF_1024;
}

CMainConfig::~CMainConfig()
{
}

bool CMainConfig::Init(const char* szConfigPath)
{
	char* pData = NULL;
	OUR_DEBUG((LM_INFO, "[CMainConfig::Init_Main]Filename = %s.\n", szConfigPath));
	if(false == m_MainConfig.Init(szConfigPath))
	{
		OUR_DEBUG((LM_INFO, "[CMainConfig::Init_Main]File Read Error = %s.\n", szConfigPath));
		return false;
	}

	//获得当前网络模型
	pData = m_MainConfig.GetData("RecvInfo", "BuffSize");
	if(NULL != pData)
	{
		m_u4BuffSize = (uint32)ACE_OS::atoi(pData);
	}

	//获得监听端口信息
	_ServerInfo serverinfo;

	m_vecServerInfo.clear();
	TiXmlElement* pNextTiXmlElementIP     = NULL;
	TiXmlElement* pNextTiXmlElementPort   = NULL;
	TiXmlElement* pNextTiXmlElementIpType = NULL;
	TiXmlElement* pNextTiXmlElementkey    = NULL;
	while(true)
	{
		pData = m_MainConfig.GetData("TCPServerIP", "ip", pNextTiXmlElementIP);
		if(pData != NULL)
		{
			sprintf_safe(serverinfo.m_szServerIP, MAX_BUFF_50, "%s", pData);
		}
		else
		{
			break;
		}

		pData = m_MainConfig.GetData("TCPServerIP", "port", pNextTiXmlElementPort);
		if(pData != NULL)
		{
			serverinfo.m_nPort = ACE_OS::atoi(pData);
		}
		else
		{
			break;
		}

		pData = m_MainConfig.GetData("TCPServerIP", "ipType", pNextTiXmlElementIpType);
		if(pData != NULL)
		{
			if(ACE_OS::strcmp(pData, "IPV6") == 0)
			{
				serverinfo.m_u1IPType = TYPE_IPV6;
			}
			else
			{
				serverinfo.m_u1IPType = TYPE_IPV4;
			}
		}
		else
		{
			break;
		}

		pData = m_MainConfig.GetData("TCPServerIP", "key", pNextTiXmlElementkey);
		if(pData != NULL)
		{
			sprintf_safe(serverinfo.m_szKey, MAX_BUFF_50, "%s", pData);
		}
		else
		{
			break;
		}

		m_vecServerInfo.push_back(serverinfo);
	}

	return true;
}

uint32 CMainConfig::GetBuffSize()
{
	return m_u4BuffSize;
}

uint32 CMainConfig::GerServerInfoCount()
{
	return (uint32)m_vecServerInfo.size();
}

_ServerInfo* CMainConfig::GetServerInfo(uint32 u4Index)
{
	if(u4Index < (uint32)m_vecServerInfo.size())
	{
		return &m_vecServerInfo[u4Index];
	}
	else
	{
		return NULL;
	}
}

void CMainConfig::Display()
{
	OUR_DEBUG((LM_INFO, "[CMainConfig::Display]m_u4BuffSize=%d.\n", m_u4BuffSize));
	for(uint32 i = 0; i < (uint32)m_vecServerInfo.size(); i++)
	{
		OUR_DEBUG((LM_INFO, "[CMainConfig::Display](%d)m_szServerIP=%s.\n", i, m_vecServerInfo[i].m_szServerIP));
		OUR_DEBUG((LM_INFO, "[CMainConfig::Display](%d)m_nPort=%d.\n", i, m_vecServerInfo[i].m_nPort));
		OUR_DEBUG((LM_INFO, "[CMainConfig::Display](%d)m_nPort=%s.\n", i, m_vecServerInfo[i].m_szKey));
	}
}

