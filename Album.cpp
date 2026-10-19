#include "Album.h"
#include "Usuario.h"
#include "UtilidadesCadena.h"

void Album::liberarFotos() {
  if (fotos != nullptr) {
    for (std::size_t i = 0; i < cantFotos; ++i) {
      delete fotos[i];
    }
    delete[] fotos;
    fotos = nullptr;
  }
  cantFotos = 0;
  capFotos = 0;
  indiceCarrusel = 0;
}

void Album::redimensionar(std::size_t nuevaCapacidad) {
  Foto **nuevoArreglo = new Foto *[nuevaCapacidad];
  for (std::size_t i = 0; i < cantFotos; ++i) {
    nuevoArreglo[i] = fotos[i];
  }
  delete[] fotos;
  fotos = nuevoArreglo;
  capFotos = nuevaCapacidad;
}

Album::Album(const char *nombreAlbum, const char *rutaBase,
             const char *privacidad)
    : nombre(UtilidadesCadena::duplicar(nombreAlbum)),
      rutaAlmacenamiento(UtilidadesCadena::duplicar(rutaBase)),
      tipoPrivacidad(UtilidadesCadena::duplicar(privacidad ? privacidad : "Publico")),
      fotos(nullptr), cantFotos(0), capFotos(0), indiceCarrusel(0) {}

Album::Album(const Album &otro)
    : nombre(UtilidadesCadena::duplicar(otro.nombre)),
      rutaAlmacenamiento(UtilidadesCadena::duplicar(otro.rutaAlmacenamiento)),
      tipoPrivacidad(UtilidadesCadena::duplicar(otro.tipoPrivacidad)),
      fotos(nullptr), cantFotos(0), capFotos(0),
      indiceCarrusel(otro.indiceCarrusel) {
  if (otro.cantFotos > 0) {
    capFotos = otro.cantFotos;
    fotos = new Foto *[capFotos];
    for (std::size_t i = 0; i < otro.cantFotos; ++i) {
      fotos[i] = new Foto(*otro.fotos[i]);
      ++cantFotos;
    }
  }
}

Album::~Album() {
  delete[] nombre;
  delete[] rutaAlmacenamiento;
  delete[] tipoPrivacidad;
  liberarFotos();
}

Album &Album::operator=(const Album &otro) {
  if (this != &otro) {
    char *nuevoNombre = UtilidadesCadena::duplicar(otro.nombre);
    char *nuevaRuta = UtilidadesCadena::duplicar(otro.rutaAlmacenamiento);
    char *nuevaPriv = UtilidadesCadena::duplicar(otro.tipoPrivacidad);

    delete[] nombre;
    delete[] rutaAlmacenamiento;
    delete[] tipoPrivacidad;
    liberarFotos();

    nombre = nuevoNombre;
    rutaAlmacenamiento = nuevaRuta;
    tipoPrivacidad = nuevaPriv;
    indiceCarrusel = otro.indiceCarrusel;

    if (otro.cantFotos > 0) {
      capFotos = otro.cantFotos;
      fotos = new Foto *[capFotos];
      for (std::size_t i = 0; i < otro.cantFotos; ++i) {
        fotos[i] = new Foto(*otro.fotos[i]);
        ++cantFotos;
      }
    }
  }
  return *this;
}

void Album::agregarFoto(Foto *foto) {
  if (foto == nullptr) {
    return;
  }
  if (cantFotos == capFotos) {
    const std::size_t nuevaCap = (capFotos == 0) ? 2 : capFotos * 2;
    redimensionar(nuevaCap);
  }
  fotos[cantFotos] = foto;
  ++cantFotos;
}

Foto *Album::getFotoEn(std::size_t indice) const {
  if (indice < cantFotos) {
    return fotos[indice];
  }
  return nullptr;
}

Foto *Album::getFotoActual() const {
  if (cantFotos == 0) {
    return nullptr;
  }
  return fotos[indiceCarrusel];
}

Foto *Album::siguienteFoto() {
  if (cantFotos == 0) {
    return nullptr;
  }
  indiceCarrusel = (indiceCarrusel + 1) % cantFotos;
  return fotos[indiceCarrusel];
}

Foto *Album::previaFoto() {
  if (cantFotos == 0) {
    return nullptr;
  }
  indiceCarrusel = (indiceCarrusel - 1 + cantFotos) % cantFotos;
  return fotos[indiceCarrusel];
}

void Album::reiniciarCarrusel() { indiceCarrusel = 0; }

const char *Album::getNombre() const { return nombre; }

const char *Album::getRutaAlmacenamiento() const { return rutaAlmacenamiento; }

const char *Album::getTipoPrivacidad() const { return tipoPrivacidad; }

std::size_t Album::getCantFotos() const { return cantFotos; }

std::size_t Album::getCapFotos() const { return capFotos; }

std::size_t Album::getIndiceCarrusel() const { return indiceCarrusel; }

void Album::setTipoPrivacidad(const char *nuevaPrivacidad) {
  if (nuevaPrivacidad != nullptr) {
    char *nuevo = UtilidadesCadena::duplicar(nuevaPrivacidad);
    delete[] tipoPrivacidad;
    tipoPrivacidad = nuevo;
  }
}

bool Album::esAccesiblePor(bool esDueno, bool esAmigo) const {
  if (tipoPrivacidad == nullptr ||
      UtilidadesCadena::sonIguales(tipoPrivacidad, "Publico")) {
    return true;
  }
  if (UtilidadesCadena::sonIguales(tipoPrivacidad, "Privado")) {
    return esDueno;
  }
  if (UtilidadesCadena::sonIguales(tipoPrivacidad, "Amigos")) {
    return esDueno || esAmigo;
  }
  return true;
}

bool Album::esAccesiblePorUsuario(const Usuario *solicitante,
                                  const Usuario *dueno) const {
  const bool esDueno = (solicitante != nullptr && solicitante == dueno);
  const bool esAmigo = (dueno != nullptr && solicitante != nullptr &&
                        dueno->esAmigoDe(solicitante));
  return esAccesiblePor(esDueno, esAmigo);
}
