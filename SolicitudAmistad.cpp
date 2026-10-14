#include "SolicitudAmistad.h"
#include <iostream>

SolicitudAmistad::SolicitudAmistad(Usuario *emisor, Usuario *receptor,
                                   const char *fechaHora)
    : Notificacion(emisor, receptor, fechaHora), estado(PENDIENTE) {}

SolicitudAmistad::~SolicitudAmistad() {}

void SolicitudAmistad::aceptar() {
  estado = ACEPTADA;
  setProcesada(true);
}

void SolicitudAmistad::rechazar() {
  estado = RECHAZADA;
  setProcesada(true);
}

int SolicitudAmistad::getEstado() const { return estado; }

bool SolicitudAmistad::estaPendiente() const { return estado == PENDIENTE; }

bool SolicitudAmistad::estaAceptada() const { return estado == ACEPTADA; }

bool SolicitudAmistad::estaRechazada() const { return estado == RECHAZADA; }

void SolicitudAmistad::mostrarEnPantalla() const {
  const char *txtEstado = "Pendiente";
  if (estado == ACEPTADA) {
    txtEstado = "Aceptada";
  } else if (estado == RECHAZADA) {
    txtEstado = "Rechazada";
  }

  std::cout << "[" << fechaHora << "] Solicitud de Amistad (" << txtEstado
            << ")\n";
}

const char *SolicitudAmistad::getTipo() const { return "SolicitudAmistad"; }
