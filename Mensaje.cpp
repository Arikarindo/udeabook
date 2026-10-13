#include "Mensaje.h"
#include "UtilidadesCadena.h"
#include <iostream>

Mensaje::Mensaje(Usuario *emisor, Usuario *receptor, const char *fechaHora,
                 const char *textoMensaje)
    : Notificacion(emisor, receptor, fechaHora), texto(nullptr), leido(false) {
  // Limita el mensaje a un máximo de 512 caracteres
  const std::size_t len = UtilidadesCadena::longitud(textoMensaje);
  const std::size_t maxLen = (len > 512) ? 512 : len;

  texto = new char[maxLen + 1];
  for (std::size_t i = 0; i < maxLen; ++i) {
    texto[i] = textoMensaje[i];
  }
  texto[maxLen] = '\0';
}

Mensaje::Mensaje(const Mensaje &otro)
    : Notificacion(otro), texto(UtilidadesCadena::duplicar(otro.texto)),
      leido(otro.leido) {}

Mensaje::~Mensaje() { delete[] texto; }

Mensaje &Mensaje::operator=(const Mensaje &otro) {
  if (this != &otro) {
    Notificacion::operator=(otro);

    char *nuevoTexto = UtilidadesCadena::duplicar(otro.texto);
    delete[] texto;
    texto = nuevoTexto;
    leido = otro.leido;
  }
  return *this;
}

void Mensaje::marcarComoLeido() {
  leido = true;
  setProcesada(true);
}

bool Mensaje::estaLeido() const { return leido; }

const char *Mensaje::getTexto() const { return texto; }

bool Mensaje::estaEnRango(const char *fechaInicio, const char *fechaFin) const {
  if (fechaInicio == nullptr || fechaFin == nullptr || fechaHora == nullptr) {
    return false;
  }

  // Extrae la porción de fecha "AAAA-MM-DD" (primeros 10 caracteres)
  char fechaActual[11];
  for (std::size_t i = 0; i < 10; ++i) {
    fechaActual[i] = (fechaHora[i] != '\0') ? fechaHora[i] : ' ';
  }
  fechaActual[10] = '\0';

  // Comparación inclusiva: fechaInicio <= fechaActual <= fechaFin
  const bool despuesDeInicio =
      (UtilidadesCadena::comparar(fechaActual, fechaInicio) >= 0);
  const bool antesDeFin =
      (UtilidadesCadena::comparar(fechaActual, fechaFin) <= 0);

  return despuesDeInicio && antesDeFin;
}

void Mensaje::mostrarEnPantalla() const {
  std::cout << "[" << fechaHora << "] " << (leido ? "(Leido) " : "(Nuevo) ")
            << ": " << texto << '\n';
}

const char *Mensaje::getTipo() const { return "Mensaje"; }
