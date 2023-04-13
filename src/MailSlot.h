#pragma once
#include <windows.h>
#include <cinttypes>
#include <string>
namespace std
{
	class mailslot
	{
	public:
		struct info
		{
			uint32_t MaxMessageSize;
			uint32_t NextSize;
			uint32_t MessageCount;
			uint32_t ReadTimeout;
		};

		mailslot();

		bool isValid();
		info GetInfo();
		void SetInfo(uint32_t readtimeout);

		void Close();
	protected:
		HANDLE handle;
	};
	class mailslotserver : public mailslot
	{
	public:
		mailslotserver();
		mailslotserver(std::wstring&& name);
		mailslotserver(std::string&& name);

		uint32_t Read(void* bf, int msgSize);
		std::string ReadString();
		template <typename T>
		T Read();
	};
	class mailslotclient : public mailslot
	{
	public:
		mailslotclient();
		mailslotclient(std::wstring&& name);
		mailslotclient(std::string&& name);

		uint32_t Write(void* bf, int msgSize);
		void WriteString(std::string&& str);
		template <typename T>
		void Write(T&& val);
	};
	

	template <typename T>
	void mailslot_write(mailslotclient& it,T&& v)
	{
		it.Write(&v, sizeof(T));
	}
	template <typename T>
	T mailslot_read(mailslotserver& it)
	{
		T val{};
		it.Read((void*)&val, sizeof(T));
		return val;
	}
	template<typename T>
	inline T mailslotserver::Read()
	{
		
		return mailslot_read<T>(*this);
	}
	template<typename T>
	inline void mailslotclient::Write(T&& val)
	{
		mailslot_write(*this, val);
	}

}


