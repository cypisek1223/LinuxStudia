#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

void handler(int sig) {
    if (sig == SIGUSR1) printf("Czekam na rozkazy\n");
    if (sig == SIGUSR2) printf("Rozpoczynam misje\n");
    if (sig == SIGTERM) { printf("Misja zakonczona\n"); exit(0); }
}

int main() {
    char *imiona[] = {"Gamma", "Beta", "Alpha", "Delta"};
    for (int i = 0; i < 4; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            signal(SIGUSR1, handler);
            signal(SIGUSR2, handler);
            signal(SIGTERM, handler);
            printf("Imie agenta: %s – Gotowy do misji\n", imiona[i]);

            char komenda[32];
            while (fgets(komenda, sizeof(komenda), stdin)) {
                if (strncmp(komenda, "Identify!", 9) == 0)
                    printf("My codename is %s\n", imiona[i]);
                if (strncmp(komenda, "Status?", 7) == 0)
                    printf("%s: My PID is: %d\n", imiona[i], getpid());
            }
            exit(0);
        }
    }
    sleep(10);
    return 0;
}
