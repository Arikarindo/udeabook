// Foto.cpp — IMPLEMENTACIÓN de la clase Foto
#include "Foto.h"
#include "UtilidadesCadena.h"
#include <iostream>

// ---------------------------------------------------------------------------
// Constructores y Destructor
// ---------------------------------------------------------------------------

Foto::Foto()
    : rutaArchivo(UtilidadesCadena::duplicar(nullptr)),
      fechaCaptura(UtilidadesCadena::duplicar(nullptr)),
      tamanoBytes(0),
      conteoLikes(0) {}

Foto::Foto(const char *ruta, const char *fecha, std::size_t tamano)
    : rutaArchivo(UtilidadesCadena::duplicar(ruta)),
      fechaCaptura(UtilidadesCadena::duplicar(fecha)),
      tamanoBytes(tamano),
      conteoLikes(0) {}

Foto::Foto(const Foto &otra)
    : rutaArchivo(UtilidadesCadena::duplicar(otra.rutaArchivo)),
      fechaCaptura(UtilidadesCadena::duplicar(otra.fechaCaptura)),
      tamanoBytes(otra.tamanoBytes),
      conteoLikes(otra.conteoLikes) {}

Foto::~Foto() {
  delete[] rutaArchivo;
  delete[] fechaCaptura;
}

// ---------------------------------------------------------------------------
// Sobrecarga de Operadores
// ---------------------------------------------------------------------------

Foto &Foto::operator=(const Foto &otra) {
  if (this != &otra) {
    char *nuevaRuta = UtilidadesCadena::duplicar(otra.rutaArchivo);
    char *nuevaFecha = UtilidadesCadena::duplicar(otra.fechaCaptura);

    delete[] rutaArchivo;
    delete[] fechaCaptura;

    rutaArchivo = nuevaRuta;
    fechaCaptura = nuevaFecha;
    tamanoBytes = otra.tamanoBytes;
    conteoLikes = otra.conteoLikes;
  }
  return *this;
}

// ---------------------------------------------------------------------------
// Modificadores
// ---------------------------------------------------------------------------

void Foto::darLike() { ++conteoLikes; }

void Foto::quitarLike() {
  if (conteoLikes > 0) {
    --conteoLikes;
  }
}

// ---------------------------------------------------------------------------
// Consultas (Getters const)
// ---------------------------------------------------------------------------

int Foto::getLikes() const { return conteoLikes; }

const char *Foto::getRuta() const { return rutaArchivo; }

const char *Foto::getFechaCaptura() const { return fechaCaptura; }

std::size_t Foto::getTamanoBytes() const { return tamanoBytes; }

// ---------------------------------------------------------------------------
// Visualización
// ---------------------------------------------------------------------------

void Foto::mostrarRuta() const { std::cout << rutaArchivo << '\n'; }
