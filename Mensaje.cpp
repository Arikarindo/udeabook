#include "Mensaje.h"
#include "UtilidadesCadena.h"
#include <iostream>

Mensaje::Mensaje(Usuario *emisor, Usuario *receptor, const char *fechaHora,
                 const char *textoMensaje)
    : emisor(emisor), receptor(receptor),
      fechaHora(UtilidadesCadena::duplicar(fechaHora)), texto(nullptr),
      leido(false) {
  const std::size_t len = UtilidadesCadena::longitud(textoMensaje);
  const std::size_t maxLen = (len > 512) ? 512 : len;

  texto = new char[maxLen + 1];
  for (std::size_t i = 0; i < maxLen; ++i) {
    texto[i] = textoMensaje[i];
  }
  texto[maxLen] = '\0';
}

Mensaje::Mensaje(const Mensaje &otro)
    : emisor(otro.emisor), receptor(otro.receptor),
      fechaHora(UtilidadesCadena::duplicar(otro.fechaHora)),
      texto(UtilidadesCadena::duplicar(otro.texto)), leido(otro.leido) {}

Mensaje::~Mensaje() {
  delete[] fechaHora;
  delete[] texto;
}

Mensaje &Mensaje::operator=(const Mensaje &otro) {
  if (this != &otro) {
    char *nuevaFecha = UtilidadesCadena::duplicar(otro.fechaHora);
    char *nuevoTexto = UtilidadesCadena::duplicar(otro.texto);

    delete[] fechaHora;
    delete[] texto;

    fechaHora = nuevaFecha;
    texto = nuevoTexto;
    emisor = otro.emisor;
    receptor = otro.receptor;
    leido = otro.leido;
  }
  return *this;
}

void Mensaje::marcarComoLeido() { leido = true; }

bool Mensaje::estaLeido() const { return leido; }

const char *Mensaje::getTexto() const { return texto; }

const char *Mensaje::getFechaHora() const { return fechaHora; }

Usuario *Mensaje::getEmisor() const { return emisor; }

Usuario *Mensaje::getReceptor() const { return receptor; }

bool Mensaje::estaEnRango(const char *fechaInicio, const char *fechaFin) const {
  if (fechaInicio == nullptr || fechaFin == nullptr || fechaHora == nullptr) {
    return false;
  }

  char fechaActual[11];
  for (std::size_t i = 0; i < 10; ++i) {
    fechaActual[i] = (fechaHora[i] != '\0') ? fechaHora[i] : ' ';
  }
  fechaActual[10] = '\0';

  const bool despuesDeInicio =
      (UtilidadesCadena::comparar(fechaActual, fechaInicio) >= 0);
  const bool antesDeFin =
      (UtilidadesCadena::comparar(fechaActual, fechaFin) <= 0);

  return despuesDeInicio && antesDeFin;
}

void Mensaje::mostrarEnPantalla() const {
  std::cout << "[" << (fechaHora ? fechaHora : "") << "] "
            << (leido ? "(Leido) " : "(Nuevo) ") << ": "
            << (texto ? texto : "") << '\n';
}
