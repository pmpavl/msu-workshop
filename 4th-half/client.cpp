#include <iostream>
#include <string>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{
    while (1)
    {
        WSAData WSData;
        WSAStartup(0x101, &WSData);
        SOCKET S = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in SA;
        SA.sin_family = AF_INET;
        SA.sin_port = htons(3135);
        SA.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

        connect(S, (sockaddr *)&SA, sizeof(SA));
        string s;
        getline(cin, s);
        char len = s.length();
        send(S, &len, 1, 0);
        send(S, s.c_str(), s.length(), 0);
        if (s.find("end") == 0)
        {
            s.clear();
            closesocket(S);
            return 0;
        }
        s.clear();
        closesocket(S);
    }

    return 0;
}
