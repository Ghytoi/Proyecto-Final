#ifndef FUNCIONES_H
#define FUNCIONES_H

#define MAX_TAREAS 100
#define HASHMAP_SIZE 101

typedef enum {
    POR_HACER,
    EN_PROCESO,
    COMPLETADO
} Estado;

typedef enum {
    ALTA,
    MEDIA,
    BAJA
} Prioridad;

typedef struct {
    char descripcion[200];
    char responsable[100];
    char fechaLimite[11];
    Estado estado;
    Prioridad prioridad;
} Tarea;

typedef struct {
    char nombre[100];
    char descripcion[300];
    char fechaInicio[11];
    char fechaFin[11];
    Tarea* tareasAlta[MAX_TAREAS];
    int numAlta;
    Tarea* tareasMedia[MAX_TAREAS];
    int numMedia;
    Tarea* tareasBaja[MAX_TAREAS];
    int numBaja;
} Proyecto;

typedef struct NodoProyecto {
    Proyecto* proyecto;
    struct NodoProyecto* next;
} NodoProyecto;

extern NodoProyecto* proyectos;

typedef struct {
    char responsable[100];
    Tarea** tareas;
    int size;
    int capacity;
} ListaTareasResponsable;

typedef struct NodoHash {
    char clave[100];
    ListaTareasResponsable* valor;
    struct NodoHash* next;
} NodoHash;

extern NodoHash* hashmap[HASHMAP_SIZE];

// Funciones generales
void leerLinea(char* buffer, int largo);
void crearProyecto();
Proyecto* buscarProyecto(const char* nombre);
void agregarTarea();
void listarTareas();
void progresoProyecto();
void modificarProyecto();
void modificarTarea();
void eliminarProyecto();
void eliminarTarea();
void liberarProyectos();
void mostrarMenu();

// Funciones hashmap responsable
unsigned int hash(const char* key);
ListaTareasResponsable* crearListaTareasResponsable(const char* responsable);
void insertarTareaOrdenado(ListaTareasResponsable* lista, Tarea* t);
void eliminarTareaDeResponsable(Tarea* t, const char* responsableAnt, Prioridad prioridadAnt);
ListaTareasResponsable* buscarResponsable(const char* responsable);
void agregarTareaAResponsable(Tarea* t);
void liberarHashMap();
void listarTareasPorResponsable();

#endif