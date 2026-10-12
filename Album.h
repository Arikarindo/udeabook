#ifndef ALBUM_H
#define ALBUM_H
#include "Foto.h"
#include <cstddef>

class Usuario;
class Album {
protected:
  char *nombre;               // Nombre del álbum (memoria dinámica)
  char *rutaAlmacenamiento;   // Ruta base del almacenamiento (memoria dinámica)
  Foto **fotos;               // Arreglo dinámico de punteros a Foto (Foto* [])
  std::size_t cantFotos;      // Cantidad de fotos almacenadas actualmente
  std::size_t capFotos;       // Capacidad máxima asignada en el arreglo actual
  std::size_t indiceCarrusel; // Posición actual en el recorrido circular

  // Funciones auxiliares internas
  void redimensionar(std::size_t nuevaCapacidad);
  void liberarFotos();

  // Métodos auxiliares propios para manejo de cadenas en memoria dinámica
  static std::size_t longitudCadena(const char *cadena);
  static char *duplicarCadena(const char *origen);

public:
  // --- Constructores y Destructor Virtual ---
  Album(const char *nombreAlbum, const char *rutaBase);
  Album(const Album &otro); // Constructor de copia profunda
  virtual ~Album();         // OBLIGATORIO virtual en clase base polimórfica

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
  std::size_t getCantFotos() const;
  std::size_t getCapFotos() const;
  std::size_t getIndiceCarrusel() const;

  // --- Polimorfismo puro de Privacidad ---
  // Retorna una cadena constante descriptiva del tipo de privacidad
  virtual const char *getTipoPrivacidad() const = 0;

  // Método virtual puro con flags lógicos:
  // esDueno: true si quien intenta acceder es el propietario del álbum
  // esAmigo: true si quien intenta acceder es amigo del propietario
  virtual bool esAccesiblePor(bool esDueno, bool esAmigo) const = 0;

  // Sobrecarga de conveniencia que interactúa con la entidad Usuario (definida
  // en Nivel 4)
  virtual bool esAccesiblePorUsuario(const Usuario *solicitante,
                                     const Usuario *dueno) const;
};

/*
 * Subclase: AlbumPublico
 * Accesible por cualquier usuario registrado en la plataforma.
 */
class AlbumPublico : public Album {
public:
  AlbumPublico(const char *nombreAlbum, const char *rutaBase);
  virtual ~AlbumPublico() override;

  virtual const char *getTipoPrivacidad() const override;
  virtual bool esAccesiblePor(bool esDueno, bool esAmigo) const override;
};

/*
 * Subclase: AlbumPrivado
 * Accesible exclusivamente por el dueño del álbum.
 */
class AlbumPrivado : public Album {
public:
  AlbumPrivado(const char *nombreAlbum, const char *rutaBase);
  virtual ~AlbumPrivado() override;

  virtual const char *getTipoPrivacidad() const override;
  virtual bool esAccesiblePor(bool esDueno, bool esAmigo) const override;
};

/*
 * Subclase: AlbumAmigos
 * Accesible por el dueño y por sus amigos directos.
 */
class AlbumAmigos : public Album {
public:
  AlbumAmigos(const char *nombreAlbum, const char *rutaBase);
  virtual ~AlbumAmigos() override;

  virtual const char *getTipoPrivacidad() const override;
  virtual bool esAccesiblePor(bool esDueno, bool esAmigo) const override;
};

#endif // ALBUM_H
