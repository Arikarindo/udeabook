

#include "Album.h"
#include "Foto.h"
#include "Mensaje.h"
#include "Programa.h"
#include "SolicitudAmistad.h"
#include "Usuario.h"
#include "UtilidadesCadena.h"
#include <iostream>

// Genera una marca de fecha y hora en formato "AAAA-MM-DD HH:MM"
static void obtenerFechaHoraActual(char *buffer, int maxLen) {
  if (buffer == nullptr || maxLen < 17) {
    return;
  }
  static int minutosSesion = 0;
  const int hora = 10 + (minutosSesion / 60) % 14;
  const int minuto = minutosSesion % 60;
  ++minutosSesion;

  UtilidadesCadena::copiar(buffer, "2026-10-18 00:00", maxLen);
  buffer[11] = static_cast<char>('0' + (hora / 10));
  buffer[12] = static_cast<char>('0' + (hora % 10));
  buffer[14] = static_cast<char>('0' + (minuto / 10));
  buffer[15] = static_cast<char>('0' + (minuto % 10));
}

// Función para limpiar el buffer de entrada de cin
static void limpiarBufferCin() {
  while (std::cin.get() != '\n') {
    // Descarta caracteres residuales
  }
}

// ---------------------------------------------------------------------------
// Submenús de Funcionalidades (Paso por referencia para evitar copias)
// ---------------------------------------------------------------------------

static void mostrar3MejoresFotos(const Usuario &usuario, Programa &app) {
  const char *rutas[3] = {nullptr, nullptr, nullptr};
  int cant = 0;
  usuario.seleccionar3MejoresAmigos(rutas, cant);

  // Contabiliza las iteraciones realizadas en la selección ponderada
  const int nAmigos = usuario.getCantAmigos();
  if (nAmigos <= 3) {
    app.registrarIteraciones(nAmigos);
  } else {
    // Cálculo de amigos en común + 3 rondas de ruleta ponderada
    app.registrarIteraciones(nAmigos * nAmigos + 3 * (2 * nAmigos));
  }

  std::cout << "\n--- FOTOS DE PERFIL DE TUS MEJORES AMIGOS ---\n";
  if (cant == 0) {
    std::cout << "(Aun no tienes amigos agregados en la plataforma)\n";
  } else {
    for (int i = 0; i < cant; ++i) {
      app.registrarIteraciones(1);
      std::cout << " [" << (i + 1) << "] " << (rutas[i] ? rutas[i] : "")
                << '\n';
    }
  }
  std::cout << "---------------------------------------------\n";
}

static void accionEnviarSolicitudAmistad(Programa &app,
                                         Usuario &usuarioActual) {
  char usernameDestino[128];
  std::cout << "\nIngrese el nombre de usuario a quien enviar solicitud: ";
  std::cin >> usernameDestino;

  Usuario *dest = app.buscarUsuario(usernameDestino);
  if (dest == nullptr) {
    std::cout << "El usuario @" << usernameDestino
              << " no existe en la plataforma.\n";
  } else if (dest == &usuarioActual) {
    std::cout << "No puedes enviarte una solicitud a ti mismo.\n";
  } else {
    app.registrarIteraciones(usuarioActual.getCantAmigos());
    if (usuarioActual.esAmigoDe(dest)) {
      std::cout << "Ya eres amigo de @" << usernameDestino << ".\n";
    } else {
      app.registrarIteraciones(usuarioActual.getCantSolicitudes() +
                               dest->getCantSolicitudes());
      if (usuarioActual.tieneSolicitudPendienteCon(dest)) {
        std::cout << "Ya existe una solicitud pendiente entre ustedes.\n";
      } else {
        char fechaActual[32];
        obtenerFechaHoraActual(fechaActual, sizeof(fechaActual));
        usuarioActual.enviarSolicitudAmistad(dest, fechaActual);
        std::cout << "Solicitud de amistad enviada con exito a @"
                  << usernameDestino << " (" << fechaActual << ").\n";
      }
    }
  }
  app.reportarConsumo("Enviar Solicitud de Amistad", sizeof(usernameDestino));
  app.reiniciarIteraciones();
}

static void accionGestionarSolicitudesRecibidas(Programa &app,
                                                Usuario &usuarioActual) {
  int pendientes[50];
  int cantPendientes = 0;

  std::cout << "\n--- SOLICITUDES DE AMISTAD RECIBIDAS ---\n";
  for (int i = 0; i < usuarioActual.getCantSolicitudes(); ++i) {
    app.registrarIteraciones(1);
    SolicitudAmistad *sol = usuarioActual.getSolicitudEn(i);
    if (sol->estaPendiente() && sol->getReceptor() == &usuarioActual) {
      pendientes[cantPendientes] = i;
      std::cout << "[" << (cantPendientes + 1) << "] De: @"
                << (sol->getEmisor() ? sol->getEmisor()->getUsername()
                                     : "desconocido")
                << " | Fecha: " << sol->getFechaHora() << '\n';
      ++cantPendientes;
      if (cantPendientes >= 50)
        break;
    }
  }

  if (cantPendientes == 0) {
    std::cout << "(No tienes solicitudes de amistad pendientes)\n";
  } else {
    std::cout
        << "\nIngrese el numero de solicitud a responder (0 para cancelar): ";
    int sel = 0;
    if (std::cin >> sel && sel > 0 && sel <= cantPendientes) {
      SolicitudAmistad *sol =
          usuarioActual.getSolicitudEn(pendientes[sel - 1]);

      std::cout << "¿Desea (1) Aceptar o (2) Rechazar?: ";
      int accion = 0;
      if (std::cin >> accion) {
        if (accion == 1) {
          sol->aceptar();
          app.registrarIteraciones(usuarioActual.getCantAmigos() +
                                   (sol->getEmisor()
                                        ? sol->getEmisor()->getCantAmigos()
                                        : 0));
          usuarioActual.establecerAmistadMutua(sol->getEmisor());
          app.registrarAmistadEnMatriz(&usuarioActual, sol->getEmisor());
          std::cout << "¡Solicitud aceptada! Ahora son amigos.\n";
        } else if (accion == 2) {
          sol->rechazar();
          std::cout << "Solicitud rechazada.\n";
        }
      }
    }
  }
  app.reportarConsumo("Gestionar Solicitudes de Amistad", sizeof(pendientes));
  app.reiniciarIteraciones();
}

static void menuCarruselFotos(Album &album, Programa &app) {
  if (album.getCantFotos() == 0) {
    std::cout << "El album '" << album.getNombre()
              << "' no tiene fotos registradas.\n";
    return;
  }

  album.reiniciarCarrusel();
  int accion = 0;

  do {
    Foto *actual = album.getFotoActual();
    std::cout
        << "\n------------------------------------------------------------\n";
    std::cout << "FOTO ACTUAL (" << (album.getIndiceCarrusel() + 1) << " de "
              << album.getCantFotos() << ")\n";
    std::cout << "Ruta externa: ";
    if (actual != nullptr)
      actual->mostrarRuta();
    if (actual != nullptr) {
      std::cout << "Fecha de captura: " << actual->getFechaCaptura()
                << " | Tamaño: " << actual->getTamanoBytes() << " bytes"
                << " | Likes: " << actual->getLikes() << '\n';
    }
    std::cout
        << "------------------------------------------------------------\n";
    std::cout << "1. Siguiente foto\n";
    std::cout << "2. Previa foto\n";
    std::cout << "3. Dar Like\n";
    std::cout << "4. Quitar Like\n";
    std::cout << "5. Salir del album\n";
    std::cout << "Seleccione accion: ";
    if (!(std::cin >> accion)) {
      limpiarBufferCin();
      continue;
    }

    app.registrarIteraciones(1);
    if (accion == 1) {
      album.siguienteFoto();
    } else if (accion == 2) {
      album.previaFoto();
    } else if (accion == 3) {
      if (actual != nullptr) {
        actual->darLike();
        std::cout << "¡Le diste Me Gusta a la foto!\n";
      }
    } else if (accion == 4) {
      if (actual != nullptr) {
        if (actual->getLikes() > 0) {
          actual->quitarLike();
          std::cout << "Le quitaste el Me Gusta a la foto.\n";
        } else {
          std::cout
              << "La foto ya tiene 0 Me Gusta, no se puede disminuir mas.\n";
        }
      }
    }

    app.reportarConsumo("Interaccion Carrusel Fotos", sizeof(accion));
    app.reiniciarIteraciones();

  } while (accion != 5);
}

static void menuVisitarPerfilAmigo(Programa &app, Usuario &usuarioActual) {
  std::cout
      << "\n============================================================\n";
  std::cout << "                 VISITAR PERFIL Y ALBUMES\n";
  std::cout << "============================================================\n";
  std::cout << "1. Seleccionar de mi lista de amigos ("
            << usuarioActual.getCantAmigos() << " amigos)\n";
  std::cout << "2. Buscar cualquier usuario por nombre (amigos o no amigos)\n";
  std::cout << "0. Volver al menu principal\n";
  std::cout << "Seleccione una opcion: ";

  int modo = 0;
  if (!(std::cin >> modo) || modo <= 0) {
    if (modo < 0) {
      limpiarBufferCin();
    }
    return;
  }

  Usuario *amigo = nullptr;

  if (modo == 1) {
    if (usuarioActual.getCantAmigos() == 0) {
      std::cout << "\n(Aun no tienes amigos agregados en tu lista de amigos)\n";
      app.reportarConsumo("Visitar Perfil Usuario", sizeof(modo));
      app.reiniciarIteraciones();
      return;
    }

    std::cout
        << "\n============================================================\n";
    std::cout << "                 TUS AMIGOS DISPONIBLES\n";
    std::cout
        << "============================================================\n";
    for (int i = 0; i < usuarioActual.getCantAmigos(); ++i) {
      app.registrarIteraciones(1);
      Usuario *a = usuarioActual.getAmigoEn(i);
      std::cout << " [" << (i + 1) << "] @" << a->getUsername() << " ("
                << a->getCiudad() << ", " << a->getPais() << " | "
                << a->getCantAlbumes() << " albumes)\n";
    }
    std::cout
        << "============================================================\n";
    std::cout << "Seleccione el numero del amigo a visitar (0 para volver): ";
    int seleccion = 0;
    if (!(std::cin >> seleccion) || seleccion <= 0 ||
        seleccion > usuarioActual.getCantAmigos()) {
      if (seleccion < 0) {
        limpiarBufferCin();
      }
      return;
    }

    amigo = usuarioActual.getAmigoEn(seleccion - 1);
  } else if (modo == 2) {
    char nombreBuscado[128];
    std::cout << "\nIngrese el nombre de usuario a visitar: ";
    if (!(std::cin >> nombreBuscado)) {
      limpiarBufferCin();
      return;
    }
    amigo = app.buscarUsuario(nombreBuscado);
    if (amigo == nullptr) {
      std::cout << "El usuario @" << nombreBuscado
                << " no existe en la plataforma.\n";
      app.reportarConsumo("Visitar Perfil Usuario",
                          sizeof(modo) + sizeof(nombreBuscado));
      app.reiniciarIteraciones();
      return;
    }
  } else {
    return;
  }

  const bool esMismoUsuario = (amigo == &usuarioActual);
  const bool sonAmigos = usuarioActual.esAmigoDe(amigo);

  std::cout
      << "\n============================================================\n";
  std::cout << "   PERFIL DE @" << amigo->getUsername() << " ("
            << (esMismoUsuario ? "Tu perfil"
                               : (sonAmigos ? "Amigo" : "No es tu amigo"))
            << ")\n";
  std::cout << "============================================================\n";
  std::cout << "Foto de perfil: " << amigo->getRutaFotoPerfil() << '\n';
  std::cout << "Ciudad: " << amigo->getCiudad()
            << " | Pais: " << amigo->getPais() << '\n';

  // Listar álbumes accesibles según relación (Público, Amigos o Privado)
  Album *visibles[50];
  int cantVisibles = 0;

  std::cout << "\nAlbumes disponibles:\n";
  for (int i = 0; i < amigo->getCantAlbumes(); ++i) {
    app.registrarIteraciones(1 + amigo->getCantAmigos());
    Album *alb = amigo->getAlbumEn(i);
    if (alb->esAccesiblePorUsuario(&usuarioActual, amigo)) {
      visibles[cantVisibles] = alb;
      std::cout << " [" << (cantVisibles + 1) << "] " << alb->getNombre()
                << " (" << alb->getTipoPrivacidad() << ") - "
                << alb->getCantFotos() << " fotos\n";
      ++cantVisibles;
      if (cantVisibles >= 50)
        break;
    }
  }

  app.reportarConsumo("Visitar Perfil Usuario y Listar Albumes",
                      sizeof(visibles) + sizeof(modo));
  app.reiniciarIteraciones();

  if (cantVisibles == 0) {
    std::cout << "(No hay albumes visibles para mostrar)\n";
  } else {
    int sel = 0;
    do {
      std::cout
          << "\nSeleccione el numero de album a visualizar (0 para salir): ";
      if (!(std::cin >> sel)) {
        limpiarBufferCin();
        sel = 0;
        break;
      }
      if (sel > 0 && sel <= cantVisibles) {
        menuCarruselFotos(*visibles[sel - 1], app);
      }
    } while (sel > 0 && sel <= cantVisibles);
  }
}

static void menuSugerenciasAmistad(Programa &app, Usuario &usuarioActual) {
  int k = 0;
  std::cout << "\nIngrese la cantidad K de amigos sugeridos a consultar (entre "
               "1 y 20): ";
  if (!(std::cin >> k) || k < 1) {
    k = 5;
  }

  Usuario *sugeridos[20];
  int coincidencias[20];
  int cant = 0;

  app.reiniciarIteraciones();
  app.sugerirAmigos(&usuarioActual, k, sugeridos, coincidencias, cant);

  std::cout << "\n--- TOP " << cant << " SUGERENCIAS DE AMISTAD ---\n";
  if (cant == 0) {
    std::cout << "(No se encontraron sugerencias con amigos en comun)\n";
  } else {
    for (int i = 0; i < cant; ++i) {
      app.registrarIteraciones(1);
      std::cout << " [" << (i + 1) << "] @" << sugeridos[i]->getUsername()
                << " (" << sugeridos[i]->getCiudad() << ", "
                << sugeridos[i]->getPais() << " | " << coincidencias[i]
                << " amigos en comun)\n";
    }

    std::cout << "\n¿Desea enviar solicitud a alguno de ellos? (Ingrese el "
                 "numero o 0 para omitir): ";
    int sel = 0;
    if (std::cin >> sel && sel > 0 && sel <= cant) {
      Usuario *elegido = sugeridos[sel - 1];
      if (usuarioActual.esAmigoDe(elegido)) {
        std::cout << "Ya eres amigo de @" << elegido->getUsername() << ".\n";
      } else if (usuarioActual.tieneSolicitudPendienteCon(elegido)) {
        std::cout << "Ya existe una solicitud pendiente con @"
                  << elegido->getUsername() << ".\n";
      } else {
        char fechaActual[32];
        obtenerFechaHoraActual(fechaActual, sizeof(fechaActual));
        usuarioActual.enviarSolicitudAmistad(elegido, fechaActual);
        std::cout << "Solicitud enviada exitosamente a @"
                  << elegido->getUsername() << " (" << fechaActual << ").\n";
      }
    }
  }

  app.reportarConsumo("Ver Sugerencias de Amistad",
                      sizeof(sugeridos) + sizeof(coincidencias));
  app.reiniciarIteraciones();
}

static void menuNotificaciones(Programa &app, Usuario &usuarioActual) {
  int opcion = 0;
  do {
    std::cout
        << "\n============================================================\n";
    std::cout << "                 CENTRO DE NOTIFICACIONES\n";
    std::cout
        << "============================================================\n";
    std::cout << "1. Ver y responder solicitudes de amistad recibidas\n";
    std::cout << "2. Ver sugerencias de amistad\n";
    std::cout << "3. Leer nuevos mensajes recibidos\n";
    std::cout << "4. Buscar mensajes por rango de fecha\n";
    std::cout << "5. Enviar mensaje a un amigo\n";
    std::cout << "6. Volver al menu principal\n";
    std::cout << "Seleccione una opcion: ";
    if (!(std::cin >> opcion)) {
      limpiarBufferCin();
      continue;
    }

    if (opcion == 1) {
      accionGestionarSolicitudesRecibidas(app, usuarioActual);

    } else if (opcion == 2) {
      menuSugerenciasAmistad(app, usuarioActual);

    } else if (opcion == 3) {
      std::cout << "\n--- NUEVOS MENSAJES RECIBIDOS ---\n";
      int nuevos = 0;

      for (int i = 0; i < usuarioActual.getCantMensajes(); ++i) {
        app.registrarIteraciones(1);
        Mensaje *m = usuarioActual.getMensajeEn(i);
        if (!m->estaLeido() && m->getReceptor() == &usuarioActual) {
          std::cout << "De: @"
                    << (m->getEmisor() ? m->getEmisor()->getUsername()
                                       : "anonimo")
                    << " ";
          m->mostrarEnPantalla();
          m->marcarComoLeido();
          ++nuevos;
        }
      }

      if (nuevos == 0) {
        std::cout << "(No tienes mensajes nuevos sin leer)\n";
      } else {
        std::cout << "-> Se marcaron " << nuevos << " mensajes como leidos.\n";
      }
      app.reportarConsumo("Leer Nuevos Mensajes", sizeof(nuevos));
      app.reiniciarIteraciones();

    } else if (opcion == 4) {
      char fechaIni[12];
      char fechaFin[12];
      std::cout << "Ingrese fecha inicial (AAAA-MM-DD): ";
      std::cin >> fechaIni;
      std::cout << "Ingrese fecha final   (AAAA-MM-DD): ";
      std::cin >> fechaFin;

      if (UtilidadesCadena::comparar(fechaIni, fechaFin) > 0) {
        std::cout << "Error: La fecha inicial no puede ser mayor que la fecha "
                     "final.\n";
      } else {
        std::cout << "\n--- MENSAJES RECIBIDOS EN EL RANGO [" << fechaIni
                  << " a " << fechaFin << "] ---\n";
        int encontrados = 0;

        for (int i = 0; i < usuarioActual.getCantMensajes(); ++i) {
          app.registrarIteraciones(1);
          Mensaje *m = usuarioActual.getMensajeEn(i);
          if (m->getReceptor() == &usuarioActual) {
            app.registrarIteraciones(10); // Comparación de fecha en estaEnRango
            if (m->estaEnRango(fechaIni, fechaFin)) {
              std::cout << "De: @"
                        << (m->getEmisor() ? m->getEmisor()->getUsername()
                                           : "anonimo")
                        << " ";
              m->mostrarEnPantalla();
              ++encontrados;
            }
          }
        }

        if (encontrados == 0) {
          std::cout << "(No se encontraron mensajes en el rango indicado)\n";
        }
      }
      app.reportarConsumo("Buscar Mensajes por Rango de Fecha",
                          sizeof(fechaIni) + sizeof(fechaFin));
      app.reiniciarIteraciones();

    } else if (opcion == 5) {
      if (usuarioActual.getCantAmigos() == 0) {
        std::cout
            << "\n(Aun no tienes amigos en tu lista para enviarles mensajes)\n";
        continue;
      }

      std::cout << "\n--- TUS AMIGOS DISPONIBLES PARA MENSAJE ---\n";
      for (int i = 0; i < usuarioActual.getCantAmigos(); ++i) {
        app.registrarIteraciones(1);
        Usuario *a = usuarioActual.getAmigoEn(i);
        std::cout << " [" << (i + 1) << "] @" << a->getUsername() << '\n';
      }
      std::cout
          << "Seleccione el numero del amigo destinatario (0 para cancelar): ";
      int selAmigo = 0;
      if (!(std::cin >> selAmigo) || selAmigo <= 0 ||
          selAmigo > usuarioActual.getCantAmigos()) {
        continue;
      }

      Usuario *dest = usuarioActual.getAmigoEn(selAmigo - 1);

      std::cout << "Ingrese el texto del mensaje para @" << dest->getUsername()
                << " (hasta 512 caracteres):\n> ";
      limpiarBufferCin();
      char texto[513];
      std::cin.getline(texto, sizeof(texto));

      bool soloEspacios = true;
      for (int i = 0; texto[i] != '\0'; ++i) {
        app.registrarIteraciones(1);
        if (texto[i] != ' ' && texto[i] != '\t' && texto[i] != '\r') {
          soloEspacios = false;
          break;
        }
      }

      if (soloEspacios) {
        std::cout
            << "No se puede enviar un mensaje vacio o de solo espacios.\n";
      } else {
        char fechaActual[32];
        obtenerFechaHoraActual(fechaActual, sizeof(fechaActual));
        app.registrarIteraciones(UtilidadesCadena::longitud(texto));
        usuarioActual.enviarMensaje(dest, texto, fechaActual);
        std::cout << "Mensaje enviado exitosamente a @" << dest->getUsername()
                  << " (" << fechaActual << ").\n";
      }
      app.reportarConsumo("Enviar Mensaje Privado", sizeof(texto));
      app.reiniciarIteraciones();
    }
  } while (opcion != 6);
}

// ---------------------------------------------------------------------------
// Función Principal (Punto de Entrada)
// ---------------------------------------------------------------------------

int main() {
  Programa app;
  const char *archivoAlmacenamiento = "dataset_udeabook_prueba.txt";

  // Carga automática desde almacenamiento permanente
  app.reiniciarIteraciones();
  if (!app.cargarDatos(archivoAlmacenamiento)) {
    std::cerr << "Error: No se pudo abrir o cargar el archivo de datos '"
              << archivoAlmacenamiento << "'.\n";
    return 1;
  }
  app.reportarConsumo("Carga Inicial desde Almacenamiento Permanente",
                      sizeof(archivoAlmacenamiento));
  app.reiniciarIteraciones();

  std::cout
      << "\n************************************************************\n";
  std::cout << "             BIENVENIDO A UDE@BOOK - CONSOLA\n";
  std::cout << "************************************************************\n";

  Usuario *usuarioActual = nullptr;

  // Ciclo de Autenticación
  while (usuarioActual == nullptr) {
    char user[128];
    char pass[128];

    std::cout << "\n--- INICIO DE SESION ---\n";
    std::cout
        << "(Ejemplo para prueba rapida: usuario 'juanito76', clave '1234')\n";
    std::cout << "Usuario: ";
    if (!(std::cin >> user)) {
      break;
    }
    std::cout << "Contrasena: ";
    if (!(std::cin >> pass)) {
      break;
    }

    app.reiniciarIteraciones();
    usuarioActual = app.autenticarUsuario(user, pass);

    if (usuarioActual == nullptr) {
      std::cout << "Error: Credenciales invalidas. Intente nuevamente.\n";
    } else {
      std::cout << "\n¡Bienvenido @" << usuarioActual->getUsername() << "!\n";
    }

    app.reportarConsumo("Autenticacion e Inicio de Sesion",
                        sizeof(user) + sizeof(pass));
    app.reiniciarIteraciones();
  }

  if (usuarioActual == nullptr) {
    return 0;
  }

  // Visualización inicial de las 3 mejores fotos de amigos
  mostrar3MejoresFotos(*usuarioActual, app);
  app.reiniciarIteraciones();

  int opcion = 0;
  do {
    std::cout
        << "\n============================================================\n";
    std::cout << "                   MENU PRINCIPAL\n";
    std::cout
        << "============================================================\n";
    std::cout << "1. Actualizar set de 3 fotos de mejores amigos\n";
    std::cout << "2. Enviar solicitud de amistad\n";
    std::cout << "3. Visitar perfil de un usuario (amigos o cualquier usuario) "
                 "y ver albumes/fotos\n";
    std::cout << "4. Notificaciones (Solicitudes, sugerencias y mensajes)\n";
    std::cout << "5. Cerrar sesion y salir\n";
    std::cout << "Seleccione una opcion: ";

    if (!(std::cin >> opcion)) {
      limpiarBufferCin();
      continue;
    }

    switch (opcion) {
    case 1:
      mostrar3MejoresFotos(*usuarioActual, app);
      app.reportarConsumo("Actualizar Set de Mejores Amigos", 0);
      app.reiniciarIteraciones();
      break;

    case 2:
      accionEnviarSolicitudAmistad(app, *usuarioActual);
      break;

    case 3:
      menuVisitarPerfilAmigo(app, *usuarioActual);
      break;

    case 4:
      menuNotificaciones(app, *usuarioActual);
      break;

    case 5:
      std::cout << "\nGuardando cambios en almacenamiento permanente...\n";
      app.reiniciarIteraciones();
      app.guardarDatos(archivoAlmacenamiento);
      app.reportarConsumo("Guardado y Cierre de Almacenamiento Permanente",
                          sizeof(archivoAlmacenamiento));
      app.reiniciarIteraciones();
      std::cout << "¡Hasta pronto, @" << usuarioActual->getUsername() << "!\n";
      break;

    default:
      std::cout << "Opcion invalida. Intente de nuevo.\n";
      break;
    }

  } while (opcion != 5);

  return 0;
}
