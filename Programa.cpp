// ===========================================================================
// Programa.cpp — Implementación de la Controladora del Sistema (NIVEL 5)
// ===========================================================================

#include "Programa.h"
#include "Album.h"
#include "Foto.h"
#include "Mensaje.h"
#include "SolicitudAmistad.h"
#include "UtilidadesCadena.h"
#include <fstream>
#include <iostream>

// ---------------------------------------------------------------------------
// Clase SugerenciaAmigo
// ---------------------------------------------------------------------------

SugerenciaAmigo::SugerenciaAmigo()
    : usuario(nullptr), amigosEnComun(0) {}

SugerenciaAmigo::SugerenciaAmigo(Usuario *u, std::size_t c)
    : usuario(u), amigosEnComun(c) {}

// ---------------------------------------------------------------------------
// Helpers de Memoria y Grafo
// ---------------------------------------------------------------------------

void Programa::redimensionarUsuarios(std::size_t nuevaCapacidad) {
  Usuario **nuevoArreglo = new Usuario *[nuevaCapacidad];
  for (std::size_t i = 0; i < cantUsuarios; ++i) {
    nuevoArreglo[i] = usuarios[i];
  }
  delete[] usuarios;
  usuarios = nuevoArreglo;
  capUsuarios = nuevaCapacidad;
}

void Programa::liberarMatriz() {
  if (matrizAdyacencia != nullptr) {
    for (std::size_t i = 0; i < tamañoMatriz; ++i) {
      delete[] matrizAdyacencia[i];
    }
    delete[] matrizAdyacencia;
    matrizAdyacencia = nullptr;
  }
  tamañoMatriz = 0;
}

void Programa::liberarUsuarios() {
  if (usuarios != nullptr) {
    for (std::size_t i = 0; i < cantUsuarios; ++i) {
      delete usuarios[i];
    }
    delete[] usuarios;
    usuarios = nullptr;
  }
  cantUsuarios = 0;
  capUsuarios = 0;
}

// ---------------------------------------------------------------------------
// Constructor y Destructor
// ---------------------------------------------------------------------------

Programa::Programa()
    : usuarios(nullptr),
      cantUsuarios(0),
      capUsuarios(0),
      matrizAdyacencia(nullptr),
      tamañoMatriz(0),
      contadorIteraciones(0) {
  rutaPersistencia[0] = '\0';
}

Programa::~Programa() {
  liberarMatriz();
  liberarUsuarios();
}

// ---------------------------------------------------------------------------
// Gestión de Usuarios
// ---------------------------------------------------------------------------

void Programa::registrarUsuario(Usuario *nuevoUsuario) {
  if (nuevoUsuario == nullptr) {
    return;
  }
  if (cantUsuarios == capUsuarios) {
    const std::size_t nuevaCap = (capUsuarios == 0) ? 4 : capUsuarios * 2;
    redimensionarUsuarios(nuevaCap);
  }
  usuarios[cantUsuarios] = nuevoUsuario;
  ++cantUsuarios;

  // Actualiza la matriz con la nueva dimensión
  construirMatrizAdyacencia();
}

Usuario *Programa::buscarUsuario(const char *username) const {
  if (username == nullptr) {
    return nullptr;
  }
  for (std::size_t i = 0; i < cantUsuarios; ++i) {
    registrarIteraciones(1);
    if (UtilidadesCadena::sonIguales(usuarios[i]->getUsername(), username)) {
      return usuarios[i];
    }
  }
  return nullptr;
}

int Programa::obtenerIndiceUsuario(const Usuario *u) const {
  if (u == nullptr) {
    return -1;
  }
  for (std::size_t i = 0; i < cantUsuarios; ++i) {
    registrarIteraciones(1);
    if (usuarios[i] == u || *usuarios[i] == *u) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

std::size_t Programa::getCantUsuarios() const {
  return cantUsuarios;
}

Usuario *Programa::getUsuarioEn(std::size_t indice) const {
  if (indice < cantUsuarios) {
    return usuarios[indice];
  }
  return nullptr;
}

// ---------------------------------------------------------------------------
// Autenticación (Requisito II)
// ---------------------------------------------------------------------------

Usuario *Programa::autenticarUsuario(const char *username,
                                     const char *password) const {
  if (username == nullptr || password == nullptr) {
    return nullptr;
  }
  for (std::size_t i = 0; i < cantUsuarios; ++i) {
    registrarIteraciones(1);
    if (UtilidadesCadena::sonIguales(usuarios[i]->getUsername(), username) &&
        UtilidadesCadena::sonIguales(usuarios[i]->getPassword(), password)) {
      return usuarios[i];
    }
  }
  return nullptr;
}

// ---------------------------------------------------------------------------
// Grafo de Amistades (Matriz de Adyacencia)
// ---------------------------------------------------------------------------

void Programa::construirMatrizAdyacencia() {
  liberarMatriz();
  if (cantUsuarios == 0) {
    return;
  }

  tamañoMatriz = cantUsuarios;
  matrizAdyacencia = new bool *[tamañoMatriz];
  for (std::size_t i = 0; i < tamañoMatriz; ++i) {
    matrizAdyacencia[i] = new bool[tamañoMatriz];
    for (std::size_t j = 0; j < tamañoMatriz; ++j) {
      matrizAdyacencia[i][j] = false;
    }
  }

  // Llena la matriz a partir de las listas de amigos
  for (std::size_t i = 0; i < cantUsuarios; ++i) {
    Usuario *u = usuarios[i];
    for (std::size_t a = 0; a < u->getCantAmigos(); ++a) {
      Usuario *amigo = u->getAmigoEn(a);
      const int j = obtenerIndiceUsuario(amigo);
      if (j >= 0) {
        matrizAdyacencia[i][j] = true;
        matrizAdyacencia[j][i] = true;
      }
    }
  }
}

bool Programa::sonAmigosPorMatriz(std::size_t i, std::size_t j) const {
  if (i < tamañoMatriz && j < tamañoMatriz && matrizAdyacencia != nullptr) {
    return matrizAdyacencia[i][j];
  }
  return false;
}

std::size_t Programa::contarAmigosEnComunPorMatriz(std::size_t i,
                                                   std::size_t j) const {
  if (i >= tamañoMatriz || j >= tamañoMatriz || matrizAdyacencia == nullptr) {
    return 0;
  }
  std::size_t comun = 0;
  for (std::size_t col = 0; col < tamañoMatriz; ++col) {
    registrarIteraciones(1);
    if (matrizAdyacencia[i][col] && matrizAdyacencia[j][col]) {
      ++comun;
    }
  }
  return comun;
}

// ---------------------------------------------------------------------------
// Sugerencias de Amistad (Requisito V)
// ---------------------------------------------------------------------------

void Programa::sugerirAmigos(const Usuario *actual, std::size_t k,
                             Usuario **sugeridos, std::size_t *amigosComun,
                             std::size_t &cantSugeridos) const {
  cantSugeridos = 0;
  const int idxActual = obtenerIndiceUsuario(actual);
  if (idxActual < 0 || tamañoMatriz == 0) {
    return;
  }

  // Arreglo temporal de candidatos
  SugerenciaAmigo *candidatos = new SugerenciaAmigo[cantUsuarios];
  std::size_t cantCandidatos = 0;

  for (std::size_t j = 0; j < tamañoMatriz; ++j) {
    registrarIteraciones(1);
    // Un candidato no puede ser el mismo usuario ni un amigo ya existente
    if (static_cast<int>(j) != idxActual && !matrizAdyacencia[idxActual][j]) {
      const std::size_t comun = contarAmigosEnComunPorMatriz(static_cast<std::size_t>(idxActual), j);
      // Solo sugiere si son "amigos de sus amigos directos" (amigos en común > 0)
      if (comun > 0) {
        candidatos[cantCandidatos] = SugerenciaAmigo(usuarios[j], comun);
        ++cantCandidatos;
      }
    }
  }

  // Ordenamiento de mayor a menor según cantidad de amigos en común
  for (std::size_t i = 0; i < cantCandidatos; ++i) {
    for (std::size_t j = i + 1; j < cantCandidatos; ++j) {
      registrarIteraciones(1);
      if (candidatos[j].amigosEnComun > candidatos[i].amigosEnComun) {
        SugerenciaAmigo temp = candidatos[i];
        candidatos[i] = candidatos[j];
        candidatos[j] = temp;
      }
    }
  }

  // Limita a un máximo de K sugerencias
  const std::size_t maxK = (k > 20) ? 20 : ((k == 0) ? 1 : k);
  const std::size_t limite = (cantCandidatos < maxK) ? cantCandidatos : maxK;

  for (std::size_t i = 0; i < limite; ++i) {
    sugeridos[i] = candidatos[i].usuario;
    amigosComun[i] = candidatos[i].amigosEnComun;
    ++cantSugeridos;
  }

  delete[] candidatos;
}

// ---------------------------------------------------------------------------
// Persistencia en Disco (Requisito I)
// ---------------------------------------------------------------------------

bool Programa::guardarDatos(const char *rutaArchivo) const {
  const char *destinos[2] = {nullptr, nullptr};
  std::size_t cantDestinos = 0;

  if (rutaArchivo != nullptr && rutaArchivo[0] != '\0') {
    destinos[cantDestinos++] = rutaArchivo;
  }
  if (rutaPersistencia[0] != '\0') {
    if (cantDestinos == 0 || !UtilidadesCadena::sonIguales(destinos[0], rutaPersistencia)) {
      destinos[cantDestinos++] = rutaPersistencia;
    }
  }
  if (cantDestinos == 0) {
    destinos[0] = "datos_udeabook.txt";
    cantDestinos = 1;
  }

  bool alMenosUno = false;
  for (std::size_t d = 0; d < cantDestinos; ++d) {
    std::ofstream archivo(destinos[d]);
    if (!archivo.is_open()) {
      continue;
    }
    alMenosUno = true;

    // 1. Guardar Usuarios
    archivo << "[USUARIOS]\n";
    archivo << cantUsuarios << '\n';
    for (std::size_t i = 0; i < cantUsuarios; ++i) {
      Usuario *u = usuarios[i];
      archivo << u->getUsername() << ';'
              << u->getPassword() << ';'
              << u->getFechaNacimiento() << ';'
              << u->getCorreo() << ';'
              << u->getRutaFotoPerfil() << ';'
              << u->getCiudad() << ';'
              << u->getPais() << ';'
              << u->getFechaInscripcion() << '\n';
    }

    // 2. Guardar Amistades (Pares únicos)
    archivo << "[AMISTADES]\n";
    for (std::size_t i = 0; i < cantUsuarios; ++i) {
      for (std::size_t j = i + 1; j < cantUsuarios; ++j) {
        if (matrizAdyacencia != nullptr && matrizAdyacencia[i][j]) {
          archivo << usuarios[i]->getUsername() << ';'
                  << usuarios[j]->getUsername() << '\n';
        }
      }
    }
    archivo << "[FIN_AMISTADES]\n";

    // 3. Guardar Álbumes y Fotos
    archivo << "[ALBUMES]\n";
    for (std::size_t i = 0; i < cantUsuarios; ++i) {
      Usuario *u = usuarios[i];
      for (std::size_t a = 0; a < u->getCantAlbumes(); ++a) {
        Album *alb = u->getAlbumEn(a);
        archivo << u->getUsername() << ';'
                << alb->getTipoPrivacidad() << ';'
                << alb->getNombre() << ';'
                << alb->getRutaAlmacenamiento() << ';'
                << alb->getCantFotos() << '\n';

        for (std::size_t f = 0; f < alb->getCantFotos(); ++f) {
          Foto *foto = alb->getFotoEn(f);
          archivo << foto->getRuta() << ';'
                  << foto->getFechaCaptura() << ';'
                  << foto->getTamanoBytes() << ';'
                  << foto->getLikes() << '\n';
        }
      }
    }
    archivo << "[FIN_ALBUMES]\n";

    // 4. Guardar Mensajes
    archivo << "[MENSAJES]\n";
    for (std::size_t i = 0; i < cantUsuarios; ++i) {
      Usuario *u = usuarios[i];
      for (std::size_t n = 0; n < u->getCantMensajes(); ++n) {
        Mensaje *m = u->getMensajeEn(n);
        if (m->getReceptor() == u) {
          archivo << (m->getEmisor() ? m->getEmisor()->getUsername() : "anonimo") << ';'
                  << u->getUsername() << ';'
                  << m->getFechaHora() << ';'
                  << (m->estaLeido() ? 1 : 0) << ';'
                  << m->getTexto() << '\n';
        }
      }
    }
    archivo << "[FIN_MENSAJES]\n";

    // 5. Guardar Solicitudes de Amistad
    archivo << "[SOLICITUDES]\n";
    for (std::size_t i = 0; i < cantUsuarios; ++i) {
      Usuario *u = usuarios[i];
      for (std::size_t n = 0; n < u->getCantSolicitudes(); ++n) {
        SolicitudAmistad *s = u->getSolicitudEn(n);
        if (s->getReceptor() == u) {
          archivo << (s->getEmisor() ? s->getEmisor()->getUsername() : "anonimo") << ';'
                  << u->getUsername() << ';'
                  << s->getFechaHora() << ';'
                  << s->getEstado() << '\n';
        }
      }
    }
    archivo << "[FIN_SOLICITUDES]\n";

    archivo.close();
  }
  return alMenosUno;
}

bool Programa::cargarDatos(const char *rutaArchivo) {
  if (rutaArchivo == nullptr) {
    return false;
  }
  std::ifstream archivo(rutaArchivo);
  char rutaUsada[256];
  UtilidadesCadena::copiar(rutaUsada, rutaArchivo, sizeof(rutaUsada));

  if (!archivo.is_open()) {
    // Intenta buscar en el directorio padre si se ejecuta desde build/
    char rutaAlt[256] = "../";
    std::size_t idx = 3;
    for (std::size_t p = 0; rutaArchivo[p] != '\0' && idx < 254; ++p) {
      rutaAlt[idx++] = rutaArchivo[p];
    }
    rutaAlt[idx] = '\0';
    archivo.open(rutaAlt);
    if (!archivo.is_open()) {
      return false;
    }
    UtilidadesCadena::copiar(rutaUsada, rutaAlt, sizeof(rutaUsada));
  }
  UtilidadesCadena::copiar(rutaPersistencia, rutaUsada, sizeof(rutaPersistencia));

  char linea[1024];

  // 1. Leer sección de usuarios
  while (archivo.getline(linea, sizeof(linea))) {
    std::size_t l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r') linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[USUARIOS]")) {
      break;
    }
  }

  std::size_t numUsuarios = 0;
  if (!(archivo >> numUsuarios)) {
    return false;
  }
  archivo.getline(linea, sizeof(linea)); // Consumir salto de línea

  if (numUsuarios > capUsuarios) {
    redimensionarUsuarios(numUsuarios + 50);
  }

  for (std::size_t i = 0; i < numUsuarios; ++i) {
    if (!archivo.getline(linea, sizeof(linea))) break;

    std::size_t len = UtilidadesCadena::longitud(linea);
    if (len > 0 && linea[len - 1] == '\r') {
      linea[len - 1] = '\0';
    }

    // Parseo manual con campos separados por ';'
    char campos[8][256];
    std::size_t campoActual = 0;
    std::size_t pos = 0;

    for (std::size_t c = 0; linea[c] != '\0' && campoActual < 8; ++c) {
      if (linea[c] == ';') {
        campos[campoActual][pos] = '\0';
        ++campoActual;
        pos = 0;
      } else {
        if (pos < 255) {
          campos[campoActual][pos++] = linea[c];
        }
      }
    }
    campos[campoActual][pos] = '\0';

    if (campoActual == 7) {
      Usuario *u = new Usuario(campos[0], campos[1], campos[2], campos[3],
                               campos[4], campos[5], campos[6], campos[7]);
      if (cantUsuarios == capUsuarios) {
        const std::size_t nuevaCap = (capUsuarios == 0) ? 4 : capUsuarios * 2;
        redimensionarUsuarios(nuevaCap);
      }
      usuarios[cantUsuarios] = u;
      ++cantUsuarios;
    }
  }

  // 2. Leer sección de amistades
  while (archivo.getline(linea, sizeof(linea))) {
    std::size_t l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r') linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[AMISTADES]")) {
      break;
    }
  }

  while (archivo.getline(linea, sizeof(linea))) {
    std::size_t l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r') linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[FIN_AMISTADES]")) {
      break;
    }
    char userA[128];
    char userB[128];
    std::size_t idx = 0, p = 0;
    while (linea[idx] != ';' && linea[idx] != '\0' && p < 127) {
      userA[p++] = linea[idx++];
    }
    userA[p] = '\0';
    if (linea[idx] == ';') ++idx;
    p = 0;
    while (linea[idx] != '\0' && p < 127) {
      userB[p++] = linea[idx++];
    }
    userB[p] = '\0';

    Usuario *uA = buscarUsuario(userA);
    Usuario *uB = buscarUsuario(userB);
    if (uA != nullptr && uB != nullptr) {
      uA->establecerAmistadMutua(uB);
    }
  }

  // 3. Leer sección de álbumes y fotos
  while (archivo.getline(linea, sizeof(linea))) {
    std::size_t l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r') linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[ALBUMES]")) {
      break;
    }
  }

  while (archivo.getline(linea, sizeof(linea))) {
    std::size_t l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r') linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[FIN_ALBUMES]")) {
      break;
    }
    char camposAlb[5][256];
    std::size_t cIdx = 0, p = 0;
    for (std::size_t i = 0; linea[i] != '\0' && cIdx < 5; ++i) {
      if (linea[i] == ';') {
        camposAlb[cIdx][p] = '\0';
        ++cIdx;
        p = 0;
      } else if (p < 255) {
        camposAlb[cIdx][p++] = linea[i];
      }
    }
    camposAlb[cIdx][p] = '\0';

    if (cIdx >= 4) {
      Usuario *owner = buscarUsuario(camposAlb[0]);
      Album *alb = new Album(camposAlb[2], camposAlb[3], camposAlb[1]);

      std::size_t nFotos = 0;
      for (std::size_t k = 0; camposAlb[4][k] != '\0'; ++k) {
        if (camposAlb[4][k] >= '0' && camposAlb[4][k] <= '9') {
          nFotos = nFotos * 10 + (camposAlb[4][k] - '0');
        }
      }

      for (std::size_t f = 0; f < nFotos; ++f) {
        if (!archivo.getline(linea, sizeof(linea))) break;
        std::size_t lf = UtilidadesCadena::longitud(linea);
        if (lf > 0 && linea[lf - 1] == '\r') linea[lf - 1] = '\0';

        char camposFoto[4][256];
        std::size_t fIdx = 0, fp = 0;
        for (std::size_t fi = 0; linea[fi] != '\0' && fIdx < 4; ++fi) {
          if (linea[fi] == ';') {
            camposFoto[fIdx][fp] = '\0';
            ++fIdx;
            fp = 0;
          } else if (fp < 255) {
            camposFoto[fIdx][fp++] = linea[fi];
          }
        }
        camposFoto[fIdx][fp] = '\0';

        std::size_t tamBytes = 1024;
        std::size_t numTam = 0;
        for (std::size_t tk = 0; camposFoto[2][tk] != '\0'; ++tk) {
          if (camposFoto[2][tk] >= '0' && camposFoto[2][tk] <= '9') {
            numTam = numTam * 10 + (camposFoto[2][tk] - '0');
          }
        }
        if (numTam > 0) tamBytes = numTam;

        int nLikes = 0;
        for (std::size_t lk = 0; camposFoto[3][lk] != '\0'; ++lk) {
          if (camposFoto[3][lk] >= '0' && camposFoto[3][lk] <= '9') {
            nLikes = nLikes * 10 + (camposFoto[3][lk] - '0');
          }
        }

        Foto *foto = new Foto(camposFoto[0], camposFoto[1], tamBytes);
        for (int l = 0; l < nLikes; ++l) {
          foto->darLike();
        }
        alb->agregarFoto(foto);
      }

      if (owner != nullptr) {
        owner->agregarAlbum(alb);
      } else {
        delete alb;
      }
    }
  }

  // 4. Leer sección de mensajes
  while (archivo.getline(linea, sizeof(linea))) {
    std::size_t l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r') linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[MENSAJES]")) {
      break;
    }
  }

  while (archivo.getline(linea, sizeof(linea))) {
    std::size_t l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r') linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[FIN_MENSAJES]")) {
      break;
    }
    char camposMsg[5][514];
    std::size_t mIdx = 0, mp = 0;
    for (std::size_t i = 0; linea[i] != '\0' && mIdx < 5; ++i) {
      if (linea[i] == ';') {
        camposMsg[mIdx][mp] = '\0';
        ++mIdx;
        mp = 0;
      } else if (mp < 513) {
        camposMsg[mIdx][mp++] = linea[i];
      }
    }
    camposMsg[mIdx][mp] = '\0';

    if (mIdx >= 4) {
      Usuario *uEmisor = buscarUsuario(camposMsg[0]);
      Usuario *uReceptor = buscarUsuario(camposMsg[1]);
      if (uEmisor != nullptr && uReceptor != nullptr) {
        Mensaje *msgRecibido = new Mensaje(uEmisor, uReceptor, camposMsg[2], camposMsg[4]);
        if (camposMsg[3][0] == '1') {
          msgRecibido->marcarComoLeido();
        }
        uReceptor->recibirMensaje(msgRecibido);
      }
    }
  }

  // 5. Leer sección de solicitudes de amistad (si existe)
  while (archivo.getline(linea, sizeof(linea))) {
    std::size_t l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r') linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[SOLICITUDES]")) {
      break;
    }
  }

  while (archivo.getline(linea, sizeof(linea))) {
    std::size_t l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r') linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[FIN_SOLICITUDES]")) {
      break;
    }
    char camposSol[4][128];
    std::size_t sIdx = 0, sp = 0;
    for (std::size_t i = 0; linea[i] != '\0' && sIdx < 4; ++i) {
      if (linea[i] == ';') {
        camposSol[sIdx][sp] = '\0';
        ++sIdx;
        sp = 0;
      } else if (sp < 127) {
        camposSol[sIdx][sp++] = linea[i];
      }
    }
    camposSol[sIdx][sp] = '\0';

    if (sIdx >= 3) {
      Usuario *uEmisor = buscarUsuario(camposSol[0]);
      Usuario *uReceptor = buscarUsuario(camposSol[1]);
      if (uEmisor != nullptr && uReceptor != nullptr) {
        SolicitudAmistad *solRecibida = new SolicitudAmistad(uEmisor, uReceptor, camposSol[2]);
        if (camposSol[3][0] == '1') {
          solRecibida->aceptar();
        } else if (camposSol[3][0] == '2') {
          solRecibida->rechazar();
        }
        uReceptor->recibirSolicitud(solRecibida);
      }
    }
  }

  construirMatrizAdyacencia();
  archivo.close();
  reiniciarIteraciones();
  return true;
}

// ---------------------------------------------------------------------------
// Métricas de Consumo de Recursos (Requisito VII)
// ---------------------------------------------------------------------------

void Programa::reiniciarIteraciones() {
  contadorIteraciones = 0;
}

void Programa::registrarIteraciones(std::size_t cantidad) const {
  contadorIteraciones += cantidad;
}

std::size_t Programa::getIteraciones() const {
  return contadorIteraciones;
}

std::size_t Programa::calcularMemoriaTotal(std::size_t memoriaLocal) const {
  std::size_t total = sizeof(Programa);
  total += capUsuarios * sizeof(Usuario *);

  // Matriz de adyacencia
  if (matrizAdyacencia != nullptr) {
    total += tamañoMatriz * sizeof(bool *);
    total += tamañoMatriz * tamañoMatriz * sizeof(bool);
  }

  // Memoria de cada usuario y sus estructuras hijas
  for (std::size_t i = 0; i < cantUsuarios; ++i) {
    Usuario *u = usuarios[i];
    total += sizeof(Usuario);

    // Cadenas del usuario
    total += UtilidadesCadena::longitud(u->getUsername()) + 1;
    total += UtilidadesCadena::longitud(u->getPassword()) + 1;
    total += UtilidadesCadena::longitud(u->getFechaNacimiento()) + 1;
    total += UtilidadesCadena::longitud(u->getCorreo()) + 1;
    total += UtilidadesCadena::longitud(u->getRutaFotoPerfil()) + 1;
    total += UtilidadesCadena::longitud(u->getCiudad()) + 1;
    total += UtilidadesCadena::longitud(u->getPais()) + 1;
    total += UtilidadesCadena::longitud(u->getFechaInscripcion()) + 1;

    // Arreglo de amigos
    total += u->getCantAmigos() * sizeof(Usuario *);

    // Álbumes y fotos
    total += u->getCantAlbumes() * sizeof(Album *);
    for (std::size_t a = 0; a < u->getCantAlbumes(); ++a) {
      Album *alb = u->getAlbumEn(a);
      total += sizeof(Album);
      total += UtilidadesCadena::longitud(alb->getNombre()) + 1;
      total += UtilidadesCadena::longitud(alb->getRutaAlmacenamiento()) + 1;
      total += alb->getCapFotos() * sizeof(Foto *);

      for (std::size_t f = 0; f < alb->getCantFotos(); ++f) {
        Foto *foto = alb->getFotoEn(f);
        total += sizeof(Foto);
        total += UtilidadesCadena::longitud(foto->getRuta()) + 1;
        total += UtilidadesCadena::longitud(foto->getFechaCaptura()) + 1;
      }
    }

    // Mensajes recibidos
    total += u->getCantMensajes() * sizeof(Mensaje *);
    for (std::size_t m = 0; m < u->getCantMensajes(); ++m) {
      Mensaje *men = u->getMensajeEn(m);
      total += sizeof(Mensaje);
      total += UtilidadesCadena::longitud(men->getFechaHora()) + 1;
      total += UtilidadesCadena::longitud(men->getTexto()) + 1;
    }

    // Solicitudes de amistad recibidas
    total += u->getCantSolicitudes() * sizeof(SolicitudAmistad *);
    for (std::size_t s = 0; s < u->getCantSolicitudes(); ++s) {
      SolicitudAmistad *sol = u->getSolicitudEn(s);
      total += sizeof(SolicitudAmistad);
      total += UtilidadesCadena::longitud(sol->getFechaHora()) + 1;
    }
  }

  // Suma las variables locales y parámetros por valor reportados por el llamador
  total += memoriaLocal;
  return total;
}

void Programa::reportarConsumo(const char *nombreFuncionalidad,
                               std::size_t memoriaLocal) const {
  const std::size_t totalBytes = calcularMemoriaTotal(memoriaLocal);
  const double totalKB = static_cast<double>(totalBytes) / 1024.0;

  std::cout << "\n============================================================\n";
  std::cout << "[METRICAS DE EFICIENCIA - REQUISITO VII]\n";
  std::cout << "Funcionalidad ejecutada: " << (nombreFuncionalidad ? nombreFuncionalidad : "General") << '\n';
  std::cout << "-> Iteraciones requeridas (directas e indirectas): " << contadorIteraciones << '\n';
  std::cout << "-> Memoria RAM total consumida en este instante: "
            << totalBytes << " bytes (" << totalKB << " KB)\n";
  std::cout << "============================================================\n";
}
