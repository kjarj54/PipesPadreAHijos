#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " texto" << endl;
        return 1;
    }

    // Crear una tubería
    int p[2];
    if (pipe(p) == -1) {
        perror("Error al crear la tubería");
        return 2;
    }

    // Hacer un fork para generar el proceso hijo
    pid_t pid = fork();
    if (pid == -1) {
        perror("Error al hacer fork");
        return 3;
    }

    if (pid == 0) {
        close(p[1]);
        // Leer el texto enviado por el padre desde la tubería
        char buffer[100];
        int n = read(p[0], buffer, 100);
        if (n == -1) {
            perror("Error al leer de la tubería");
            return 4;
        }
        cout << "Soy el proceso hijo y he recibido el texto: " << buffer << endl;
        // Cerrar el extremo de lectura de la tubería
        close(p[0]);
    }
    else {
        close(p[0]);
        // Enviar el texto pasado como argumento al hijo por la tubería
        const char *texto = argv[1];
        int n = write(p[1], texto, strlen(texto) + 1);
        if (n == -1) {
            perror("Error al escribir en la tubería");
            return 5;
        }
        // Cerrar el extremo de escritura de la tubería
        close(p[1]);
        // Esperar a que el hijo termine
        wait(NULL);
    }

    return 0;
}
