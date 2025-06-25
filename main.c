#include <stdio.h>
#include "funciones.h"

int main() {
    int opcion;
    do {
        mostrarMenu();
        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n');
            opcion = -1;
        } else {
            getchar();
        }
        switch (opcion) {
            case 1: crearProyecto(); break;
            case 2: agregarTarea(); break;
            case 3: modificarProyecto(); break;
            case 4: modificarTarea(); break;
            case 5: eliminarTarea(); break;
            case 6: eliminarProyecto(); break;
            case 7: listarTareas(); break;
            case 8: progresoProyecto(); break;
            case 9: listarTareasPorResponsable(); break;
            case 0: printf("Hasta luego\n"); break;
            default: printf("Opcion invalida\n");
        }
    } while (opcion != 0);

    liberarProyectos();

    return 0;
}