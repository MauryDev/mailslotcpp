#include "MailSlot.h"



std::mailslotview::mailslotview(HANDLE _handle)
{
    handle = NULL;
}

std::mailslotserver::mailslotserver() : mailslot() {}

std::mailslotserver::mailslotserver(std::wstring&& name, uint32_t nMaxMessageSize, uint32_t lReadTimeout)
{
    auto slotname = L"\\\\.\\mailslot\\" + name;
    handle = CreateMailslotW(slotname.c_str(),
        nMaxMessageSize,                             // no maximum message size 
        lReadTimeout,         // no time-out for operations 
        (LPSECURITY_ATTRIBUTES)NULL); // default security
}

std::mailslotserver::mailslotserver(std::string&& name, uint32_t nMaxMessageSize, uint32_t lReadTimeout)
{
    auto slotname = "\\\\.\\mailslot\\" + name;
    handle = CreateMailslotA(slotname.c_str(),
        nMaxMessageSize,                             // no maximum message size 
        lReadTimeout,         // no time-out for operations 
        (LPSECURITY_ATTRIBUTES)NULL); // default security
}


std::mailslotserver::mailslotserver(std::wstring& name, uint32_t nMaxMessageSize, uint32_t lReadTimeout) : mailslotserver(std::move(name), nMaxMessageSize, lReadTimeout) {}

std::mailslotserver::mailslotserver(std::string& name, uint32_t nMaxMessageSize, uint32_t lReadTimeout) : mailslotserver(std::move(name), nMaxMessageSize, lReadTimeout) {}

std::mailslotclient::mailslotclient() : mailslot() {};

std::mailslotclient::mailslotclient(std::wstring&& name)
{
    mailslot slot;
    auto slotname = L"\\\\.\\mailslot\\" + name;

    handle = CreateFileW(slotname.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_READ,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);
}

std::mailslotclient::mailslotclient(std::string&& name)
{
    auto slotname = "\\\\.\\mailslot\\" + name;

    handle = CreateFileA(slotname.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_READ,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0);
}

std::mailslotclient::mailslotclient(std::wstring& name) : mailslotclient(std::move(name)) {}

std::mailslotclient::mailslotclient(std::string& name) : mailslotclient(std::move(name)) {}


bool std::mailslotview::isValid()
{
    if (handle == INVALID_HANDLE_VALUE)
    {
       
        return FALSE;
    }
    return TRUE;
}

std::mailslot::info std::mailslotview::GetInfo()
{
    info t = {};
    auto success = GetMailslotInfo(handle, (DWORD*)&t.MaxMessageSize, (DWORD*)&t.NextSize, (DWORD*)&t.MessageCount, (DWORD*)&t.ReadTimeout);
    if (success == FALSE)
    {
        throw GetLastError();
    }
    return t;
}

void std::mailslotview::SetInfo(uint32_t readtimeout)
{
    auto success = SetMailslotInfo(handle, readtimeout);
    if (success == FALSE)
    {
        throw GetLastError();
    }
}

uint32_t std::mailslotserver::Read(void* bf,int msgSize)
{
    
    uint32_t numRead = 0;
    auto success = ReadFile(handle, bf, msgSize, (DWORD*)&numRead, NULL);
    
    if (success == FALSE)
    {
        throw GetLastError();
    }
    
    return numRead;
}

std::string std::mailslotserver::ReadString()
{
    auto info = GetInfo();
    std::string str(info.NextSize, '0');
    Read((void*)str.c_str(), info.NextSize);

    return str;
}

uint32_t std::mailslotclient::Write(void* bf, int msgSize)
{
    uint32_t numwritten = 0;

    auto success = WriteFile(handle, bf, msgSize, (DWORD*)&numwritten, NULL);
    if (success == FALSE)
    {
        throw GetLastError();
    }
    return numwritten;
}

void std::mailslotclient::WriteString(std::string&& str)
{
    Write((void*)str.c_str(), str.size());
}

void std::mailslotclient::WriteString(std::string& str) {
    WriteString(std::move(str));
}







void std::mailslotview::Close()
{
    if (handle != NULL)
    {
        CloseHandle(handle);
        handle = NULL;
    }
}

std::mailslot::mailslot(HANDLE _handle) : mailslotview(handle) {}

std::mailslot::~mailslot()
{
    if (handle != NULL)
    {
        CloseHandle(handle);
        handle = NULL;
    }
}
