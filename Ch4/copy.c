#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[]) {
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("usage: %s old-file new-file\n", argv[0]);
        return 1;
    }

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        perror("open");
        return 1;
    }

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IWOTH;
    outputFd = open(argv[2], openFlags, filePerms);    
    if (outputFd == -1) {
        perror("open");
        return 1;
    }

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
        if(write(outputFd, buf, numRead) != numRead) {
            perror("write");
            return 1;
        }
    }
    if (numRead == -1) {
        perror("read");
        return 1;
    }
    if (close(inputFd) == -1) {
        perror("close");
        return 1;
    }
    if (close(outputFd) == -1) {
        perror("close");
        return 1;
    }
    return 0;
}