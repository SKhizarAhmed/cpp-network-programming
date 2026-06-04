#include <iostream>
#include <string>
#include <winsock2.h>
#include <tchar.h> // This is the specific header that handles the _T() macro
#include <ws2tcpip.h> // <-- ADD THIS LINE FOR InetPton
using namespace std;
int main() {

	// STEP 1: Initialize WSAStartup()

	WSADATA OSData;
	int wsaCheck;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaCheck = WSAStartup(wVersionRequested, &OSData);
	if (wsaCheck != 0) {
		cout << "The winsock dll not found" << endl;
		return -1;
	}
	else {
		cout << "Winsock dll sucessfully found" << endl;
		cout << "The status: " << OSData.szSystemStatus << endl;
	}

	// STEP 2: Prepare target DATA
	
	sockaddr_in scanner_data;
	scanner_data.sin_family = AF_INET;



	// SET THE TARGET IP HERE

	InetPton(AF_INET, _T("192.168.1.6"), &scanner_data.sin_addr.s_addr);
	
	// ADD AS Many ports you want but make sure to increase the array size
	// Also chage the condition of loop according to the size
	// E.g: If array size is incread to 20 condition should be k<20. 

	int ports[10] = { 21, 22, 23, 25, 53, 80, 135, 443, 445, 8080 };
	


	// STEP 3: SCAN Loop

	for (int k = 0; k < 10; k++) {
		
		SOCKET scanner = INVALID_SOCKET; 
		// Creates new socket(unbound in each iteration so if a port is closed 
		// the bound socket is destroyed and new one gets created)

		scanner = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		int current_port = ports[k];
		scanner_data.sin_port = htons(current_port);

		if (scanner == INVALID_SOCKET) {
			cout << "Error creating socket for Port " << current_port << " " << WSAGetLastError() << endl;
			continue; //skips rest of the loop statements and jump towards next port since socket
			// for the current_port wasn't created
		}
		

		if (connect(scanner, (SOCKADDR*)&scanner_data, sizeof(scanner_data)) == SOCKET_ERROR) {
			cout << "Port "<<current_port<<" is CLOSED!" << endl;
		}
		else {
			cout << " + Port "<<current_port<<" is OPEN!" << endl;
		}
		closesocket(scanner); //Close socket everytime, to free up resources since each iteration
		// creates a new unbound socket that we connect to a new port, if we dont close socket the older
		// sockets will take up unnecessary space
		// NOTE: Never ever do WSACleanup() here or it will remove the dll necessary for the loop to
		// continue
	}

	// change the IP in 2nd parameter to the target address

	WSACleanup();
	cout << "Scan Completed" << endl;
	system("pause");
	return 0;
}