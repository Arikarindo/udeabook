#ifndef ALBUM_H
#define ALBUM_H

#include "Foto.h"

class Usuario;

class Album {
private:
  char *nombre;             // Nombre del álbum (memoria dinámica)
  char *rutaAlmacenamiento; // Ruta base del almacenamiento (memoria dinámica)
  char *tipoPrivacidad;     // "Publico", "Privado", "Amigos" (memoria dinámica)
  Foto **fotos;             // Arreglo dinámico de punteros a Foto (Foto* [])
  int cantFotos;            // Cantidad de fotos almacenadas actualmente
  int capFotos;             // Capacidad máxima asignada en el arreglo actual
  int indiceCarrusel;       // Posición actual en el recorrido circular

  // Funciones auxiliares internas
  void redimensionar(int nuevaCapacidad);
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
  Foto *getFotoEn(int indice) const;
  Foto *getFotoActual() const;

  // --- Carrusel Circular ---
  Foto *siguienteFoto(); // Avanza y retorna la foto actual (cíclico)
  Foto *previaFoto();    // Retrocede y retorna la foto actual (cíclico)
  void reiniciarCarrusel();

  // --- Consultas (Getters const) ---
  const char *getNombre() const;
  const char *getRutaAlmacenamiento() const;
  const char *getTipoPrivacidad() const;
  int getCantFotos() const;
  int getCapFotos() const;
  int getIndiceCarrusel() const;

  // --- Control de Acceso y Privacidad (Sin Herencia) ---
  void setTipoPrivacidad(const char *nuevaPrivacidad);
  bool esAccesiblePor(bool esDueno, bool esAmigo) const;
  bool esAccesiblePorUsuario(const Usuario *solicitante,
                             const Usuario *dueno) const;
};

#endif // ALBUM_H
