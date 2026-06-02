# cpp-network-programming
# C++ TCP Socket Toolkit

# A lightweight, low-level network communication toolkit implemented in C++ using the native Windows Sockets (**Winsock2**) API. This project builds a synchronous TCP/IP client-server pipeline to transmit and receive raw text payloads across a local network loopback interface.

Make 2 separate projects, 1st project with server in source file and 2nd project with client code in source file
In 1st project add server code in source file after that right click on solution explorer (a panel on your right side in visual studio, not your project name, e.g if your project name is server it would be
Solution "Server") => right click on that, then click add, new project(if u havenot yet created a project for client) and name it whatever u want, after that add client code in the 2nd project's source file,
then again right click on Solution Explorer click properties at the bottom => Common Properties => Configure startup projects => Multiple startup projects and then from the drop down select "start" for both projects
=> Apply => OK and your done

Also make sure to include ws2_32.lib
Right click your project name (not solution explorer) (for both projects) => click drop down of Configuration Properties => Linker => Input => Drop down of Additional Dependencies = > Edit = > then paste "ws2_32.lib"
in the upper empty text box click OK then Apply then again Ok and your ready to go.
