// Sender - FIFO (Named Pipe)
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
using namespace std;

int main() {
    mkfifo("/tmp/myfifo", 0666);
    char text[100];
    
    while(true) {
        cout << "Message: ";
        cin.getline(text, 100);
        int fd = open("/tmp/myfifo", O_WRONLY);
        write(fd, text, sizeof(text));
        close(fd);
        if(strcmp(text, "exit") == 0) break;
    }
    return 0;
}

