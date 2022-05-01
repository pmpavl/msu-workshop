#include <iostream>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{
    WSAData WSData;
    WSAStartup(0x101, &WSData);
    SOCKET S = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in SA;
    SA.sin_family = AF_INET;
    SA.sin_port = htons(3135);
    SA.sin_addr.S_un.S_addr = INADDR_ANY;
    bind(S, (sockaddr *)&SA, sizeof(SA));
    listen(S, SOMAXCONN);
    SOCKET NS;
    while ((NS = accept(S, 0, 0)) != INVALID_SOCKET)
    {
        cout << "-------------------->\n";
        cout << "CONNECT...\n";
        char n = 0;
        recv(NS, &n, 1, 0);
        cout << "Received Length = " << int(n) << "\n";
        char *a = new char[100];
        recv(NS, a, n, 0);
        cout << "Received: \"";
        for (int i = 0; i < n; i++)
        {
            cout << a[i];
        }
        cout << "\"\n";
        closesocket(NS);
        cout << "-------------------->\n";
    }
    closesocket(S);
    return 0;
}
