// Receiver - Convert to uppercase (FIFO)
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <cctype>
using namespace std;

int main() {
    char text[100];
    
    while(true) {
        int fd = open("/tmp/myfifo", O_RDONLY);
        read(fd, text, 100);
        close(fd);
        for(int i = 0; text[i]; i++)
            text[i] = toupper(text[i]);
        cout << "Message received: " << text << "\n";
        if(strcmp(text, "EXIT") == 0) break;
    }
    
    unlink("/tmp/myfifo");
    return 0;
}

