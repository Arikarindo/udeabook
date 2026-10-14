#ifndef SOLICITUD_AMISTAD_H
#define SOLICITUD_AMISTAD_H

#include "Notificacion.h"

class SolicitudAmistad : public Notificacion {
public:
  enum Estado { PENDIENTE = 0, ACEPTADA = 1, RECHAZADA = 2 };

private:
  int estado; // Estado actual de la solicitud

public:
  // --- Constructores y Destructor ---
  SolicitudAmistad(Usuario *emisor, Usuario *receptor, const char *fechaHora);
  virtual ~SolicitudAmistad() override;

  // --- Operaciones de Negocio ---
  void aceptar();
  void rechazar();

  // --- Consultas ---
  int getEstado() const;
  bool estaPendiente() const;
  bool estaAceptada() const;
  bool estaRechazada() const;

  // --- Implementación de Métodos Virtuales ---
  virtual void mostrarEnPantalla() const override;
  virtual const char *getTipo() const override;
};

#endif // SOLICITUD_AMISTAD_H
