#include <iostream>
#include <chrono>
#include <thread>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <cstring>

using namespace std;

int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        cout << "tcp_ping IP port" << endl;
        return 0;
    }
    
    struct addrinfo hints;
    struct addrinfo *res;
    
    memset(&hints, 0, sizeof(hints));
    
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_DEFAULT;
    hints.ai_family = AF_INET;
    
    int status = 0;
    
    if ((status = getaddrinfo(*(argv + 1), *(argv + 2), &hints, &res))) {
        cerr << "getaddrinfo error: " << gai_strerror(status) << endl;
        return -1;
    }
    
    struct addrinfo *next = res;
    
    if (nullptr == next) {
        cerr << "getaddrinfo returns nullptr" << endl;
        return -1;
    }
    
    int sock_fd = 0;
    while (true) {
        sock_fd = socket(next->ai_family, next->ai_socktype, next->ai_protocol);
        
        if (-1 == sock_fd) {
            cerr << "socket failed" << endl;
            break;
        }
        
        chrono::time_point<chrono::system_clock> begin = chrono::system_clock::now();
        status = connect(sock_fd, next->ai_addr, next->ai_addrlen);
        if (-1 == status) {
            cerr << "connect failed" << endl;
            return -1;
        }
        auto dur = chrono::system_clock::now() - begin;
        cout << "Costs " << chrono::duration_cast<chrono::milliseconds>(dur).count() << "ms" << endl;
        
        shutdown(sock_fd, 2);
        
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    
    freeaddrinfo(res);
    cout << "TCP Ping" << endl;
    
    return 0;
}
