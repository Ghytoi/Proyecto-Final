Gestor de Tareas y Proyectos en C
Esta aplicación de consola, escrita completamente en lenguaje C, permite gestionar proyectos y tareas de forma estructurada, 
utilizando estructuras de datos como listas enlazadas, arreglos y tablas hash para una manipulación eficiente de la información.
El sistema fue diseñado para simular el flujo de trabajo de herramientas como Trello o Asana, en un entorno puramente textual.

Características Generales
- Crear proyectos con nombre, descripción y fechas de inicio y término.
- Agregar tareas a proyectos existentes, indicando responsable, estado, prioridad y fecha límite.
- Las tareas se organizan por prioridad dentro de cada proyecto (alta, media, baja).
- Modificar información tanto de proyectos como de tareas individualmente.
- Eliminar tareas específicas o proyectos completos junto con sus tareas asociadas.
- Consultar el progreso de un proyecto en función de tareas completadas.
- Consultar todas las tareas asignadas a un responsable, ordenadas automáticamente por prioridad.
- Gestión dinámica de memoria para listas de tareas por responsable.
  
Estructura del Código
El código está dividido en varios archivos para lograr modularidad y facilidad de mantenimiento:
- main.c: Punto de entrada principal del programa. Implementa el menú de usuario y el bucle de ejecución.
- funciones.h: Archivo de cabecera donde se definen todas las estructuras de datos usadas (Tarea, Proyecto,
  ListaTareasResponsable, NodoProyecto, NodoHash), enumeraciones (Estado, Prioridad) y los prototipos de las funciones.
- funciones.c: Contiene toda la implementación funcional del sistema, desde la creación y modificación de proyectos y
  tareas, hasta la lógica de las estructuras de datos como la tabla hash de responsables.

Estructuras de Datos Utilizadas
1. Estructura Proyecto:
Contiene campos básicos como nombre, descripción, fecha de inicio y fin. Además, agrupa tareas en tres listas separadas (arreglos estáticos)
según su prioridad: alta, media y baja. Esto facilita el acceso y organización por prioridad dentro de cada proyecto.
3. Estructura Tarea:
Cada tarea posee una descripción, el nombre del responsable, la fecha límite, estado (por hacer, en proceso, completado) y prioridad (alta, media, baja).
4. Estructura ListaTareasResponsable:
Implementa una lista dinámica de punteros a tareas asignadas a un mismo responsable. Cada lista se almacena en un hashmap,
permitiendo acceso rápido por nombre del responsable. Las tareas dentro de cada lista están ordenadas automáticamente por prioridad.
6. Tabla Hash (HashMap):
Se implementa como un arreglo de listas enlazadas (colisiones abiertas). La clave es el nombre del responsable,
y el valor es un puntero a su ListaTareasResponsable. El hash se calcula con una función personalizada usando aritmética sobre caracteres en minúscula.

Funciones Clave
- crearProyecto(): Solicita los datos del usuario, reserva memoria para el proyecto y lo inserta en la lista enlazada de proyectos.
- agregarTarea(): Permite ingresar una nueva tarea a un proyecto específico. Dependiendo de la prioridad, la tarea se almacena en el arreglo
  correspondiente dentro del proyecto. También se actualiza el hashmap por responsable, agregando ordenadamente la tarea.
- modificarTarea(): Permite cambiar el contenido de una tarea (descripción, responsable, prioridad, estado). Si el responsable o prioridad cambian,
  la tarea se reubica en las estructuras correspondientes.
- eliminarTarea(): Permite eliminar una tarea por índice dentro de su prioridad. Libera memoria y actualiza el hashmap de responsables.
- listarTareasPorResponsable(): Recorre la tabla hash e imprime todas las tareas asignadas a cada responsable, mostrándolas ordenadas por prioridad.
- progresoProyecto(): Calcula el porcentaje de tareas completadas dentro de un proyecto, considerando todas las prioridades.

Requisitos del Sistema
- Sistema operativo compatible con C (Linux, Windows, macOS)
- Compilador C estándar (GCC o equivalente)
- Consola para entrada y salida.

Compilación y Ejecución
Para compilar el programa, se recomienda usar GCC desde la terminal:
gcc main.c funciones.c -o gestor
Y para ejecutarlo:
./gestor
