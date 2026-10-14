# UdeABook

Proyecto del Desafio II para la materia Informatica II (2026-2).
Consiste en el desarrollo de una red social por consola en C++ aplicando programacion orientada a objetos.

Requisitos principales:
- Implementacion sin usar la libreria estandar (STL) ni structs.
- Manejo manual de memoria dinamica con punteros y arreglos dinamicos.
- Funciones propias para el manejo de cadenas.
- Medicion de eficiencia en iteraciones y memoria RAM consumida.

Avance del proyecto:
- UtilidadesCadena: funciones basicas para copiar, comparar, medir longitud y concatenar texto en memoria dinamica.
- Foto: entidad para representar fotos con fecha, ruta, tamano y conteo de likes.
- Album: clase base y clases derivadas (AlbumPublico, AlbumAmigos, AlbumPrivado) con navegacion circular tipo carrusel.
- Mensaje: mensajes entre usuarios con fecha y texto, con filtrado por rango de fechas.
- SolicitudAmistad: gestion de solicitudes de amistad con estados (pendiente, aceptada, rechazada).

Proximos pasos:
- Clase Usuario con arreglos dinamicos para amigos, albumes, mensajes y solicitudes.
- Clase Programa para coordinar la persistencia en archivo y la matriz de adyacencia.