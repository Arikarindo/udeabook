// ===========================================================================
// Programa.cpp — Implementación de la Controladora del Sistema
// ===========================================================================

#include "Programa.h"
#include "Album.h"
#include "Foto.h"
#include "Mensaje.h"
#include "SolicitudAmistad.h"
#include "UtilidadesCadena.h"
#include <fstream>
#include <iostream>

// Helper interno para partir una línea por ';' in-place sin copiar caracteres
static int dividirLineaPorPuntoYComa(char *linea, const char **campos,
                                     int maxCampos) {
  if (linea == nullptr || campos == nullptr || maxCampos <= 0) {
    return 0;
  }
  int len = UtilidadesCadena::longitud(linea);
  if (len > 0 && linea[len - 1] == '\r') {
    linea[len - 1] = '\0';
  }

  int numCampos = 0;
  campos[numCampos++] = linea;

  for (int i = 0; linea[i] != '\0'; ++i) {
    if (linea[i] == ';') {
      linea[i] = '\0';
      if (numCampos < maxCampos) {
        campos[numCampos++] = &linea[i + 1];
      }
    }
  }
  return numCampos;
}

// Clase SugerenciaAmigo

SugerenciaAmigo::SugerenciaAmigo() : usuario(nullptr), amigosEnComun(0) {}

SugerenciaAmigo::SugerenciaAmigo(Usuario *u, int c)
    : usuario(u), amigosEnComun(c) {}

// Helpers de Memoria y Grafo

void Programa::redimensionarUsuarios(int nuevaCapacidad) {
  Usuario **nuevoArreglo = new Usuario *[nuevaCapacidad];
  for (int i = 0; i < cantUsuarios; ++i) {
    nuevoArreglo[i] = usuarios[i];
  }
  delete[] usuarios;
  usuarios = nuevoArreglo;
  capUsuarios = nuevaCapacidad;
}

void Programa::liberarMatriz() {
  if (matrizAdyacencia != nullptr) {
    for (int i = 0; i < tamañoMatriz; ++i) {
      delete[] matrizAdyacencia[i];
    }
    delete[] matrizAdyacencia;
    matrizAdyacencia = nullptr;
  }
  tamañoMatriz = 0;
}

void Programa::liberarUsuarios() {
  if (usuarios != nullptr) {
    for (int i = 0; i < cantUsuarios; ++i) {
      delete usuarios[i];
    }
    delete[] usuarios;
    usuarios = nullptr;
  }
  cantUsuarios = 0;
  capUsuarios = 0;
}

// Constructor y Destructor

Programa::Programa()
    : usuarios(nullptr), cantUsuarios(0), capUsuarios(0),
      matrizAdyacencia(nullptr), tamañoMatriz(0), contadorIteraciones(0) {
  rutaDatabase[0] = '\0';
}

Programa::~Programa() {
  liberarMatriz();
  liberarUsuarios();
}

// Gestión de Usuarios

void Programa::registrarUsuario(Usuario *nuevoUsuario) {
  if (nuevoUsuario == nullptr ||
      buscarUsuario(nuevoUsuario->getUsername()) != nullptr) {
    return;
  }
  if (cantUsuarios == capUsuarios) {
    const int nuevaCap = (capUsuarios == 0) ? 4 : capUsuarios * 2;
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
  for (int i = 0; i < cantUsuarios; ++i) {
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
  for (int i = 0; i < cantUsuarios; ++i) {
    registrarIteraciones(1);
    if (usuarios[i] == u || *usuarios[i] == *u) {
      return i;
    }
  }
  return -1;
}

int Programa::getCantUsuarios() const { return cantUsuarios; }

Usuario *Programa::getUsuarioEn(int indice) const {
  if (indice >= 0 && indice < cantUsuarios) {
    return usuarios[indice];
  }
  return nullptr;
}

// Autenticación

Usuario *Programa::autenticarUsuario(const char *username,
                                     const char *password) const {
  if (username == nullptr || password == nullptr) {
    return nullptr;
  }
  for (int i = 0; i < cantUsuarios; ++i) {
    registrarIteraciones(1);
    if (UtilidadesCadena::sonIguales(usuarios[i]->getUsername(), username) &&
        UtilidadesCadena::sonIguales(usuarios[i]->getPassword(), password)) {
      return usuarios[i];
    }
  }
  return nullptr;
}

// Grafo de Amistades (Matriz de Adyacencia)

void Programa::construirMatrizAdyacencia() {
  liberarMatriz();
  if (cantUsuarios == 0) {
    return;
  }

  tamañoMatriz = cantUsuarios;
  matrizAdyacencia = new bool *[tamañoMatriz];
  for (int i = 0; i < tamañoMatriz; ++i) {
    bool *filaI = new bool[tamañoMatriz];
    matrizAdyacencia[i] = filaI;
    for (int j = 0; j < tamañoMatriz; ++j) {
      filaI[j] = false;
    }
  }

  // Llena la matriz a partir de las listas de amigos usando punteros de fila
  for (int i = 0; i < cantUsuarios; ++i) {
    Usuario *u = usuarios[i];
    bool *filaI = matrizAdyacencia[i];
    const int numAmigos = u->getCantAmigos();
    for (int a = 0; a < numAmigos; ++a) {
      Usuario *amigo = u->getAmigoEn(a);
      const int j = obtenerIndiceUsuario(amigo);
      if (j >= 0 && !filaI[j]) {
        filaI[j] = true;
        matrizAdyacencia[j][i] = true;
      }
    }
  }
}

void Programa::registrarAmistadEnMatriz(const Usuario *uA, const Usuario *uB) {
  if (matrizAdyacencia == nullptr || tamañoMatriz != cantUsuarios) {
    construirMatrizAdyacencia();
    return;
  }
  const int i = obtenerIndiceUsuario(uA);
  const int j = obtenerIndiceUsuario(uB);
  if (i >= 0 && j >= 0 && i < tamañoMatriz && j < tamañoMatriz) {
    matrizAdyacencia[i][j] = true;
    matrizAdyacencia[j][i] = true;
  }
}

bool Programa::sonAmigosPorMatriz(int i, int j) const {
  if (i >= 0 && j >= 0 && i < tamañoMatriz && j < tamañoMatriz &&
      matrizAdyacencia != nullptr) {
    return matrizAdyacencia[i][j];
  }
  return false;
}

int Programa::contarAmigosEnComunPorMatriz(int i, int j) const {
  if (i < 0 || j < 0 || i >= tamañoMatriz || j >= tamañoMatriz ||
      matrizAdyacencia == nullptr) {
    return 0;
  }
  // Punteros directos a cada fila para evitar doble desreferenciación en el
  // ciclo

  const bool *filaI = matrizAdyacencia[i];
  const bool *filaJ = matrizAdyacencia[j];
  int comun = 0;
  for (int col = 0; col < tamañoMatriz; ++col) {
    registrarIteraciones(1);
    if (filaI[col] && filaJ[col]) {
      ++comun;
    }
  }
  return comun;
}

// Sugerencias de Amistad

void Programa::sugerirAmigos(const Usuario *actual, int k, Usuario **sugeridos,
                             int *amigosComun, int &cantSugeridos) const {
  cantSugeridos = 0;
  const int indiceActual = obtenerIndiceUsuario(actual);
  if (indiceActual < 0 || tamañoMatriz == 0 || matrizAdyacencia == nullptr) {
    return;
  }

  // Puntero directo a la fila de amistades del usuario actual
  const bool *filaActual = matrizAdyacencia[indiceActual];

  // Arreglo temporal de candidatos
  SugerenciaAmigo *candidatos = new SugerenciaAmigo[cantUsuarios];
  int cantCandidatos = 0;

  for (int j = 0; j < tamañoMatriz; ++j) {
    registrarIteraciones(1);
    // Un candidato no puede ser el mismo usuario, un amigo ya existente
    // ni alguien con quien ya exista una solicitud de amistad pendiente
    if (j != indiceActual && !filaActual[j] &&
        !actual->tieneSolicitudPendienteCon(usuarios[j])) {
      const int comun = contarAmigosEnComunPorMatriz(indiceActual, j);
      // Solo sugiere si son "amigos de sus amigos directos" (amigos en común >
      // 0)
      if (comun > 0) {
        SugerenciaAmigo &cand = candidatos[cantCandidatos];
        cand.usuario = usuarios[j];
        cand.amigosEnComun = comun;
        ++cantCandidatos;
      }
    }
  }

  // Ordenamiento de mayor a menor según cantidad de amigos en común usando
  // referencias
  for (int i = 0; i < cantCandidatos; ++i) {
    for (int j = i + 1; j < cantCandidatos; ++j) {
      registrarIteraciones(1);
      SugerenciaAmigo &candI = candidatos[i];
      SugerenciaAmigo &candJ = candidatos[j];
      if (candJ.amigosEnComun > candI.amigosEnComun) {
        Usuario *tempUsuario = candI.usuario;
        int tempComun = candI.amigosEnComun;
        candI.usuario = candJ.usuario;
        candI.amigosEnComun = candJ.amigosEnComun;
        candJ.usuario = tempUsuario;
        candJ.amigosEnComun = tempComun;
      }
    }
  }

  // Limita a un máximo de K sugerencias
  const int maxK = (k > 20) ? 20 : ((k <= 0) ? 1 : k);
  const int limite = (cantCandidatos < maxK) ? cantCandidatos : maxK;

  for (int i = 0; i < limite; ++i) {
    sugeridos[i] = candidatos[i].usuario;
    amigosComun[i] = candidatos[i].amigosEnComun;
    ++cantSugeridos;
  }

  delete[] candidatos;
}

// ---------------------------------------------------------------------------
// Persistencia en Disco
// ---------------------------------------------------------------------------

bool Programa::guardarDatos(const char *rutaArchivo) const {
  const char *destino = nullptr;
  if (rutaDatabase[0] != '\0') {
    destino = rutaDatabase;
  } else if (rutaArchivo != nullptr && rutaArchivo[0] != '\0') {
    destino = rutaArchivo;
  } else {
    destino = "datos_udeabook.txt";
  }

  bool alMenosUno = false;
  for (int d = 0; d < 1; ++d) {
    std::ofstream archivo(destino);
    if (!archivo.is_open()) {
      continue;
    }
    alMenosUno = true;

    // 1. Guardar Usuarios
    archivo << "[USUARIOS]\n";
    archivo << cantUsuarios << '\n';
    for (int i = 0; i < cantUsuarios; ++i) {
      registrarIteraciones(1);
      Usuario *u = usuarios[i];
      archivo << u->getUsername() << ';' << u->getPassword() << ';'
              << u->getFechaNacimiento() << ';' << u->getCorreo() << ';'
              << u->getRutaFotoPerfil() << ';' << u->getCiudad() << ';'
              << u->getPais() << ';' << u->getFechaInscripcion() << '\n';
    }

    // 2. Guardar Amistades (Pares únicos usando puntero de fila)
    archivo << "[AMISTADES]\n";
    if (matrizAdyacencia != nullptr) {
      for (int i = 0; i < cantUsuarios; ++i) {
        const bool *filaI = matrizAdyacencia[i];
        const char *userI = usuarios[i]->getUsername();
        for (int j = i + 1; j < cantUsuarios; ++j) {
          registrarIteraciones(1);
          if (filaI[j]) {
            archivo << userI << ';' << usuarios[j]->getUsername() << '\n';
          }
        }
      }
    }
    archivo << "[FIN_AMISTADES]\n";

    // 3. Guardar Álbumes y Fotos
    archivo << "[ALBUMES]\n";
    for (int i = 0; i < cantUsuarios; ++i) {
      registrarIteraciones(1);
      Usuario *u = usuarios[i];
      for (int a = 0; a < u->getCantAlbumes(); ++a) {
        registrarIteraciones(1);
        Album *alb = u->getAlbumEn(a);
        archivo << u->getUsername() << ';' << alb->getTipoPrivacidad() << ';'
                << alb->getNombre() << ';' << alb->getRutaAlmacenamiento()
                << ';' << alb->getCantFotos() << '\n';

        for (int f = 0; f < alb->getCantFotos(); ++f) {
          registrarIteraciones(1);
          Foto *foto = alb->getFotoEn(f);
          archivo << foto->getRuta() << ';' << foto->getFechaCaptura() << ';'
                  << foto->getTamanoBytes() << ';' << foto->getLikes() << '\n';
        }
      }
    }
    archivo << "[FIN_ALBUMES]\n";

    // 4. Guardar Mensajes
    archivo << "[MENSAJES]\n";
    for (int i = 0; i < cantUsuarios; ++i) {
      registrarIteraciones(1);
      Usuario *u = usuarios[i];
      for (int n = 0; n < u->getCantMensajes(); ++n) {
        registrarIteraciones(1);
        Mensaje *m = u->getMensajeEn(n);
        if (m->getReceptor() == u) {
          archivo << (m->getEmisor() ? m->getEmisor()->getUsername()
                                     : "anonimo")
                  << ';' << u->getUsername() << ';' << m->getFechaHora() << ';'
                  << (m->estaLeido() ? 1 : 0) << ';' << m->getTexto() << '\n';
        }
      }
    }
    archivo << "[FIN_MENSAJES]\n";

    // 5. Guardar Solicitudes de Amistad
    archivo << "[SOLICITUDES]\n";
    for (int i = 0; i < cantUsuarios; ++i) {
      registrarIteraciones(1);
      Usuario *u = usuarios[i];
      for (int n = 0; n < u->getCantSolicitudes(); ++n) {
        registrarIteraciones(1);
        SolicitudAmistad *s = u->getSolicitudEn(n);
        if (s->getReceptor() == u) {
          archivo << (s->getEmisor() ? s->getEmisor()->getUsername()
                                     : "anonimo")
                  << ';' << u->getUsername() << ';' << s->getFechaHora() << ';'
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

  if (archivo.is_open()) {
    UtilidadesCadena::copiar(rutaDatabase, rutaArchivo, sizeof(rutaDatabase));
  } else {
    // Intenta buscar en ../ o ../../ si se ejecuta desde build/ o subcarpeta de Qt
    const char *prefijos[2] = {"../", "../../"};
    bool abierto = false;
    for (int pr = 0; pr < 2 && !abierto; ++pr) {
      char rutaAlt[256];
      int indice = 0;
      for (int k = 0; prefijos[pr][k] != '\0'; ++k) {
        rutaAlt[indice++] = prefijos[pr][k];
      }
      for (int p = 0; rutaArchivo[p] != '\0' && indice < 254; ++p) {
        rutaAlt[indice++] = rutaArchivo[p];
      }
      rutaAlt[indice] = '\0';
      archivo.open(rutaAlt);
      if (archivo.is_open()) {
        UtilidadesCadena::copiar(rutaDatabase, rutaAlt, sizeof(rutaDatabase));
        abierto = true;
      } else {
        archivo.clear();
      }
    }

    if (!abierto) {
      const char *respaldos[3] = {"datos_udeabook.txt", "../datos_udeabook.txt",
                                  "../../datos_udeabook.txt"};
      for (int r = 0; r < 3 && !abierto; ++r) {
        archivo.open(respaldos[r]);
        if (archivo.is_open()) {
          UtilidadesCadena::copiar(rutaDatabase, respaldos[r],
                                   sizeof(rutaDatabase));
          abierto = true;
        } else {
          archivo.clear();
        }
      }
    }

    if (!abierto) {
      return false;
    }
  }

  char linea[1024];
  const char *campos[8];

  // 1. Leer sección de usuarios
  while (archivo.getline(linea, sizeof(linea))) {
    int l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r')
      linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[USUARIOS]")) {
      break;
    }
  }

  int numUsuarios = 0;
  if (!(archivo >> numUsuarios)) {
    return false;
  }
  archivo.getline(linea, sizeof(linea)); // Consumir salto de línea

  if (numUsuarios > capUsuarios) {
    redimensionarUsuarios(numUsuarios + 50);
  }

  for (int i = 0; i < numUsuarios; ++i) {
    registrarIteraciones(1);
    if (!archivo.getline(linea, sizeof(linea)))
      break;

    if (dividirLineaPorPuntoYComa(linea, campos, 8) == 8) {
      Usuario *u = new Usuario(campos[0], campos[1], campos[2], campos[3],
                               campos[4], campos[5], campos[6], campos[7]);
      if (cantUsuarios == capUsuarios) {
        const int nuevaCap = (capUsuarios == 0) ? 4 : capUsuarios * 2;
        redimensionarUsuarios(nuevaCap);
      }
      usuarios[cantUsuarios] = u;
      ++cantUsuarios;
    }
  }

  // 2. Leer sección de amistades
  while (archivo.getline(linea, sizeof(linea))) {
    int l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r')
      linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[AMISTADES]")) {
      break;
    }
  }

  while (archivo.getline(linea, sizeof(linea))) {
    int l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r')
      linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[FIN_AMISTADES]")) {
      break;
    }
    if (dividirLineaPorPuntoYComa(linea, campos, 2) >= 2) {
      Usuario *uA = buscarUsuario(campos[0]);
      Usuario *uB = buscarUsuario(campos[1]);
      if (uA != nullptr && uB != nullptr) {
        uA->establecerAmistadMutua(uB);
      }
    }
  }

  // 3. Leer sección de álbumes y fotos
  while (archivo.getline(linea, sizeof(linea))) {
    int l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r')
      linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[ALBUMES]")) {
      break;
    }
  }

  while (archivo.getline(linea, sizeof(linea))) {
    int l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r')
      linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[FIN_ALBUMES]")) {
      break;
    }

    if (dividirLineaPorPuntoYComa(linea, campos, 5) >= 5) {
      Usuario *owner = buscarUsuario(campos[0]);
      Album *alb = new Album(campos[2], campos[3], campos[1]);

      int nFotos = 0;
      for (int k = 0; campos[4][k] != '\0'; ++k) {
        if (campos[4][k] >= '0' && campos[4][k] <= '9') {
          nFotos = nFotos * 10 + (campos[4][k] - '0');
        }
      }

      for (int f = 0; f < nFotos; ++f) {
        if (!archivo.getline(linea, sizeof(linea)))
          break;

        if (dividirLineaPorPuntoYComa(linea, campos, 4) >= 4) {
          int tamBytes = 1024;
          int numTam = 0;
          for (int tk = 0; campos[2][tk] != '\0'; ++tk) {
            if (campos[2][tk] >= '0' && campos[2][tk] <= '9') {
              numTam = numTam * 10 + (campos[2][tk] - '0');
            }
          }
          if (numTam > 0)
            tamBytes = numTam;

          int nLikes = 0;
          for (int lk = 0; campos[3][lk] != '\0'; ++lk) {
            if (campos[3][lk] >= '0' && campos[3][lk] <= '9') {
              nLikes = nLikes * 10 + (campos[3][lk] - '0');
            }
          }

          Foto *foto = new Foto(campos[0], campos[1], tamBytes);
          for (int l = 0; l < nLikes; ++l) {
            foto->darLike();
          }
          alb->agregarFoto(foto);
        }
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
    int l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r')
      linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[MENSAJES]")) {
      break;
    }
  }

  while (archivo.getline(linea, sizeof(linea))) {
    int l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r')
      linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[FIN_MENSAJES]")) {
      break;
    }

    if (dividirLineaPorPuntoYComa(linea, campos, 5) >= 5) {
      Usuario *uEmisor = buscarUsuario(campos[0]);
      Usuario *uReceptor = buscarUsuario(campos[1]);
      if (uEmisor != nullptr && uReceptor != nullptr) {
        Mensaje *msgRecibido =
            new Mensaje(uEmisor, uReceptor, campos[2], campos[4]);
        if (campos[3][0] == '1') {
          msgRecibido->marcarComoLeido();
        }
        uReceptor->recibirMensaje(msgRecibido);
      }
    }
  }

  // 5. Leer sección de solicitudes de amistad (si existe)
  while (archivo.getline(linea, sizeof(linea))) {
    int l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r')
      linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[SOLICITUDES]")) {
      break;
    }
  }

  while (archivo.getline(linea, sizeof(linea))) {
    int l = UtilidadesCadena::longitud(linea);
    if (l > 0 && linea[l - 1] == '\r')
      linea[l - 1] = '\0';
    if (UtilidadesCadena::sonIguales(linea, "[FIN_SOLICITUDES]")) {
      break;
    }

    if (dividirLineaPorPuntoYComa(linea, campos, 4) >= 4) {
      Usuario *uEmisor = buscarUsuario(campos[0]);
      Usuario *uReceptor = buscarUsuario(campos[1]);
      if (uEmisor != nullptr && uReceptor != nullptr) {
        SolicitudAmistad *solRecibida =
            new SolicitudAmistad(uEmisor, uReceptor, campos[2]);
        if (campos[3][0] == '1') {
          solRecibida->aceptar();
        } else if (campos[3][0] == '2') {
          solRecibida->rechazar();
        }
        uReceptor->recibirSolicitud(solRecibida);
      }
    }
  }

  construirMatrizAdyacencia();
  archivo.close();
  return true;
}

// Métricas de Consumo de Recursos

void Programa::reiniciarIteraciones() { contadorIteraciones = 0; }

void Programa::registrarIteraciones(int cantidad) const {
  contadorIteraciones += cantidad;
}

int Programa::getIteraciones() const { return contadorIteraciones; }

int Programa::calcularMemoriaTotal(int memoriaLocal) const {
  int total = sizeof(Programa);
  total += capUsuarios * sizeof(Usuario *);

  // Matriz de adyacencia
  if (matrizAdyacencia != nullptr) {
    total += tamañoMatriz * sizeof(bool *);
    total += tamañoMatriz * tamañoMatriz * sizeof(bool);
  }

  // Memoria de cada usuario y sus estructuras hijas
  for (int i = 0; i < cantUsuarios; ++i) {
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
    for (int a = 0; a < u->getCantAlbumes(); ++a) {
      Album *alb = u->getAlbumEn(a);
      total += sizeof(Album);
      total += UtilidadesCadena::longitud(alb->getNombre()) + 1;
      total += UtilidadesCadena::longitud(alb->getRutaAlmacenamiento()) + 1;
      total += alb->getCapFotos() * sizeof(Foto *);

      for (int f = 0; f < alb->getCantFotos(); ++f) {
        Foto *foto = alb->getFotoEn(f);
        total += sizeof(Foto);
        total += UtilidadesCadena::longitud(foto->getRuta()) + 1;
        total += UtilidadesCadena::longitud(foto->getFechaCaptura()) + 1;
      }
    }

    // Mensajes recibidos
    total += u->getCantMensajes() * sizeof(Mensaje *);
    for (int m = 0; m < u->getCantMensajes(); ++m) {
      Mensaje *men = u->getMensajeEn(m);
      total += sizeof(Mensaje);
      total += UtilidadesCadena::longitud(men->getFechaHora()) + 1;
      total += UtilidadesCadena::longitud(men->getTexto()) + 1;
    }

    // Solicitudes de amistad recibidas
    total += u->getCantSolicitudes() * sizeof(SolicitudAmistad *);
    for (int s = 0; s < u->getCantSolicitudes(); ++s) {
      SolicitudAmistad *sol = u->getSolicitudEn(s);
      total += sizeof(SolicitudAmistad);
      total += UtilidadesCadena::longitud(sol->getFechaHora()) + 1;
    }
  }

  // Suma las variables locales y parámetros por valor reportados por el
  // llamador
  total += memoriaLocal;
  return total;
}

void Programa::reportarConsumo(const char *nombreFuncionalidad,
                               int memoriaLocal) const {
  const int totalBytes = calcularMemoriaTotal(memoriaLocal);
  const double totalKB = static_cast<double>(totalBytes) / 1024.0;

  std::cout
      << "\n============================================================\n";
  std::cout << "[METRICAS DE EFICIENCIA]\n";
  std::cout << "Funcionalidad ejecutada: "
            << (nombreFuncionalidad ? nombreFuncionalidad : "General") << '\n';
  std::cout << "-> Iteraciones requeridas (directas e indirectas): "
            << contadorIteraciones << '\n';
  std::cout << "-> Memoria RAM total consumida en este instante: " << totalBytes
            << " bytes (" << totalKB << " KB)\n";
  std::cout << "============================================================\n";
}
