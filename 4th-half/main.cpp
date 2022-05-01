#include "AVLTree.hpp"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

vector<string> split(string s, string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    token.~basic_string();
    return res;
}

void getRandomInfo(int n)
{
    ofstream fout("infoAll.txt");
    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        int n = 4 + rand() % 6;
        fout << "add"
             << " ";
        fout << (char)((int)'A' + rand() % ((int)'Z' - (int)'A' + 1));

        for (int i = 0; i < n; i++)
        {
            fout << (char)((int)'a' + rand() % ((int)'z' - (int)'a' + 1));
        }

        unsigned long long int phone;
        phone = 0;
        for (int i = 0; i < 7; i++)
        {
            phone = phone * 10 + 0 + rand() % 10;
        }

        int group;
        group = 100 + rand() % 499;

        fout << " " << phone << " ";
        fout << group << endl
             << endl;
    }
    fout.close();
}

int getForAdd(string *name, unsigned long long int *phone, int *group, const std::string &line)
{
    istringstream iss(line);
    string tmp;

    iss >> tmp; // add
    tmp.clear();
    iss >> tmp; // name
    *name = tmp;
    tmp.clear();
    iss >> tmp; // phone
    *phone = atoi(tmp.c_str());
    tmp.clear();
    iss >> tmp; // group
    *group = atoi(tmp.c_str());

    tmp.~basic_string();
    return 1;
}

int main()
{

    int n;
    cin >> n;
    getRandomInfo(n);
    ifstream fileIn("infoAll.txt");
    assert(fileIn);

    AVLTree<string> avl;
    string line;
    srand(time(NULL));
    while (getline(fileIn, line))
    {
        if (line.find("add") == 0)
        {
            istringstream iss(line);
            string tmp;
            string name;
            unsigned long long int phone;
            int group;

            getForAdd(&name, &phone, &group, line);
            avl.insert(name, phone, group);
            tmp.~basic_string();
            name.~basic_string();
        }
    }
    fileIn.close();

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

        string line = string(a);
        if (line.find("end") == 0)
        {
            line.~basic_string();
            avl.~AVLTree();
            return 0;
        }

        if (line.find("delete") == 0)
        {
            istringstream iss(line);
            string tmp;

            iss >> tmp; // delete
            if (tmp != "delete")
            {
                cout << "err" << endl;
                continue;
            }
            tmp.clear();
            iss >> tmp; // where
            if (tmp != "where")
            {
                cout << "err" << endl;
                continue;
            }
            tmp.clear();

            line.clear();
            getline(iss, line);
            line = &line[1];

            string parseOR = " or ";
            string parseAND = " and ";
            vector<string> foror = split(line, parseOR);

            for (auto block : foror)
            {
                vector<string> forand = split(block, parseAND);
                string name = "";
                unsigned long long int phone = 0;
                int group = 0;

                for (auto element : forand)
                {
                    if (element.find("name") == 0)
                    {
                        if (name == "")
                        {
                            name = &element[element.find("=") + 1];
                        }
                        else
                        {
                            cout << "something wrong with name" << endl;
                        }
                        continue;
                    }
                    if (element.find("phone") == 0)
                    {
                        if (phone == 0)
                        {
                            phone = atoi(&element[element.find("=") + 1]);
                        }
                        else
                        {
                            cout << "something wrong with phone" << endl;
                        }
                        continue;
                    }
                    if (element.find("group") == 0)
                    {
                        if (group == 0)
                        {
                            group = atoi(&element[element.find("=") + 1]);
                        }
                        else
                        {
                            cout << "something wrong with group" << endl;
                        }
                        continue;
                    }
                    else
                    {
                        cout << "something wrong" << endl;
                    }
                }

                if (name == "" && phone == 0 && group == 0)
                {
                    name.~basic_string();
                    forand.~vector();
                    continue;
                }

                avl.remove(name, phone, group);
                name.~basic_string();
                forand.~vector();
            }

            foror.~vector();
            parseOR.~basic_string();
            parseAND.~basic_string();
            tmp.~basic_string();
            continue;
        }
        if (line.find("print") == 0)
        {
            avl.printInOrder();
            continue;
        }
        if (line.find("add") == 0)
        {
            string name;
            unsigned long long int phone;
            int group;

            if (getForAdd(&name, &phone, &group, line))
            {
                avl.insert(name, phone, group);
                cout << "OK" << endl;
            }
            name.~basic_string();
            continue;
        }
        if (line.find("search") == 0)
        {
            istringstream iss(line);
            string tmp;

            iss >> tmp; // search
            if (tmp != "search")
            {
                cout << "err" << endl;
                continue;
            }
            tmp.clear();
            iss >> tmp; // where
            if (tmp != "where")
            {
                cout << "err" << endl;
                continue;
            }
            tmp.clear();

            line.clear();
            getline(iss, line);
            line = &line[1];

            string parseOR = " or ";
            string parseAND = " and ";
            vector<string> foror = split(line, parseOR);

            for (auto block : foror)
            {
                vector<string> forand = split(block, parseAND);
                string name = "";
                unsigned long long int phone = 0;
                int group = 0;

                for (auto element : forand)
                {
                    if (element.find("name") == 0)
                    {
                        if (name == "")
                        {
                            name = &element[element.find("=") + 1];
                        }
                        else
                        {
                            cout << "something wrong with name" << endl;
                        }
                        continue;
                    }
                    if (element.find("phone") == 0)
                    {
                        if (phone == 0)
                        {
                            phone = atoi(&element[element.find("=") + 1]);
                        }
                        else
                        {
                            cout << "something wrong with phone" << endl;
                        }
                        continue;
                    }
                    if (element.find("group") == 0)
                    {
                        if (group == 0)
                        {
                            group = atoi(&element[element.find("=") + 1]);
                        }
                        else
                        {
                            cout << "something wrong with group" << endl;
                        }
                        continue;
                    }
                    else
                    {
                        cout << "something wrong" << endl;
                    }
                }

                if (name == "" && phone == 0 && group == 0)
                {
                    name.~basic_string();
                    forand.~vector();
                    continue;
                }
                avl.search(name, phone, group);
                name.~basic_string();
                forand.~vector();
            }

            foror.~vector();
            parseOR.~basic_string();
            parseAND.~basic_string();
            tmp.~basic_string();
            continue;
        }

        closesocket(NS);
        cout << "-------------------->\n";
    }
    closesocket(S);

    line.~basic_string();
    avl.~AVLTree();
    return 0;
}
