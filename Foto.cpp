// Foto.cpp — IMPLEMENTACIÓN de la clase Foto
#include "Foto.h"
#include <iostream>

// ---------------------------------------------------------------------------
// Métodos auxiliares para manejo propio de cadenas
// ---------------------------------------------------------------------------

std::size_t Foto::longitudCadena(const char *cadena) {
  if (cadena == nullptr) {
    return 0;
  }
  std::size_t len = 0;
  while (cadena[len] != '\0') {
    ++len;
  }
  return len;
}

char *Foto::duplicarCadena(const char *origen) {
  if (origen == nullptr) {
    char *vacio = new char[1];
    vacio[0] = '\0';
    return vacio;
  }
  std::size_t len = longitudCadena(origen);
  char *destino = new char[len + 1];
  for (std::size_t i = 0; i < len; ++i) {
    destino[i] = origen[i];
  }
  destino[len] = '\0';
  return destino;
}

// ---------------------------------------------------------------------------
// Constructores y Destructor
// ---------------------------------------------------------------------------

Foto::Foto()
    : rutaArchivo(duplicarCadena(nullptr)),
      fechaCaptura(duplicarCadena(nullptr)),
      tamanoBytes(0),
      conteoLikes(0) {}

Foto::Foto(const char *ruta, const char *fecha, std::size_t tamano)
    : rutaArchivo(duplicarCadena(ruta)),
      fechaCaptura(duplicarCadena(fecha)),
      tamanoBytes(tamano),
      conteoLikes(0) {}

Foto::Foto(const Foto &otra)
    : rutaArchivo(duplicarCadena(otra.rutaArchivo)),
      fechaCaptura(duplicarCadena(otra.fechaCaptura)),
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
    char *nuevaRuta = duplicarCadena(otra.rutaArchivo);
    char *nuevaFecha = duplicarCadena(otra.fechaCaptura);

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
