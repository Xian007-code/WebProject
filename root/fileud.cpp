#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUFFER_SIZE = 1024;

std::string readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (file) {
        std::ostringstream ss;
        ss << file.rdbuf();  // 将文件内容读入输出字符串流
        return ss.str();  // 返回字符串流中的内容
    }
    return "";  // 文件读取失败时返回空字符串
}

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::binary);
    if (file) {
        file << content;  // 将字符串内容写入文件
    }
}

void handleRequest(int clientSocket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);  // 接收客户端发送的数据
    if (bytesRead > 0) {
        std::string request(buffer, bytesRead);  // 将接收到的数据转换为字符串格式

        // 解析请求行和请求头部分（此处省略）

        // 提取请求方法、请求路径等信息，根据需要进行逻辑处理

        // 文件下载处理
        if (request.find("GET") == 0) {  // 如果请求方法是 GET
            std::string filePath = "xxx.mp4";  // 文件路径
            std::string fileContent = readFile(filePath);  // 读取文件内容

            std::string response = "HTTP/1.1 200 OK\r\n";  // 构造响应头部
            response += "Content-Type: application/octet-stream\r\n";
            response += "Content-Disposition: attachment; filename=\"file.txt\"\r\n";
            response += "Content-Length: " + std::to_string(fileContent.size()) + "\r\n\r\n";  // 文件内容长度
            response += fileContent;  // 文件内容

            send(clientSocket, response.c_str(), response.size(), 0);  // 发送响应给客户端
        }

        // 文件上传处理
        if (request.find("POST") == 0) {  // 如果请求方法是 POST
            size_t pos = request.find("\r\n\r\n");  // 查找请求头和请求体的分隔位置
            if (pos != std::string::npos) {
                std::string fileContent = request.substr(pos + 4);  // 提取请求体部分（即文件内容）
                writeFile("path/to/save/file.txt", fileContent);  // 将文件内容写入指定路径的文件

                std::string response = "HTTP/1.1 200 OK\r\n";  // 构造响应头部
                response += "Content-Length: 0\r\n\r\n";  // 响应内容长度为0

                send(clientSocket, response.c_str(), response.size(), 0);  // 发送响应给客户端
            }
        }
    }

    close(clientSocket);  // 关闭与客户端的连接
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);  // 创建套接字
    if (serverSocket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {  // 绑定套接字到指定地址和端口
        std::cerr << "Failed to bind socket" << std::endl;
        return -1;
    }

    if (listen(serverSocket, 5) < 0) {  // 开始监听连接请求
        std::cerr << "Failed to listen on socket" << std::endl;
        return -1;
    }

    std::cout << "Server started. Listening on port 8080..." << std::endl;

    while (true) {
        struct sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);  // 接受客户端连接请求
        if (clientSocket < 0) {
            std::cerr << "Failed to accept client connection" << std::endl;
            continue;
        }

        handleRequest(clientSocket);  // 处理客户端请求

        close(clientSocket);  // 关闭与客户端的连接
    }

    close(serverSocket);  // 关闭服务器套接字

    return 0;
}
