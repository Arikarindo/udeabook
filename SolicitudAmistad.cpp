#include "SolicitudAmistad.h"
#include "UtilidadesCadena.h"
#include <iostream>

SolicitudAmistad::SolicitudAmistad(Usuario *emisor, Usuario *receptor,
                                   const char *fechaHora)
    : emisor(emisor), receptor(receptor),
      fechaHora(UtilidadesCadena::duplicar(fechaHora)), estado(PENDIENTE) {}

SolicitudAmistad::SolicitudAmistad(const SolicitudAmistad &otra)
    : emisor(otra.emisor), receptor(otra.receptor),
      fechaHora(UtilidadesCadena::duplicar(otra.fechaHora)),
      estado(otra.estado) {}

SolicitudAmistad::~SolicitudAmistad() { delete[] fechaHora; }

SolicitudAmistad &SolicitudAmistad::operator=(const SolicitudAmistad &otra) {
  if (this != &otra) {
    char *nuevaFecha = UtilidadesCadena::duplicar(otra.fechaHora);
    delete[] fechaHora;
    fechaHora = nuevaFecha;
    emisor = otra.emisor;
    receptor = otra.receptor;
    estado = otra.estado;
  }
  return *this;
}

void SolicitudAmistad::aceptar() { estado = ACEPTADA; }

void SolicitudAmistad::rechazar() { estado = RECHAZADA; }

int SolicitudAmistad::getEstado() const { return estado; }

bool SolicitudAmistad::estaPendiente() const { return estado == PENDIENTE; }

bool SolicitudAmistad::estaAceptada() const { return estado == ACEPTADA; }

bool SolicitudAmistad::estaRechazada() const { return estado == RECHAZADA; }

const char *SolicitudAmistad::getFechaHora() const { return fechaHora; }

Usuario *SolicitudAmistad::getEmisor() const { return emisor; }

Usuario *SolicitudAmistad::getReceptor() const { return receptor; }

void SolicitudAmistad::mostrarEnPantalla() const {
  const char *txtEstado = "Pendiente";
  if (estado == ACEPTADA) {
    txtEstado = "Aceptada";
  } else if (estado == RECHAZADA) {
    txtEstado = "Rechazada";
  }

  std::cout << "[" << (fechaHora ? fechaHora : "")
            << "] Solicitud de Amistad (" << txtEstado << ")\n";
}
