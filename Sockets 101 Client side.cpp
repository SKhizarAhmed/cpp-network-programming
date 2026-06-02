// See Read Me
#include <iostream>
#include <winsock2.h>
#include <tchar.h> // This is the specific header that handles the _T() macro
#include <ws2tcpip.h> // <-- ADD THIS LINE FOR InetPton
using namespace std;
/* STEP 1: Intialize WSAStartup()
* STEP 2: Create a Socket
* STEP 3: Connect to the Server
* STEP 4: Send and Receive Data
* STEP 5: Disconnect --- CloseSocket()

		No need for bind and listen
		Binding is performed automatically within the client

*/



int main() {

	// STEP 1: Intialize WSAStartup()

	WSAData wsData;
	int wsaCheck;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaCheck = WSAStartup(wVersionRequested, &wsData);
	if (wsaCheck != 0) {
		cout << "The Winsock dll not found";
		return -1;
	}
	else {
		cout << "The Winsock dll found" << endl;
		cout << "The status: " << wsData.szSystemStatus << endl;
	}

	// STEP 2: Create a Socket

	SOCKET clientSocket = INVALID_SOCKET;

	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clientSocket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup(); //To clear out OR remove the winsock dll
		return -1;
	}
	else
	{
		cout << "socket() is OK!" << endl;
	}

	// STEP 3: Connect to the Server

	int port = 55000; // Port of the Server

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);		// IP of Server as 2nd
																			// parameter
	clientService.sin_port = htons(port);



	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {

		// 1st parameter = Descriptor identifies a socket
		// 2nd parameter = Structure conatining server IP address and port
		// 3rd parameter = Size in bytes of address structure
		// Connect will wait 75 seconds for server to respond
		// Returns 0 if successful or SOCKET_ERROR if not


		cout << "Client: connect() - Failed to Connect." << endl;
		closesocket(clientSocket); // Cus socket created in step 2
		WSACleanup();
		return -1;
	}



	else
	{
		cout << "Client: connect() is OK." << endl;
		cout << "Client: Can start sending and receiving data..." << endl;
	}

	// STEP 4: Send and Receive Data
	


	// In this step we need non-zero values to confirm the data was sent (the number of bytes)
	// In other stages 0 indicated success but here it indicates that the message was not sent or
	// received.

	char buffer[200];
	cout << "Enter your message: ";
	cin.getline(buffer, 200);
	int byteCount = send(clientSocket, buffer, strlen(buffer) + 1, 0);

	// 1st parameter = The socket (client or server)
	// 2nd parameter = A pointer to the buffer to the data to be transmitted
	// 3rd parameter = The length, in bytes, of the buffer pointed to by the buf parameter (2nd param)
	// 4th parameter = flags: Optional set of flags that influences the behaviour of this function
	// (No routing etc).

	// In 3rd parameter use "strlen(buffer) + 1" instead of "200" to avoid sending uneccessary junk
	// data also +1 is to include the null terminator for instance the length of our string is 5
	// we add +1 to include the null terminator.



	if (byteCount == SOCKET_ERROR) {

		// If byteCount == SOCKET_ERROR it means there was a problem in sending data
		// if not it means it stored the no. of bytes sent successfully.

		printf("Client send error %ld.\n", WSAGetLastError());
		
		// C++ supports C syntax and we used %ld for long integer so it can show the error code
		// which is an integer and could be long / we could use cout here too.

		closesocket(clientSocket);
		WSACleanup();

		return -1;
	}

	else
	{
		printf("Client: sent %ld bytes\n",byteCount);
	}

	// The recv() function receives data from a connected socket

	// 1st parameter = The descriptor that identifies a socket
	// 2nd parameter = A pointer to the buffer to receive the incoming data
	// 3rd parameter = The length, in bytes, of the buffer pointed to by the buf parameter(2nd param)
	// 4th paramter = flags: Optional set of flags that influences the behaviour of this function
	// If no error occurs recv() returns the number of bytes received, if the connection has been
	// gracefully closed, the return value is zero. Other SOCKET_ERROR is returned.


	char recvData[200];
	byteCount = recv(clientSocket, recvData, 200, 0);

	if (byteCount == SOCKET_ERROR) {

		printf("Client receive error %ld.\n", WSAGetLastError());

		closesocket(clientSocket);
		WSACleanup();

		return -1;
	}
	else {
		printf("The bytes received are: %ld", byteCount);
		printf("\nReceived data: %s \n", recvData);
	}


	// STEP 5: Close socket

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;


}


// ============================================================================
// UDP SOCKETS REFERENCE SUMMARY
// ============================================================================
// 1. Socket Initialization:
//    Use SOCK_DGRAM and IPPROTO_UDP instead of TCP configuration.
//    
//    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//
// 2. Main Characteristics:
//    - Connectionless: No fixed connection between the client and server.
//    - Client: Does NOT use the connect() function.
//    - Server: Binds to an address but does NOT use the accept() function.
//
// 3. Data Transmission Functions:
//    - sendto()   : Used to transmit data packets.
//    - recvfrom() : Used to receive incoming data packets.
// ============================================================================

// Sending data using UDP
// int sendto(SOCKET s, const char *buf, int len, int flags, const struct sockaddr* to, int tolen)
// 1st parameter = Descriptor indentifying a (possibly connected) socket
// 2nd parameter = A pointer to the data to be transmitted
// 3rd parameter = size in bytes of the data pointed by 2nd parameter
// 4th parameter = flags: Flags that specify the way in which the call is made
// 5th parameter = to: an optional pointer to a sockaddr_in structure that contains the address of
// the target socket
// 6th parameter = tolen: size in bytes of the address pointed to by the parameter
// If no error occurs sendto returns the total bytes sent else -1.

// Receiving data using UDP
// recvfrom(socket, buffer, len_buffer, struct sockaddr *from, int *from_len)

// if no error occurs recv recvfrom returns the bytes received else -1
