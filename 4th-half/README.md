# sql-server-client

Server-client on C++ for Win. Step to run:
- Clone git repository
- Compile with command:
  - `g++ main.cpp -l "ws2_32" -o main`
  - `g++ client.cpp -l "ws2_32" -o client`
- Run server `main.exe`
- Enter the number of initial elements of the tree (random name, phone, groups are generated)
- Run client.exe
- Client supports requests such as:
  - `print` - print tree
  - `end` - stop server and client
  - `searct where name = ... (phone = ..., group = ...)` - combine with `or` and `and`
  - `delete where name = ... (phone = ..., group = ...)` - combine with `or` and `and`
