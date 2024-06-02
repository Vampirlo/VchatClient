#include <SFML/Network.hpp>
#include <mutex>
#include <condition_variable>

#include "iniManager/iniManager.h"
#include "MessageQueue.h"

std::mutex messageMutex;
std::condition_variable messageCondition;

void receiveMessages(sf::TcpSocket& socket, MessageQueue& messageQueue)
{
    while (true)
    {
        char data[255];
        std::size_t received;
        if (socket.receive(data, sizeof(data), received) == sf::Socket::Done)
        {
            // Помещаем полученное сообщение в очередь сообщений
            std::lock_guard<std::mutex> lock(messageMutex);
            messageQueue.push(std::string(data, received));
            messageCondition.notify_one();
        }
    }
}

void printReceiveMessage(MessageQueue& messageQueue)
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(messageMutex);
        messageCondition.wait(lock, [&messageQueue] { return !messageQueue.empty(); });

        std::string serverMessage = messageQueue.pop();
        lock.unlock();

        std::cout << "Message from server: " << serverMessage << std::endl;
    }
}

int main(int argc, char* argv[])
{
    //initialization file processing       Получаем путь к исполняющему файлу           Получаем директорию исполняющего файла
    std::filesystem::path executablePath = std::filesystem::absolute(std::filesystem::path(argv[0]));
    std::filesystem::path directory = executablePath.parent_path();

    // автоматически создавать ini файл
    checkingTheExistenceOfValues(directory, "Network", "debug");
    checkingTheExistenceOfValues(directory, "Network", "Server_ip");
    checkingTheExistenceOfValues(directory, "Network", "port");

    std::string debug = iniRead(directory, "Network", "debug");
    std::string serverIp = iniRead(directory, "Network", "Server_ip");
    std::string strPort = iniRead(directory, "Network", "port");

    sf::TcpSocket socket;
    std::string name;
    std::string message;
    MessageQueue messageQueue;

    std::cout << "Enter your name: ";
    std::getline(std::cin, name);
    std::cout << "\n";

    if (socket.connect(serverIp, std::stoi(strPort)) != sf::Socket::Done)
    {
        std::cerr << "Error connecting to server" << std::endl;
        return 1;
    }
    std::cout << "connection established" << std::endl;

    socket.setBlocking(false);

    std::thread receiveMessagesThread(&receiveMessages, std::ref(socket), std::ref(messageQueue));
    std::thread printReceiveMessagesThread(&printReceiveMessage, std::ref(messageQueue));
    // первое сообщение - теряется, пустое сообщение от сервера отправляется
    receiveMessagesThread.detach();
    printReceiveMessagesThread.detach();

    while (true)
    {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        if (message != "")
        {
            if (socket.send(message.c_str(), message.size() + 1) != sf::Socket::Done)
            {
                std::cout << "Error sending message" << std::endl;
            }
        }
    }

    return 0;
}

