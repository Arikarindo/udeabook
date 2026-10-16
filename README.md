# UdeABook

Proyecto del Desafio II para la materia Informatica II (2026-2).
Sistema de red social por consola desarrollado en C++ con programacion orientada a objetos y gestion manual de memoria.

## Caracteristicas del sistema

- Sin uso de STL, structs ni ANSI C: todas las estructuras de datos fueron implementadas de forma dinamica usando clases propias, arreglos y punteros en C++.
- Manejo de cadenas propio: modulo UtilidadesCadena para todas las operaciones de texto sin librerias de C ni std::string.
- Modelo de clases:
  - Foto: ruta, fecha, tamano en bytes y conteo de likes.
  - Album: gestiona su arreglo dinamico de fotos, configuracion de privacidad (Publico, Amigos, Privado) y carrusel circular.
  - Mensaje: texto (hasta 512 caracteres), fecha, remitente, destinatario, estado de lectura y busqueda por rango de fechas.
  - SolicitudAmistad: control de solicitudes con estados pendiente, aceptada o rechazada.
  - Usuario: administra arreglos dinamicos de amigos, albumes, mensajes (ordenados cronologicamente) y solicitudes. Incluye seleccion de 3 mejores amigos por ponderacion.
  - Programa: controladora principal que maneja la autenticacion, carga y guardado del archivo datos_udeabook.txt, y la matriz de adyacencia (bool **) para encontrar amigos en comun y sugerencias Top-K.
- Metricas de eficiencia: tras cada accion se reportan las iteraciones realizadas (directas e indirectas) y la memoria RAM consumida en bytes y KB.
