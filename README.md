# UdeABook

Proyecto del Desafio II para la materia Informatica II (2026-2).
Sistema de red social por consola desarrollado en C++ con programacion orientada a objetos y gestion manual de memoria.

## Caracteristicas del sistema

- Sin uso de STL ni structs: todas las estructuras de datos fueron implementadas de forma dinamica usando arreglos y punteros.
- Manejo de cadenas propio: modulo UtilidadesCadena para todas las operaciones de texto sin librerias de C ni std::string.
- Modelo de clases:
  - Foto: ruta, fecha, tamano y conteo de likes.
  - Album: clase base abstracta con clases hijas AlbumPublico, AlbumAmigos y AlbumPrivado. Incluye carrusel circular.
  - Mensaje: texto, fecha, remitente, destinatario y busqueda por rango de fechas.
  - SolicitudAmistad: control de solicitudes con estados pendiente, aceptada o rechazada.
  - Usuario: administra arreglos dinamicos de amigos, albumes, mensajes y solicitudes. Incluye seleccion de 3 mejores amigos por ponderacion.
  - Programa: controladora principal que maneja la autenticacion, carga y guardado del archivo datos_udeabook.txt, y la matriz de adyacencia para encontrar amigos en comun y sugerencias.
- Metricas de eficiencia: tras cada accion se reportan las iteraciones realizadas y la memoria consumida en bytes.