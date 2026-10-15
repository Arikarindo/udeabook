#ifndef USUARIO_H
#define USUARIO_H

#include "Album.h"
#include "Mensaje.h"
#include "SolicitudAmistad.h"
#include <cstddef>
#include <iostream>

class Usuario {
private:
  // --- Atributos de Identificación y Perfil ---
  char *username;
  char *password;
  char *fechaNacimiento;
  char *correo;
  char *rutaFotoPerfil;
  char *ciudad;
  char *pais;
  char *fechaInscripcion;

  // --- Colecciones Dinámicas Propias ---
  Usuario **amigos; // Arreglo dinámico de amigos (Asociación)
  std::size_t cantAmigos;
  std::size_t capAmigos;

  Album **albumes; // Arreglo dinámico de álbumes (Composición)
  std::size_t cantAlbumes;
  std::size_t capAlbumes;

  Mensaje **mensajes; // Arreglo dinámico de mensajes privados (Composición)
  std::size_t cantMensajes;
  std::size_t capMensajes;

  SolicitudAmistad *
      *solicitudes; // Arreglo de solicitudes recibidas (Composición)
  std::size_t cantSolicitudes;
  std::size_t capSolicitudes;

  // --- Helpers de Redimensionamiento Dinámico ---
  void redimensionarAmigos(std::size_t nuevaCapacidad);
  void redimensionarAlbumes(std::size_t nuevaCapacidad);
  void redimensionarMensajes(std::size_t nuevaCapacidad);
  void redimensionarSolicitudes(std::size_t nuevaCapacidad);
  void liberarMemoria();

public:
  // --- Constructores y Destructor ---
  Usuario(const char *username, const char *password,
          const char *fechaNacimiento, const char *correo,
          const char *rutaFotoPerfil, const char *ciudad, const char *pais,
          const char *fechaInscripcion);
  Usuario(const Usuario &otro);
  ~Usuario();

  // --- Sobrecarga de Operadores ---
  Usuario &operator=(const Usuario &otro);
  bool operator==(const Usuario &otro) const;
  friend std::ostream &operator<<(std::ostream &os, const Usuario &usuario);

  // --- Getters de Perfil ---
  const char *getUsername() const;
  const char *getPassword() const;
  const char *getFechaNacimiento() const;
  const char *getCorreo() const;
  const char *getRutaFotoPerfil() const;
  const char *getCiudad() const;
  const char *getPais() const;
  const char *getFechaInscripcion() const;

  // --- Gestión de Amigos ---
  void agregarAmigo(Usuario *nuevoAmigo);
  void establecerAmistadMutua(Usuario *otro);
  bool esAmigoDe(const Usuario *otro) const;
  std::size_t getCantAmigos() const;
  Usuario *getAmigoEn(std::size_t indice) const;
  std::size_t contarAmigosEnComun(const Usuario *otro) const;

  // --- Gestión de Álbumes ---
  void agregarAlbum(Album *nuevoAlbum);
  std::size_t getCantAlbumes() const;
  Album *getAlbumEn(std::size_t indice) const;
  Album *buscarAlbumPorNombre(const char *nombre) const;

  // --- Gestión de Mensajería ---
  void recibirMensaje(Mensaje *mensaje);
  std::size_t getCantMensajes() const;
  Mensaje *getMensajeEn(std::size_t indice) const;
  void enviarMensaje(Usuario *destinatario, const char *texto,
                     const char *fechaHora);

  // --- Gestión de Solicitudes de Amistad ---
  void recibirSolicitud(SolicitudAmistad *solicitud);
  std::size_t getCantSolicitudes() const;
  SolicitudAmistad *getSolicitudEn(std::size_t indice) const;
  bool tieneSolicitudPendienteCon(const Usuario *otro) const;
  void enviarSolicitudAmistad(Usuario *destinatario, const char *fechaHora);

  // --- Requisito II: 3 Mejores Amigos por Ponderación ---
  void seleccionar3MejoresAmigos(const char **rutasFotos,
                                 std::size_t &cantSeleccionadas) const;
};

#endif // USUARIO_H
