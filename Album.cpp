#include "Album.h"
#include "Usuario.h"

// ---------------------------------------------------------------------------
// Métodos auxiliares para manejo propio de cadenas
// ---------------------------------------------------------------------------

std::size_t Album::longitudCadena(const char *cadena) {
  if (cadena == nullptr) {
    return 0;
  }
  std::size_t len = 0;
  while (cadena[len] != '\0') {
    ++len;
  }
  return len;
}

char *Album::duplicarCadena(const char *origen) {
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

void Album::liberarFotos() {
  if (fotos != nullptr) {
    // Al ser una relación de COMPOSICIÓN, el Álbum es dueño de sus fotos:
    // destruye cada objeto Foto individual
    for (std::size_t i = 0; i < cantFotos; ++i) {
      delete fotos[i];
    }
    // Destruye el arreglo de punteros
    delete[] fotos;
    fotos = nullptr;
  }
  cantFotos = 0;      // cantidad de fotos
  capFotos = 0;       // capacidad maxima del arreglo
  indiceCarrusel = 0; // indice actual del carrusel
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

// ---------------------------------------------------------------------------
// Constructores y Destructor de Album
// ---------------------------------------------------------------------------

Album::Album(const char *nombreAlbum, const char *rutaBase)
    : nombre(duplicarCadena(nombreAlbum)),
      rutaAlmacenamiento(duplicarCadena(rutaBase)), fotos(nullptr),
      cantFotos(0), capFotos(0), indiceCarrusel(0) {}

Album::Album(const Album &otro)
    : nombre(duplicarCadena(otro.nombre)),
      rutaAlmacenamiento(duplicarCadena(otro.rutaAlmacenamiento)),
      fotos(nullptr), cantFotos(0), capFotos(0),
      indiceCarrusel(otro.indiceCarrusel) {
  if (otro.cantFotos > 0) {
    capFotos = otro.cantFotos;
    fotos = new Foto *[capFotos];
    for (std::size_t i = 0; i < otro.cantFotos; ++i) {
      // Copia profunda de cada foto individual
      fotos[i] = new Foto(*otro.fotos[i]);
      ++cantFotos;
    }
  }
}

Album::~Album() {
  delete[] nombre;
  delete[] rutaAlmacenamiento;
  liberarFotos();
}

Album &Album::operator=(const Album &otro) {
  if (this != &otro) {
    // 1. Duplica primero cadenas
    char *nuevoNombre = duplicarCadena(otro.nombre);
    char *nuevaRuta = duplicarCadena(otro.rutaAlmacenamiento);

    // 2. Libera recursos previos
    delete[] nombre;
    delete[] rutaAlmacenamiento;
    liberarFotos();

    // 3. Asigna nuevos valores
    nombre = nuevoNombre;
    rutaAlmacenamiento = nuevaRuta;
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

// ---------------------------------------------------------------------------
// Gestión de Fotos
// ---------------------------------------------------------------------------

void Album::agregarFoto(Foto *foto) {
  if (foto == nullptr) {
    return;
  }
  // Crecimiento dinámico exponencial (factor x2)
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

// ---------------------------------------------------------------------------
// Carrusel Circular
// ---------------------------------------------------------------------------

Foto *Album::siguienteFoto() {
  if (cantFotos == 0) {
    return nullptr;
  }
  // Aritmética modular para avance cíclico
  indiceCarrusel = (indiceCarrusel + 1) % cantFotos;
  return fotos[indiceCarrusel];
}

Foto *Album::previaFoto() {
  if (cantFotos == 0) {
    return nullptr;
  }
  // Aritmética modular para retroceso cíclico seguro
  indiceCarrusel = (indiceCarrusel - 1 + cantFotos) % cantFotos;
  return fotos[indiceCarrusel];
}

void Album::reiniciarCarrusel() { indiceCarrusel = 0; }

// ---------------------------------------------------------------------------
// Getters
// ---------------------------------------------------------------------------

const char *Album::getNombre() const { return nombre; }

const char *Album::getRutaAlmacenamiento() const { return rutaAlmacenamiento; }

std::size_t Album::getCantFotos() const { return cantFotos; }

std::size_t Album::getCapFotos() const { return capFotos; }

std::size_t Album::getIndiceCarrusel() const { return indiceCarrusel; }

bool Album::esAccesiblePorUsuario(const Usuario *solicitante,
                                  const Usuario *dueno) const {
  const bool esDueno = (solicitante != nullptr && solicitante == dueno);
  const bool esAmigo = (dueno != nullptr && solicitante != nullptr &&
                        dueno->esAmigoDe(solicitante));
  return esAccesiblePor(esDueno, esAmigo);
}

// ===========================================================================
// Implementación: AlbumPublico
// ===========================================================================

AlbumPublico::AlbumPublico(const char *nombreAlbum, const char *rutaBase)
    : Album(nombreAlbum, rutaBase) {}

AlbumPublico::~AlbumPublico() {}

const char *AlbumPublico::getTipoPrivacidad() const { return "Publico"; }

bool AlbumPublico::esAccesiblePor(bool /*esDueno*/, bool /*esAmigo*/) const {
  // Un álbum público siempre es visible por cualquier usuario
  return true;
}

// ===========================================================================
// Implementación: AlbumPrivado
// ===========================================================================

AlbumPrivado::AlbumPrivado(const char *nombreAlbum, const char *rutaBase)
    : Album(nombreAlbum, rutaBase) {}

AlbumPrivado::~AlbumPrivado() {}

const char *AlbumPrivado::getTipoPrivacidad() const { return "Privado"; }

bool AlbumPrivado::esAccesiblePor(bool esDueno, bool /*esAmigo*/) const {
  // Un álbum privado solo es visible por el dueño
  return esDueno;
}

// ===========================================================================
// Implementación: AlbumAmigos
// ===========================================================================

AlbumAmigos::AlbumAmigos(const char *nombreAlbum, const char *rutaBase)
    : Album(nombreAlbum, rutaBase) {}

AlbumAmigos::~AlbumAmigos() {}

const char *AlbumAmigos::getTipoPrivacidad() const { return "Amigos"; }

bool AlbumAmigos::esAccesiblePor(bool esDueno, bool esAmigo) const {
  // Visible si es el dueño o si es un amigo directo
  return esDueno || esAmigo;
}
