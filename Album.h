#ifndef ALBUM_H
#define ALBUM_H

#include "Foto.h"
#include <cstddef> // Para std::size_t

class Usuario;

class Album {
private:
  char *nombre;               // Nombre del álbum (memoria dinámica)
  char *rutaAlmacenamiento;   // Ruta base del almacenamiento (memoria dinámica)
  char *tipoPrivacidad;       // "Publico", "Privado", "Amigos" (memoria dinámica)
  Foto **fotos;               // Arreglo dinámico de punteros a Foto (Foto* [])
  std::size_t cantFotos;      // Cantidad de fotos almacenadas actualmente
  std::size_t capFotos;       // Capacidad máxima asignada en el arreglo actual
  std::size_t indiceCarrusel; // Posición actual en el recorrido circular

  // Funciones auxiliares internas
  void redimensionar(std::size_t nuevaCapacidad);
  void liberarFotos();

public:
  // --- Constructores y Destructor ---
  Album(const char *nombreAlbum, const char *rutaBase,
        const char *privacidad = "Publico");
  Album(const Album &otro); // Constructor de copia profunda
  ~Album();

  // --- Operador de asignación profunda ---
  Album &operator=(const Album &otro);

  // --- Gestión de Fotos (Composición) ---
  void agregarFoto(Foto *foto);
  Foto *getFotoEn(std::size_t indice) const;
  Foto *getFotoActual() const;

  // --- Carrusel Circular ---
  Foto *siguienteFoto(); // Avanza y retorna la foto actual (cíclico)
  Foto *previaFoto();    // Retrocede y retorna la foto actual (cíclico)
  void reiniciarCarrusel();

  // --- Consultas (Getters const) ---
  const char *getNombre() const;
  const char *getRutaAlmacenamiento() const;
  const char *getTipoPrivacidad() const;
  std::size_t getCantFotos() const;
  std::size_t getCapFotos() const;
  std::size_t getIndiceCarrusel() const;

  // --- Control de Acceso y Privacidad (Sin Herencia) ---
  void setTipoPrivacidad(const char *nuevaPrivacidad);
  bool esAccesiblePor(bool esDueno, bool esAmigo) const;
  bool esAccesiblePorUsuario(const Usuario *solicitante,
                             const Usuario *dueno) const;
};

#endif // ALBUM_H
