#ifndef SOLICITUD_AMISTAD_H
#define SOLICITUD_AMISTAD_H

#include <cstddef>

class Usuario;

class SolicitudAmistad {
public:
  enum Estado { PENDIENTE = 0, ACEPTADA = 1, RECHAZADA = 2 };

private:
  Usuario *emisor;
  Usuario *receptor;
  char *fechaHora;
  int estado;

public:
  // --- Constructores y Destructor ---
  SolicitudAmistad(Usuario *emisor, Usuario *receptor, const char *fechaHora);
  SolicitudAmistad(const SolicitudAmistad &otra);
  ~SolicitudAmistad();

  // --- Operador de Asignación Profunda ---
  SolicitudAmistad &operator=(const SolicitudAmistad &otra);

  // --- Operaciones de Negocio ---
  void aceptar();
  void rechazar();

  // --- Consultas ---
  int getEstado() const;
  bool estaPendiente() const;
  bool estaAceptada() const;
  bool estaRechazada() const;
  const char *getFechaHora() const;
  Usuario *getEmisor() const;
  Usuario *getReceptor() const;

  // Visualización
  void mostrarEnPantalla() const;
};

#endif // SOLICITUD_AMISTAD_H
