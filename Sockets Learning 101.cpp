#include <iostream>
#include <winsock2.h>
#include <tchar.h> // This is the specific header that handles the _T() macro
#include <ws2tcpip.h> // <-- ADD THIS LINE FOR InetPton

using namespace std;

int main()
{

    // Here 0 means success any other code means failed


    // STEP 1: INITIALIZE WSAStartup()

    WSADATA wsaData; // Create a variable with type WSAData think of it like it's a database variable
    // in which windows/OS will store its information, like a class with certain properties

    int wsaerr; // To store 0 if success and any other code if failed
    WORD wVersionRequested = MAKEWORD(2, 2);

    // MAKEWORD Takes the low order byte (right side / Major) 
    // and the high order byte (left side / Minor) and combines them.



    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0) {
        cout << "The Winsock dll not found";
        return 0;
    }
    else {
        cout << "The Winsock dll found" << endl;
        cout << "The status: " << wsaData.szSystemStatus << endl;
    }

    //STEP 2: Create A Socket //Virtual Phone / Device 

    SOCKET serverSocket = INVALID_SOCKET;

    // SOCKET is a datatype
    //Default value if socket isn't created

    serverSocket = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);

    // Create of socket in the socket variable
    // 1. IPV4 Type
    // 2. SOCK_STREAM FOR TCP SOCK_DGRAM for UDP // Type of Transmission
    // 3. IPPROTO_TCP FOR TCP IPPROTO_UDP FOR UDP //Protocol


    if (serverSocket == INVALID_SOCKET) {
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup(); //To clear out OR remove the winsock dll
        return 0;
    }
    else {
        cout << "socket() is OK!" << endl;
    }

    //STEP 3: Bind Socket

    int port = 55000;

    sockaddr_in service; // An object of structure sockaddr_in (newer structure)



    service.sin_family = AF_INET; //Assings the family property of service to IPV4
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    
    //This function has 3 parameters 
    // 1. IPV4 TYPE
    // 2. The IP address string that needs to be converted to binary
    // 3. The exact pointer address of the variable in which data is to be stored (so that it doesn't)
    // store it in a new copy



    service.sin_port = htons(port); // Will intialize the port later e.g 55000
    //htons converts the interpretation of port by cpus(they interpret differently)
    // into a standard for routers

    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        
        //SOCKADDR* tpye cast service object into older compatible type
        // sizeof() is used to prevent the compiler to read more than the limit

        cout << "bind() failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket); //because the socket was created succesfully in step 2
        WSACleanup();
        return 0;
    }
    else {
        cout << "bind() is OK!" << endl;
    }

    //STEP 4: LISTEN


    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        cout << "listen(): Error listening on socket " << WSAGetLastError() << endl;
        closesocket(serverSocket); //because the socket was created succesfully in step 2
        WSACleanup();
        return -1;
    }

    //listen(serverSocket, 1) 1st argument is the socket created
    // 2nd is the number of connections allowed (OS dependent)

    else {
        cout << "listen() is OK, I'm waiting for connections... " << endl;
    }

    //STEP 5: Accept A Connection
    

    // accept() is a blocking function, such that it halts the program and waits for the client to
    // connect thus preventing the original listen socket to check for more connections/puts in queue
    // accept returns a description for a completely new socket with which server will talk to client
    
    SOCKET acceptSocket;
    acceptSocket = accept(serverSocket, NULL, NULL);

    // 1st parameter = listening socket
    // 2nd parameter = optional structure containing client address info if we only want to talk to a 
    // certain client
    // 3rd parameter = optional size of the address structure (if included)

    if (acceptSocket == INVALID_SOCKET) {
        cout << "accept failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket); //because the socket was created succesfully in step 2
        WSACleanup();
        return -1; // Tells the OS something went wrong / better practice to use 
    }
    else {
        cout << "Client succesfully connected!" << endl;
    }

    // STEP 6: Send and Receive Data

    char receiveBuffer[200] = "";
    char sendBuffer[200];
    // here we use acceptSocket because it is a separate socket that connects server to the client 
    int byteCount = recv(acceptSocket, receiveBuffer, 200, 0);
    if (byteCount == SOCKET_ERROR) {
        printf("Server: recv error %ld.\n",WSAGetLastError());
        
        closesocket(acceptSocket);
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }
    else {
        printf("The bytes received are: %ld", byteCount);
        printf("\nReceived data: %s \n", receiveBuffer);
    }

    printf("\nSend Data to the client: ");
    cin.getline(sendBuffer, 200);

    byteCount = send(acceptSocket, sendBuffer, strlen(sendBuffer)+1, 0);

    if (byteCount == SOCKET_ERROR) {
        printf("Server: send error %ld.\n", WSAGetLastError());

        closesocket(acceptSocket);
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }
    else {
        printf("\nServer: sent %ld bytes\n", byteCount);
    }


    // STEP 7: Close socket

    closesocket(acceptSocket);
    closesocket(serverSocket); // it is in last for the case
                               // if we succeed in every step so in the end the connecton is closed
    WSACleanup();
    return 0; 
    
    
    // use return 0 at the end to show everything worked without error
    // use return -1 in if statement to show that something went wrong
}


// ============================================================================
// Sending an object. Assuming the class Data has been defined
// elsewhere. Cast the object’s address to a char *
// 
// Data data;
// data.health = 100;
// byteCount = send(socket, (char *)&data, sizeof(Data), 0);
// ============================================================================

// ============================================================================
// Receiving an object
// 
// Data data;
// byteCount = recv(clientSocket, (char *) &data, sizeof(Data), 0);
// printf("Health : \"%d\"\n", data.health);
// 
// 
// Should have definition for the class in both server and client
// 
// 
// Will work without error in local host since the memory address pointer is the same for server and
// client, but wont work other than that cus the memory address of the receiving end could have other
// stuff.
// Also the class/structure needs to be defined for both sides to interpret the object.
// 
// ============================================================================

