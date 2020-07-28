#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char **argv) {
    int a = 0;

    while (a < 250) {
        char name[100];
        char nik_name[100];
        snprintf(name, 100, "bot00%d", rand() * 32);
        snprintf(nik_name, 100, "bot))%d", rand() * 32);
        char *ar[] = {"1", "10.111.2.12", "1111", name, nik_name, NULL};
        pid_t pid;
        pid = fork();
        if (pid < 0)
            exit(1);
         else if (pid == 0)
            execvp("./bot", ar);
        a++;
    }
    return 0;
}

