#include "Notificacion.h"
#include "UtilidadesCadena.h"

Notificacion::Notificacion(Usuario *emisor, Usuario *receptor,
                           const char *fechaHora)
    : emisor(emisor), receptor(receptor),
      fechaHora(UtilidadesCadena::duplicar(fechaHora)), procesada(false) {}

Notificacion::Notificacion(const Notificacion &otra)
    : emisor(otra.emisor), receptor(otra.receptor),
      fechaHora(UtilidadesCadena::duplicar(otra.fechaHora)),
      procesada(otra.procesada) {}

Notificacion::~Notificacion() { delete[] fechaHora; }

Notificacion &Notificacion::operator=(const Notificacion &otra) {
  if (this != &otra) {
    char *nuevaFecha = UtilidadesCadena::duplicar(otra.fechaHora);
    delete[] fechaHora;
    fechaHora = nuevaFecha;

    emisor = otra.emisor;
    receptor = otra.receptor;
    procesada = otra.procesada;
  }
  return *this;
}

Usuario *Notificacion::getEmisor() const { return emisor; }

Usuario *Notificacion::getReceptor() const { return receptor; }

const char *Notificacion::getFechaHora() const { return fechaHora; }

bool Notificacion::estaProcesada() const { return procesada; }

void Notificacion::setProcesada(bool estado) { procesada = estado; }
