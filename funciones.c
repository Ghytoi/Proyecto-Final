#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funciones.h"

NodoProyecto* proyectos = NULL;
NodoHash* hashmap[HASHMAP_SIZE] = {NULL};

void mostrarMenu() {
    printf("\n===== Gestion de Tareas y Proyectos =====\n");
    printf("1. Crear proyecto\n");
    printf("2. Agregar tarea a proyecto\n");
    printf("3. Modificar proyecto\n");
    printf("4. Modificar tarea\n");
    printf("5. Eliminar tarea de proyecto\n");
    printf("6. Eliminar proyecto\n");
    printf("7. Listar tareas por proyecto\n");
    printf("8. Ver progreso del proyecto\n");
    printf("9. Listar tareas por responsable\n");
    printf("0. Salir\n");
    printf("Opcion: ");
}

void leerLinea(char* buffer, int largo) {
    fgets(buffer, largo, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

const char* estadoTexto(Estado e) {
    switch(e) {
        case POR_HACER: return "Por hacer";
        case EN_PROCESO: return "En proceso";
        case COMPLETADO: return "Completado";
        default: return "Desconocido";
    }
}

const char* prioridadTexto(Prioridad p) {
    switch(p) {
        case ALTA: return "Alta";
        case MEDIA: return "Media";
        case BAJA: return "Baja";
        default: return "Desconocida";
    }
}

unsigned int hash(const char* key) {
    unsigned int h = 0;
    while (*key) {
        h = (h * 31) + tolower((unsigned char)*key);
        key++;
    }
    return h % HASHMAP_SIZE;
}

ListaTareasResponsable* crearListaTareasResponsable(const char* responsable) {
    ListaTareasResponsable* lista = malloc(sizeof(ListaTareasResponsable));
    strcpy(lista->responsable, responsable);
    lista->size = 0;
    lista->capacity = 10;
    lista->tareas = malloc(sizeof(Tarea*) * lista->capacity);
    return lista;
}

ListaTareasResponsable* buscarResponsable(const char* responsable) {
    unsigned int h = hash(responsable);
    NodoHash* nodo = hashmap[h];
    while (nodo) {
        if (strcasecmp(nodo->clave, responsable) == 0)
            return nodo->valor;
        nodo = nodo->next;
    }
    return NULL;
}

void insertarTareaOrdenado(ListaTareasResponsable* lista, Tarea* t) {
    if (lista->size == lista->capacity) {
        lista->capacity *= 2;
        lista->tareas = realloc(lista->tareas, sizeof(Tarea*) * lista->capacity);
    }
    int i = lista->size - 1;
    while (i >= 0 && lista->tareas[i]->prioridad > t->prioridad) {
        lista->tareas[i + 1] = lista->tareas[i];
        i--;
    }
    lista->tareas[i + 1] = t;
    lista->size++;
}

void agregarTareaAResponsable(Tarea* t) {
    if (!t) return;
    ListaTareasResponsable* lista = buscarResponsable(t->responsable);
    if (!lista) {
        lista = crearListaTareasResponsable(t->responsable);
        unsigned int h = hash(t->responsable);
        NodoHash* nuevoNodo = malloc(sizeof(NodoHash));
        strcpy(nuevoNodo->clave, t->responsable);
        nuevoNodo->valor = lista;
        nuevoNodo->next = hashmap[h];
        hashmap[h] = nuevoNodo;
    }
    insertarTareaOrdenado(lista, t);
}

void eliminarTareaDeResponsable(Tarea* t, const char* responsableAnt, Prioridad prioridadAnt) {
    if (!t || !responsableAnt) return;
    ListaTareasResponsable* lista = buscarResponsable(responsableAnt);
    if (!lista) return;

    int pos = -1;
    for (int i = 0; i < lista->size; i++) {
        if (lista->tareas[i] == t) {
            pos = i;
            break;
        }
    }
    if (pos == -1) return;

    for (int i = pos; i < lista->size -1; i++) {
        lista->tareas[i] = lista->tareas[i+1];
    }
    lista->size--;
}

Proyecto* buscarProyecto(const char* nombre) {
    NodoProyecto* aux = proyectos;
    while (aux) {
        if (strcmp(aux->proyecto->nombre, nombre) == 0)
            return aux->proyecto;
        aux = aux->next;
    }
    return NULL;
}

void crearProyecto() {
    Proyecto* p = malloc(sizeof(Proyecto));
    if (!p) {
        printf("Error de memoria\n");
        return;
    }
    printf("Nombre del proyecto: ");
    leerLinea(p->nombre, 100);
    printf("Descripcion: ");
    leerLinea(p->descripcion, 300);
    printf("Fecha inicio (DD-MM-YY): ");
    leerLinea(p->fechaInicio, 11);
    printf("Fecha fin (DD-MM-YY): ");
    leerLinea(p->fechaFin, 11);

    p->numAlta = 0;
    p->numMedia = 0;
    p->numBaja = 0;

    NodoProyecto* nodo = malloc(sizeof(NodoProyecto));
    nodo->proyecto = p;
    nodo->next = proyectos;
    proyectos = nodo;

    printf("Proyecto creado\n");
}

void agregarTarea() {
    char nombreProyecto[100];
    printf("Nombre del proyecto: ");
    leerLinea(nombreProyecto, 100);
    Proyecto* p = buscarProyecto(nombreProyecto);
    if (!p) {
        printf("Proyecto no encontrado\n");
        return;
    }

    Tarea* t = malloc(sizeof(Tarea));
    if (!t) {
        printf("Error de memoria\n");
        return;
    }

    printf("Descripcion tarea: ");
    leerLinea(t->descripcion, 200);
    printf("Responsable: ");
    leerLinea(t->responsable, 100);
    printf("Fecha limite (YYYY-MM-DD): ");
    leerLinea(t->fechaLimite, 11);

    int estadoInt;
    do {
        printf("Estado (0: Por hacer, 1: En proceso, 2: Completado): ");
        scanf("%d", &estadoInt);
        getchar();
    } while (estadoInt < 0 || estadoInt > 2);
    t->estado = (Estado)estadoInt;

    int prioridadInt;
    do {
        printf("Prioridad (0: Alta, 1: Media, 2: Baja): ");
        scanf("%d", &prioridadInt);
        getchar();
    } while (prioridadInt < 0 || prioridadInt > 2);
    t->prioridad = (Prioridad)prioridadInt;

    if (t->prioridad == ALTA && p->numAlta < MAX_TAREAS)
        p->tareasAlta[p->numAlta++] = t;
    else if (t->prioridad == MEDIA && p->numMedia < MAX_TAREAS)
        p->tareasMedia[p->numMedia++] = t;
    else if (t->prioridad == BAJA && p->numBaja < MAX_TAREAS)
        p->tareasBaja[p->numBaja++] = t;
    else {
        printf("Limite de tareas alcanzado\n");
        free(t);
        return;
    }

    agregarTareaAResponsable(t);

    printf("Tarea agregada\n");
}

void listarTareaArray(Tarea* tareas[], int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        Tarea* t = tareas[i];
        printf("- %s | %s | %s | Estado: %s | Prioridad: %s\n",
               t->descripcion, t->responsable, t->fechaLimite,
               estadoTexto(t->estado), prioridadTexto(t->prioridad));
    }
}

void listarTareas() {
    char nombreProyecto[100];
    printf("Nombre del proyecto: ");
    leerLinea(nombreProyecto, 100);
    Proyecto* p = buscarProyecto(nombreProyecto);
    if (!p) {
        printf("Proyecto no encontrado\n");
        return;
    }

    if (p->numAlta + p->numMedia + p->numBaja == 0) {
        printf("No hay tareas\n");
        return;
    }

    printf("Tareas prioridad ALTA:\n");
    listarTareaArray(p->tareasAlta, p->numAlta);

    printf("Tareas prioridad MEDIA:\n");
    listarTareaArray(p->tareasMedia, p->numMedia);

    printf("Tareas prioridad BAJA:\n");
    listarTareaArray(p->tareasBaja, p->numBaja);
}

void progresoProyecto() {
    char nombreProyecto[100];
    printf("Nombre del proyecto: ");
    leerLinea(nombreProyecto, 100);
    Proyecto* p = buscarProyecto(nombreProyecto);
    if (!p) {
        printf("Proyecto no encontrado\n");
        return;
    }

    int total = p->numAlta + p->numMedia + p->numBaja;
    if (total == 0) {
        printf("Progreso: 0%% completado\n");
        return;
    }

    int completadas = 0;
    for (int i = 0; i < p->numAlta; i++) if (p->tareasAlta[i]->estado == COMPLETADO) completadas++;
    for (int i = 0; i < p->numMedia; i++) if (p->tareasMedia[i]->estado == COMPLETADO) completadas++;
    for (int i = 0; i < p->numBaja; i++) if (p->tareasBaja[i]->estado == COMPLETADO) completadas++;

    printf("Progreso: %.2f%% completado\n", (completadas * 100.0f) / total);
}

void modificarProyecto() {
    char nombre[100];
    printf("Nombre del proyecto a modificar: ");
    leerLinea(nombre, 100);
    Proyecto* p = buscarProyecto(nombre);
    if (!p) {
        printf("Proyecto no encontrado\n");
        return;
    }

    printf("Nueva descripcion: ");
    leerLinea(p->descripcion, 300);
    printf("Nueva fecha inicio (YYYY-MM-DD): ");
    leerLinea(p->fechaInicio, 11);
    printf("Nueva fecha fin (YYYY-MM-DD): ");
    leerLinea(p->fechaFin, 11);

    printf("Proyecto modificado\n");
}

void modificarTarea() {
    char nombreProyecto[100];
    printf("Nombre del proyecto: ");
    leerLinea(nombreProyecto, 100);
    Proyecto* p = buscarProyecto(nombreProyecto);
    if (!p) {
        printf("Proyecto no encontrado\n");
        return;
    }

    printf("Tareas ALTA:\n");
    for (int i = 0; i < p->numAlta; i++) {
        printf("[A%d] %s\n", i, p->tareasAlta[i]->descripcion);
    }
    printf("Tareas MEDIA:\n");
    for (int i = 0; i < p->numMedia; i++) {
        printf("[M%d] %s\n", i, p->tareasMedia[i]->descripcion);
    }
    printf("Tareas BAJA:\n");
    for (int i = 0; i < p->numBaja; i++) {
        printf("[B%d] %s\n", i, p->tareasBaja[i]->descripcion);
    }

    printf("Ingrese prioridad lista de la tarea a modificar (0=ALTA,1=MEDIA,2=BAJA): ");
    int pPrioridad;
    scanf("%d", &pPrioridad);
    getchar();
    if (pPrioridad < 0 || pPrioridad > 2) {
        printf("Prioridad invalida\n");
        return;
    }

    printf("Ingrese indice de la tarea: ");
    int index;
    scanf("%d", &index);
    getchar();

    Tarea* t = NULL;
    if (pPrioridad == ALTA) {
        if (index < 0 || index >= p->numAlta) {
            printf("Indice invalido\n");
            return;
        }
        t = p->tareasAlta[index];
    } else if (pPrioridad == MEDIA) {
        if (index < 0 || index >= p->numMedia) {
            printf("Indice invalido\n");
            return;
        }
        t = p->tareasMedia[index];
    } else {
        if (index < 0 || index >= p->numBaja) {
            printf("Indice invalido\n");
            return;
        }
        t = p->tareasBaja[index];
    }

    char responsableAnt[100];
    strcpy(responsableAnt, t->responsable);
    Prioridad prioridadAnt = t->prioridad;

    printf("Nueva descripcion: ");
    leerLinea(t->descripcion, 200);
    printf("Nuevo responsable: ");
    leerLinea(t->responsable, 100);
    printf("Nueva fecha limite (YYYY-MM-DD): ");
    leerLinea(t->fechaLimite, 11);

    int estadoInt;
    do {
        printf("Estado (0: Por hacer, 1: En proceso, 2: Completado): ");
        scanf("%d", &estadoInt);
        getchar();
    } while (estadoInt < 0 || estadoInt > 2);
    t->estado = (Estado)estadoInt;

    int prioridadInt;
    do {
        printf("Prioridad (0: Alta, 1: Media, 2: Baja): ");
        scanf("%d", &prioridadInt);
        getchar();
    } while (prioridadInt < 0 || prioridadInt > 2);

    if (prioridadInt != prioridadAnt) {
        if (prioridadAnt == ALTA) {
            for (int i = 0; i < p->numAlta; i++) {
                if (p->tareasAlta[i] == t) {
                    for (int j = i; j < p->numAlta - 1; j++)
                        p->tareasAlta[j] = p->tareasAlta[j + 1];
                    p->numAlta--;
                    break;
                }
            }
        } else if (prioridadAnt == MEDIA) {
            for (int i = 0; i < p->numMedia; i++) {
                if (p->tareasMedia[i] == t) {
                    for (int j = i; j < p->numMedia - 1; j++)
                        p->tareasMedia[j] = p->tareasMedia[j + 1];
                    p->numMedia--;
                    break;
                }
            }
        } else {
            for (int i = 0; i < p->numBaja; i++) {
                if (p->tareasBaja[i] == t) {
                    for (int j = i; j < p->numBaja - 1; j++)
                        p->tareasBaja[j] = p->tareasBaja[j + 1];
                    p->numBaja--;
                    break;
                }
            }
        }

        if (prioridadInt == ALTA)
            p->tareasAlta[p->numAlta++] = t;
        else if (prioridadInt == MEDIA)
            p->tareasMedia[p->numMedia++] = t;
        else
            p->tareasBaja[p->numBaja++] = t;

        t->prioridad = (Prioridad)prioridadInt;
    }

    if (strcmp(responsableAnt, t->responsable) != 0 || prioridadInt != prioridadAnt) {
        eliminarTareaDeResponsable(t, responsableAnt, prioridadAnt);
        agregarTareaAResponsable(t);
    }

    printf("Tarea modificada\n");
}

void eliminarTarea() {
    char nombreProyecto[100];
    printf("Nombre del proyecto: ");
    leerLinea(nombreProyecto, 100);
    Proyecto* p = buscarProyecto(nombreProyecto);
    if (!p) {
        printf("Proyecto no encontrado\n");
        return;
    }

    printf("Tareas ALTA:\n");
    for (int i = 0; i < p->numAlta; i++) {
        printf("[A%d] %s\n", i, p->tareasAlta[i]->descripcion);
    }
    printf("Tareas MEDIA:\n");
    for (int i = 0; i < p->numMedia; i++) {
        printf("[M%d] %s\n", i, p->tareasMedia[i]->descripcion);
    }
    printf("Tareas BAJA:\n");
    for (int i = 0; i < p->numBaja; i++) {
        printf("[B%d] %s\n", i, p->tareasBaja[i]->descripcion);
    }

    printf("Ingrese prioridad lista de la tarea a eliminar (0=ALTA,1=MEDIA,2=BAJA): ");
    int pPrioridad;
    scanf("%d", &pPrioridad);
    getchar();
    if (pPrioridad < 0 || pPrioridad > 2) {
        printf("Prioridad invalida\n");
        return;
    }

    printf("Ingrese indice de la tarea: ");
    int index;
    scanf("%d", &index);
    getchar();

    Tarea* t = NULL;
    if (pPrioridad == ALTA) {
        if (index < 0 || index >= p->numAlta) {
            printf("Indice invalido\n");
            return;
        }
        t = p->tareasAlta[index];
        for (int i = index; i < p->numAlta - 1; i++)
            p->tareasAlta[i] = p->tareasAlta[i + 1];
        p->numAlta--;
    } else if (pPrioridad == MEDIA) {
        if (index < 0 || index >= p->numMedia) {
            printf("Indice invalido\n");
            return;
        }
        t = p->tareasMedia[index];
        for (int i = index; i < p->numMedia - 1; i++)
            p->tareasMedia[i] = p->tareasMedia[i + 1];
        p->numMedia--;
    } else {
        if (index < 0 || index >= p->numBaja) {
            printf("Indice invalido\n");
            return;
        }
        t = p->tareasBaja[index];
        for (int i = index; i < p->numBaja - 1; i++)
            p->tareasBaja[i] = p->tareasBaja[i + 1];
        p->numBaja--;
    }

    eliminarTareaDeResponsable(t, t->responsable, t->prioridad);
    free(t);

    printf("Tarea eliminada\n");
}

void eliminarProyecto() {
    char nombreProyecto[100];
    printf("Nombre del proyecto a eliminar: ");
    leerLinea(nombreProyecto, 100);

    NodoProyecto* prev = NULL;
    NodoProyecto* curr = proyectos;

    while (curr) {
        if (strcmp(curr->proyecto->nombre, nombreProyecto) == 0) {
            // Liberar tareas
            for (int i = 0; i < curr->proyecto->numAlta; i++) {
                eliminarTareaDeResponsable(curr->proyecto->tareasAlta[i], curr->proyecto->tareasAlta[i]->responsable, curr->proyecto->tareasAlta[i]->prioridad);
                free(curr->proyecto->tareasAlta[i]);
            }
            for (int i = 0; i < curr->proyecto->numMedia; i++) {
                eliminarTareaDeResponsable(curr->proyecto->tareasMedia[i], curr->proyecto->tareasMedia[i]->responsable, curr->proyecto->tareasMedia[i]->prioridad);
                free(curr->proyecto->tareasMedia[i]);
            }
            for (int i = 0; i < curr->proyecto->numBaja; i++) {
                eliminarTareaDeResponsable(curr->proyecto->tareasBaja[i], curr->proyecto->tareasBaja[i]->responsable, curr->proyecto->tareasBaja[i]->prioridad);
                free(curr->proyecto->tareasBaja[i]);
            }

            free(curr->proyecto);

            if (prev)
                prev->next = curr->next;
            else
                proyectos = curr->next;

            free(curr);

            printf("Proyecto eliminado\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Proyecto no encontrado\n");
}

void liberarProyecto(Proyecto* p) {
    for (int i = 0; i < p->numAlta; i++) free(p->tareasAlta[i]);
    for (int i = 0; i < p->numMedia; i++) free(p->tareasMedia[i]);
    for (int i = 0; i < p->numBaja; i++) free(p->tareasBaja[i]);
    free(p);
}

void liberarProyectos() {
    NodoProyecto* actual = proyectos;
    while(actual != NULL) {
        NodoProyecto* siguiente = actual->next;
        liberarProyecto(actual->proyecto);
        free(actual);
        actual = siguiente;
    }
    proyectos = NULL;
    liberarHashMap();
}

void liberarHashMap() {
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        NodoHash* nodo = hashmap[i];
        while (nodo) {
            NodoHash* siguiente = nodo->next;
            free(nodo->valor->tareas);
            free(nodo->valor);
            free(nodo);
            nodo = siguiente;
        }
        hashmap[i] = NULL;
    }
}

void listarTareasPorResponsable() {
    printf("\nListado de tareas por responsable:\n");
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        NodoHash* nodo = hashmap[i];
        while (nodo) {
            printf("Responsable: %s\n", nodo->valor->responsable);
            for (int j = 0; j < nodo->valor->size; j++) {
                Tarea* t = nodo->valor->tareas[j];
                printf("  - [%s] %s (Fecha limite: %s) Estado: %s\n",
                       prioridadTexto(t->prioridad), t->descripcion, t->fechaLimite,
                       estadoTexto(t->estado));
            }
            nodo = nodo->next;
        }
    }
}