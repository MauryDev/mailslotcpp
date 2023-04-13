# mailslotcpp
MailSlot for C++

## Example

### Server

```cpp
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "MailSlot.h"

void main()
{
    
    
    std::mailslotserver server("server");
    
    char bf[100];

    ZeroMemory(bf, 100);
    while (true)
    {
        auto info = server.GetInfo();
        
        if (info.MessageCount >= 3)
        {
            auto messagetype = server.Read<int>();
            if (messagetype == 2)
            {
                
                std::cout << "Message: " << server.Read<int>() + server.Read<int>() << std::endl;
            }
            

        }
        
        Sleep(100);
    }
    server.Close();
}
```

### Client

```cpp
#include <iostream>
#include <cstdint>
#include <Windows.h>
#include "MailSlot.h"


int main()
{
    std::mailslotclient client("192.168.0.1");
    
    if (!client.isValid())
    {
        printf("CreateFile failed: %d\n", GetLastError());
    }
    int v = 0;
    int v2 = 0;

    while (true)
    {
        std::cin >> v;
        std::cin >> v2;

        client.Write(2);
        client.Write(v);
        client.Write(v2);

        Sleep(1000);
    }
}

```
