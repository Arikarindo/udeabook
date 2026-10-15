// Usuario.cpp — Implementación de la Entidad Central (Sin Herencia)

#include "Usuario.h"
#include "Album.h"
#include "Mensaje.h"
#include "SolicitudAmistad.h"
#include "UtilidadesCadena.h"
#include <iostream>
#include <random>

// Redimensionamiento Dinámico y Memoria

void Usuario::redimensionarAmigos(int nuevaCapacidad) {
  Usuario **nuevoArreglo = new Usuario *[nuevaCapacidad];
  for (int i = 0; i < cantAmigos; ++i) {
    nuevoArreglo[i] = amigos[i];
  }
  delete[] amigos;
  amigos = nuevoArreglo;
  capAmigos = nuevaCapacidad;
}

void Usuario::redimensionarAlbumes(int nuevaCapacidad) {
  Album **nuevoArreglo = new Album *[nuevaCapacidad];
  for (int i = 0; i < cantAlbumes; ++i) {
    nuevoArreglo[i] = albumes[i];
  }
  delete[] albumes;
  albumes = nuevoArreglo;
  capAlbumes = nuevaCapacidad;
}

void Usuario::redimensionarMensajes(int nuevaCapacidad) {
  Mensaje **nuevoArreglo = new Mensaje *[nuevaCapacidad];
  for (int i = 0; i < cantMensajes; ++i) {
    nuevoArreglo[i] = mensajes[i];
  }
  delete[] mensajes;
  mensajes = nuevoArreglo;
  capMensajes = nuevaCapacidad;
}

void Usuario::redimensionarSolicitudes(int nuevaCapacidad) {
  SolicitudAmistad **nuevoArreglo = new SolicitudAmistad *[nuevaCapacidad];
  for (int i = 0; i < cantSolicitudes; ++i) {
    nuevoArreglo[i] = solicitudes[i];
  }
  delete[] solicitudes;
  solicitudes = nuevoArreglo;
  capSolicitudes = nuevaCapacidad;
}

void Usuario::liberarMemoria() {
  delete[] username;
  delete[] password;
  delete[] fechaNacimiento;
  delete[] correo;
  delete[] rutaFotoPerfil;
  delete[] ciudad;
  delete[] pais;
  delete[] fechaInscripcion;

  username = nullptr;
  password = nullptr;
  fechaNacimiento = nullptr;
  correo = nullptr;
  rutaFotoPerfil = nullptr;
  ciudad = nullptr;
  pais = nullptr;
  fechaInscripcion = nullptr;

  delete[] amigos;
  amigos = nullptr;
  cantAmigos = 0;
  capAmigos = 0;

  if (albumes != nullptr) {
    for (int i = 0; i < cantAlbumes; ++i) {
      delete albumes[i];
    }
    delete[] albumes;
    albumes = nullptr;
  }
  cantAlbumes = 0;
  capAlbumes = 0;

  if (mensajes != nullptr) {
    for (int i = 0; i < cantMensajes; ++i) {
      delete mensajes[i];
    }
    delete[] mensajes;
    mensajes = nullptr;
  }
  cantMensajes = 0;
  capMensajes = 0;

  if (solicitudes != nullptr) {
    for (int i = 0; i < cantSolicitudes; ++i) {
      delete solicitudes[i];
    }
    delete[] solicitudes;
    solicitudes = nullptr;
  }
  cantSolicitudes = 0;
  capSolicitudes = 0;
}

// Constructores y Destructor

Usuario::Usuario(const char *username, const char *password,
                 const char *fechaNacimiento, const char *correo,
                 const char *rutaFotoPerfil, const char *ciudad,
                 const char *pais, const char *fechaInscripcion)
    : username(UtilidadesCadena::duplicar(username)),
      password(UtilidadesCadena::duplicar(password)),
      fechaNacimiento(UtilidadesCadena::duplicar(fechaNacimiento)),
      correo(UtilidadesCadena::duplicar(correo)),
      rutaFotoPerfil(UtilidadesCadena::duplicar(rutaFotoPerfil)),
      ciudad(UtilidadesCadena::duplicar(ciudad)),
      pais(UtilidadesCadena::duplicar(pais)),
      fechaInscripcion(UtilidadesCadena::duplicar(fechaInscripcion)),
      amigos(nullptr), cantAmigos(0), capAmigos(0), albumes(nullptr),
      cantAlbumes(0), capAlbumes(0), mensajes(nullptr), cantMensajes(0),
      capMensajes(0), solicitudes(nullptr), cantSolicitudes(0),
      capSolicitudes(0) {}

Usuario::Usuario(const Usuario &otro)
    : username(UtilidadesCadena::duplicar(otro.username)),
      password(UtilidadesCadena::duplicar(otro.password)),
      fechaNacimiento(UtilidadesCadena::duplicar(otro.fechaNacimiento)),
      correo(UtilidadesCadena::duplicar(otro.correo)),
      rutaFotoPerfil(UtilidadesCadena::duplicar(otro.rutaFotoPerfil)),
      ciudad(UtilidadesCadena::duplicar(otro.ciudad)),
      pais(UtilidadesCadena::duplicar(otro.pais)),
      fechaInscripcion(UtilidadesCadena::duplicar(otro.fechaInscripcion)),
      amigos(nullptr), cantAmigos(0), capAmigos(0), albumes(nullptr),
      cantAlbumes(0), capAlbumes(0), mensajes(nullptr), cantMensajes(0),
      capMensajes(0), solicitudes(nullptr), cantSolicitudes(0),
      capSolicitudes(0) {
  if (otro.cantAmigos > 0) {
    capAmigos = otro.cantAmigos;
    amigos = new Usuario *[capAmigos];
    for (int i = 0; i < otro.cantAmigos; ++i) {
      amigos[i] = otro.amigos[i];
      ++cantAmigos;
    }
  }

  if (otro.cantAlbumes > 0) {
    capAlbumes = otro.cantAlbumes;
    albumes = new Album *[capAlbumes];
    for (int i = 0; i < otro.cantAlbumes; ++i) {
      albumes[i] = new Album(*otro.albumes[i]);
      ++cantAlbumes;
    }
  }

  if (otro.cantMensajes > 0) {
    capMensajes = otro.cantMensajes;
    mensajes = new Mensaje *[capMensajes];
    for (int i = 0; i < otro.cantMensajes; ++i) {
      mensajes[i] = new Mensaje(*otro.mensajes[i]);
      ++cantMensajes;
    }
  }

  if (otro.cantSolicitudes > 0) {
    capSolicitudes = otro.cantSolicitudes;
    solicitudes = new SolicitudAmistad *[capSolicitudes];
    for (int i = 0; i < otro.cantSolicitudes; ++i) {
      solicitudes[i] = new SolicitudAmistad(*otro.solicitudes[i]);
      ++cantSolicitudes;
    }
  }
}

Usuario::~Usuario() { liberarMemoria(); }

// Sobrecarga de Operadores

Usuario &Usuario::operator=(const Usuario &otro) {
  if (this != &otro) {
    char *nuevoUser = UtilidadesCadena::duplicar(otro.username);
    char *nuevoPass = UtilidadesCadena::duplicar(otro.password);
    char *nuevaFechaNac = UtilidadesCadena::duplicar(otro.fechaNacimiento);
    char *nuevoCorreo = UtilidadesCadena::duplicar(otro.correo);
    char *nuevaRuta = UtilidadesCadena::duplicar(otro.rutaFotoPerfil);
    char *nuevaCiudad = UtilidadesCadena::duplicar(otro.ciudad);
    char *nuevoPais = UtilidadesCadena::duplicar(otro.pais);
    char *nuevaFechaInsc = UtilidadesCadena::duplicar(otro.fechaInscripcion);

    liberarMemoria();

    username = nuevoUser;
    password = nuevoPass;
    fechaNacimiento = nuevaFechaNac;
    correo = nuevoCorreo;
    rutaFotoPerfil = nuevaRuta;
    ciudad = nuevaCiudad;
    pais = nuevoPais;
    fechaInscripcion = nuevaFechaInsc;

    if (otro.cantAmigos > 0) {
      capAmigos = otro.cantAmigos;
      amigos = new Usuario *[capAmigos];
      for (int i = 0; i < otro.cantAmigos; ++i) {
        amigos[i] = otro.amigos[i];
        ++cantAmigos;
      }
    }

    if (otro.cantAlbumes > 0) {
      capAlbumes = otro.cantAlbumes;
      albumes = new Album *[capAlbumes];
      for (int i = 0; i < otro.cantAlbumes; ++i) {
        albumes[i] = new Album(*otro.albumes[i]);
        ++cantAlbumes;
      }
    }

    if (otro.cantMensajes > 0) {
      capMensajes = otro.cantMensajes;
      mensajes = new Mensaje *[capMensajes];
      for (int i = 0; i < otro.cantMensajes; ++i) {
        mensajes[i] = new Mensaje(*otro.mensajes[i]);
        ++cantMensajes;
      }
    }

    if (otro.cantSolicitudes > 0) {
      capSolicitudes = otro.cantSolicitudes;
      solicitudes = new SolicitudAmistad *[capSolicitudes];
      for (int i = 0; i < otro.cantSolicitudes; ++i) {
        solicitudes[i] = new SolicitudAmistad(*otro.solicitudes[i]);
        ++cantSolicitudes;
      }
    }
  }
  return *this;
}

bool Usuario::operator==(const Usuario &otro) const {
  return UtilidadesCadena::sonIguales(username, otro.username);
}

std::ostream &operator<<(std::ostream &os, const Usuario &usuario) {
  os << "@" << (usuario.username ? usuario.username : "") << " ("
     << (usuario.ciudad ? usuario.ciudad : "") << ", "
     << (usuario.pais ? usuario.pais : "")
     << ") - Amigos: " << usuario.cantAmigos
     << " | Albumes: " << usuario.cantAlbumes;
  return os;
}

// Getters Básicos

const char *Usuario::getUsername() const { return username; }
const char *Usuario::getPassword() const { return password; }
const char *Usuario::getFechaNacimiento() const { return fechaNacimiento; }
const char *Usuario::getCorreo() const { return correo; }
const char *Usuario::getRutaFotoPerfil() const { return rutaFotoPerfil; }
const char *Usuario::getCiudad() const { return ciudad; }
const char *Usuario::getPais() const { return pais; }
const char *Usuario::getFechaInscripcion() const { return fechaInscripcion; }

// Gestión de Amistades

void Usuario::agregarAmigo(Usuario *nuevoAmigo) {
  if (nuevoAmigo == nullptr || nuevoAmigo == this || esAmigoDe(nuevoAmigo)) {
    return;
  }
  if (cantAmigos == capAmigos) {
    const int nuevaCap = (capAmigos == 0) ? 2 : capAmigos * 2;
    redimensionarAmigos(nuevaCap);
  }
  amigos[cantAmigos] = nuevoAmigo;
  ++cantAmigos;
}

void Usuario::establecerAmistadMutua(Usuario *otro) {
  if (otro == nullptr || otro == this) {
    return;
  }
  this->agregarAmigo(otro);
  otro->agregarAmigo(this);
}

bool Usuario::esAmigoDe(const Usuario *otro) const {
  if (otro == nullptr) {
    return false;
  }
  for (int i = 0; i < cantAmigos; ++i) {
    if (amigos[i] == otro || (amigos[i] != nullptr && *amigos[i] == *otro)) {
      return true;
    }
  }
  return false;
}

int Usuario::getCantAmigos() const { return cantAmigos; }

Usuario *Usuario::getAmigoEn(int indice) const {
  if (indice >= 0 && indice < cantAmigos) {
    return amigos[indice];
  }
  return nullptr;
}

int Usuario::contarAmigosEnComun(const Usuario *otro) const {
  if (otro == nullptr) {
    return 0;
  }
  int enComun = 0;
  for (int i = 0; i < cantAmigos; ++i) {
    if (otro->esAmigoDe(amigos[i])) {
      ++enComun;
    }
  }
  return enComun;
}

// Gestión de Álbumes

void Usuario::agregarAlbum(Album *nuevoAlbum) {
  if (nuevoAlbum == nullptr ||
      buscarAlbumPorNombre(nuevoAlbum->getNombre()) != nullptr) {
    return;
  }
  if (cantAlbumes == capAlbumes) {
    const int nuevaCap = (capAlbumes == 0) ? 2 : capAlbumes * 2;
    redimensionarAlbumes(nuevaCap);
  }
  albumes[cantAlbumes] = nuevoAlbum;
  ++cantAlbumes;
}

int Usuario::getCantAlbumes() const { return cantAlbumes; }

Album *Usuario::getAlbumEn(int indice) const {
  if (indice >= 0 && indice < cantAlbumes) {
    return albumes[indice];
  }
  return nullptr;
}

Album *Usuario::buscarAlbumPorNombre(const char *nombre) const {
  if (nombre == nullptr) {
    return nullptr;
  }
  for (int i = 0; i < cantAlbumes; ++i) {
    if (UtilidadesCadena::sonIguales(albumes[i]->getNombre(), nombre)) {
      return albumes[i];
    }
  }
  return nullptr;
}

// Gestión de Mensajería

void Usuario::recibirMensaje(Mensaje *mensaje) {
  if (mensaje == nullptr) {
    return;
  }
  if (cantMensajes == capMensajes) {
    const int nuevaCap = (capMensajes == 0) ? 2 : capMensajes * 2;
    redimensionarMensajes(nuevaCap);
  }
  // Inserción ordenada por fechaHora para mantener los mensajes en orden cronológico
  int pos = cantMensajes;
  while (pos > 0 &&
         UtilidadesCadena::comparar(mensajes[pos - 1]->getFechaHora(),
                                    mensaje->getFechaHora()) > 0) {
    mensajes[pos] = mensajes[pos - 1];
    --pos;
  }
  mensajes[pos] = mensaje;
  ++cantMensajes;
}

int Usuario::getCantMensajes() const { return cantMensajes; }

Mensaje *Usuario::getMensajeEn(int indice) const {
  if (indice >= 0 && indice < cantMensajes) {
    return mensajes[indice];
  }
  return nullptr;
}

void Usuario::enviarMensaje(Usuario *destinatario, const char *texto,
                            const char *fechaHora) {
  if (destinatario == nullptr || destinatario == this ||
      !esAmigoDe(destinatario) || texto == nullptr) {
    return;
  }
  bool tieneContenido = false;
  for (int i = 0; texto[i] != '\0'; ++i) {
    if (texto[i] != ' ' && texto[i] != '\t' && texto[i] != '\r' &&
        texto[i] != '\n') {
      tieneContenido = true;
      break;
    }
  }
  if (!tieneContenido) {
    return;
  }
  // Se almacena únicamente en el receptor para evitar duplicar el objeto en
  // Heap
  destinatario->recibirMensaje(
      new Mensaje(this, destinatario, fechaHora, texto));
}

// Gestión de Solicitudes de Amistad

void Usuario::recibirSolicitud(SolicitudAmistad *solicitud) {
  if (solicitud == nullptr) {
    return;
  }
  if (cantSolicitudes == capSolicitudes) {
    const int nuevaCap = (capSolicitudes == 0) ? 2 : capSolicitudes * 2;
    redimensionarSolicitudes(nuevaCap);
  }
  solicitudes[cantSolicitudes] = solicitud;
  ++cantSolicitudes;
}

int Usuario::getCantSolicitudes() const { return cantSolicitudes; }

SolicitudAmistad *Usuario::getSolicitudEn(int indice) const {
  if (indice >= 0 && indice < cantSolicitudes) {
    return solicitudes[indice];
  }
  return nullptr;
}

bool Usuario::tieneSolicitudPendienteCon(const Usuario *otro) const {
  if (otro == nullptr) {
    return false;
  }
  // Verifica solicitudes recibidas por este usuario enviadas por 'otro'
  for (int i = 0; i < cantSolicitudes; ++i) {
    if (solicitudes[i]->estaPendiente()) {
      const Usuario *emisor = solicitudes[i]->getEmisor();
      if (emisor == otro || (emisor != nullptr && *emisor == *otro)) {
        return true;
      }
    }
  }
  // Verifica solicitudes recibidas por 'otro' enviadas por este usuario
  for (int i = 0; i < otro->cantSolicitudes; ++i) {
    if (otro->solicitudes[i]->estaPendiente()) {
      const Usuario *emisor = otro->solicitudes[i]->getEmisor();
      if (emisor == this || (emisor != nullptr && *emisor == *this)) {
        return true;
      }
    }
  }
  return false;
}

void Usuario::enviarSolicitudAmistad(Usuario *destinatario,
                                     const char *fechaHora) {
  if (destinatario == nullptr || destinatario == this ||
      esAmigoDe(destinatario) || tieneSolicitudPendienteCon(destinatario)) {
    return;
  }
  // Se almacena únicamente en el destinatario para evitar copia duplicada
  destinatario->recibirSolicitud(
      new SolicitudAmistad(this, destinatario, fechaHora));
}

// 3 Mejores Amigos por Ponderación

void Usuario::seleccionar3MejoresAmigos(const char **rutasFotos,
                                        int &cantSeleccionadas) const {
  cantSeleccionadas = 0;
  if (cantAmigos == 0) {
    return;
  }

  if (cantAmigos <= 3) {
    for (int i = 0; i < cantAmigos; ++i) {
      rutasFotos[i] = amigos[i]->getRutaFotoPerfil();
    }
    cantSeleccionadas = cantAmigos;
    return;
  }

  double *pesos = new double[cantAmigos];
  bool *seleccionados = new bool[cantAmigos];

  for (int i = 0; i < cantAmigos; ++i) {
    const int comun = contarAmigosEnComun(amigos[i]);
    if (comun <= 10) {
      pesos[i] = 1.0;
    } else if (comun <= 20) {
      pesos[i] = 2.5;
    } else {
      pesos[i] = 3.5;
    }
    seleccionados[i] = false;
  }

  std::random_device rd;
  std::mt19937 gen(rd());

  while (cantSeleccionadas < 3) {
    double sumaRestante = 0.0;
    for (int i = 0; i < cantAmigos; ++i) {
      if (!seleccionados[i]) {
        sumaRestante += pesos[i];
      }
    }
    if (sumaRestante <= 0.0) {
      break;
    }

    std::uniform_real_distribution<double> dist(0.0, sumaRestante);
    const double tiro = dist(gen);

    double acum = 0.0;
    for (int i = 0; i < cantAmigos; ++i) {
      if (!seleccionados[i]) {
        acum += pesos[i];
        if (tiro <= acum) {
          seleccionados[i] = true;
          rutasFotos[cantSeleccionadas] = amigos[i]->getRutaFotoPerfil();
          ++cantSeleccionadas;
          break;
        }
      }
    }
  }

  delete[] pesos;
  delete[] seleccionados;
}
