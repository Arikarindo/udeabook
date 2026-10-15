#ifndef USUARIO_H
#define USUARIO_H

#include "Album.h"
#include "Mensaje.h"
#include "SolicitudAmistad.h"
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
  int cantAmigos;
  int capAmigos;

  Album **albumes; // Arreglo dinámico de álbumes (Composición)
  int cantAlbumes;
  int capAlbumes;

  Mensaje **mensajes; // Arreglo dinámico de mensajes privados (Composición)
  int cantMensajes;
  int capMensajes;

  SolicitudAmistad *
      *solicitudes; // Arreglo de solicitudes recibidas (Composición)
  int cantSolicitudes;
  int capSolicitudes;

  // --- Helpers de Redimensionamiento Dinámico ---
  void redimensionarAmigos(int nuevaCapacidad);
  void redimensionarAlbumes(int nuevaCapacidad);
  void redimensionarMensajes(int nuevaCapacidad);
  void redimensionarSolicitudes(int nuevaCapacidad);
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
  int getCantAmigos() const;
  Usuario *getAmigoEn(int indice) const;
  int contarAmigosEnComun(const Usuario *otro) const;

  // --- Gestión de Álbumes ---
  void agregarAlbum(Album *nuevoAlbum);
  int getCantAlbumes() const;
  Album *getAlbumEn(int indice) const;
  Album *buscarAlbumPorNombre(const char *nombre) const;

  // --- Gestión de Mensajería ---
  void recibirMensaje(Mensaje *mensaje);
  int getCantMensajes() const;
  Mensaje *getMensajeEn(int indice) const;
  void enviarMensaje(Usuario *destinatario, const char *texto,
                     const char *fechaHora);

  // --- Gestión de Solicitudes de Amistad ---
  void recibirSolicitud(SolicitudAmistad *solicitud);
  int getCantSolicitudes() const;
  SolicitudAmistad *getSolicitudEn(int indice) const;
  bool tieneSolicitudPendienteCon(const Usuario *otro) const;
  void enviarSolicitudAmistad(Usuario *destinatario, const char *fechaHora);

  // --- 3 Mejores Amigos por Ponderación ---
  void seleccionar3MejoresAmigos(const char **rutasFotos,
                                 int &cantSeleccionadas) const;
};

#endif // USUARIO_H
