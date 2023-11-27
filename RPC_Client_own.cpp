#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return -1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating client socket." << std::endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9000);

    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Address conversion failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to the server." << std::endl;

    // ���� ���� �� ����
    int retry = 1;

    while (retry == 1) {
        int clientHand;
        std::cout << "���� ���� �� ������ �����մϴ�." << std::endl;
        std::cout << "����(0), ����(1), ��(2) �� �ϳ��� �����ϼ���(�·� (3), ���� (4)): ";
        std::cin >> clientHand;

        send(clientSocket, (char*)&clientHand, sizeof(clientHand), 0);

        int result;
        recv(clientSocket, (char*)&result, sizeof(result), 0);

        if (result == 0) {
            std::cout << "���º��Դϴ�." << std::endl;
            retry = 1;
        }
        else if (result == 1) {
            std::cout << "������ �̰���ϴ�." << std::endl;
            retry = 0;
        }
        else {
            std::cout << "Ŭ���̾�Ʈ�� �̰���ϴ�." << std::endl;
            retry = 0;
        }
    }


    // ����� ����
    while (true) {
        int serverChoice;
        recv(clientSocket, (char*)&serverChoice, sizeof(serverChoice), 0);

        std::cout << "������ ������ ��: ";
        if (serverChoice == 0) {
            std::cout << "��";
        }
        else if (serverChoice == 1) {
            std::cout << "��";
        }
        else {
            std::cout << "��";
        }
        std::cout << std::endl;

        char clientChoice;
        std::cout << "��(0), ��(1), ��(2) �� �ϳ��� �����ϼ���: ";
        std::cin >> clientChoice;

        send(clientSocket, &clientChoice, sizeof(clientChoice), 0);

        char result[50];
        recv(clientSocket, result, sizeof(result), 0);
        std::cout << "���: " << result << std::endl;

        /*char playAgain;
        std::cout << "������ ����Ͻðڽ��ϱ�? (y/n): ";
        std::cin >> playAgain;

        send(clientSocket, &playAgain, sizeof(playAgain), 0);

        if (playAgain != 'y') {
            break;
        }*/
    }

    // ���� �ݱ�
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}