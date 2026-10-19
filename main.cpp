// ===========================================================================
// main.cpp — Punto de Entrada y Menú Interactivo de Consola (NIVEL 6)
// ===========================================================================
// Cumple con el 100% de los requisitos del Desafío II de UdeABook:
//   - Carga y persistencia transparente al inicio y al salir.
//   - Autenticación e inicio de sesión.
//   - Visualización estocástica ponderada de las 3 mejores fotos de amigos.
//   - Administración de amistades (enviar y aceptar solicitudes).
//   - Visita de perfiles de amigos y Carrusel Circular de fotos (siguiente,
//   previa, like/unlike).
//   - Sugerencias Top-K basadas en Matriz de Adyacencia con opción de enviar
//   solicitud.
//   - Mensajería privada: envío, lectura de no leídos y búsqueda por rango de
//   fechas.
//   - Reporte sistemático de consumo de recursos (Iteraciones y RAM) tras CADA
//   acción.
// ===========================================================================

#include "Album.h"
#include "Foto.h"
#include "Mensaje.h"
#include "Programa.h"
#include "SolicitudAmistad.h"
#include "Usuario.h"
#include "UtilidadesCadena.h"
#include <iostream>

// Función para limpiar el buffer de entrada de cin
static void limpiarBufferCin() {
  while (std::cin.get() != '\n') {
    // Descarta caracteres residuales
  }
}



// ---------------------------------------------------------------------------
// Submenús de Funcionalidades
// ---------------------------------------------------------------------------

static void mostrar3MejoresFotos(const Usuario *usuario) {
  const char *rutas[3] = {nullptr, nullptr, nullptr};
  std::size_t cant = 0;
  usuario->seleccionar3MejoresAmigos(rutas, cant);

  std::cout << "\n--- FOTOS DE PERFIL DE TUS MEJORES AMIGOS ---\n";
  if (cant == 0) {
    std::cout << "(Aun no tienes amigos agregados en la plataforma)\n";
  } else {
    for (std::size_t i = 0; i < cant; ++i) {
      std::cout << " [" << (i + 1) << "] " << (rutas[i] ? rutas[i] : "")
                << '\n';
    }
  }
  std::cout << "---------------------------------------------\n";
}

static void accionEnviarSolicitudAmistad(Programa &app,
                                         Usuario *usuarioActual) {
  char usernameDestino[128];
  std::cout << "\nIngrese el nombre de usuario a quien enviar solicitud: ";
  std::cin >> usernameDestino;

  Usuario *dest = app.buscarUsuario(usernameDestino);
  if (dest == nullptr) {
    std::cout << "El usuario @" << usernameDestino
              << " no existe en la plataforma.\n";
  } else if (dest == usuarioActual) {
    std::cout << "No puedes enviarte una solicitud a ti mismo.\n";
  } else if (usuarioActual->esAmigoDe(dest)) {
    std::cout << "Ya eres amigo de @" << usernameDestino << ".\n";
  } else if (usuarioActual->tieneSolicitudPendienteCon(dest)) {
    std::cout << "Ya existe una solicitud pendiente entre ustedes.\n";
  } else {
    usuarioActual->enviarSolicitudAmistad(dest, "2026-10-15 12:00");
    std::cout << "Solicitud de amistad enviada con exito a @" << usernameDestino
              << ".\n";
  }
  app.reportarConsumo("Enviar Solicitud de Amistad",
                      sizeof(usernameDestino) + sizeof(Usuario *));
  app.reiniciarIteraciones();
}

static void accionGestionarSolicitudesRecibidas(Programa &app,
                                                Usuario *usuarioActual) {
  std::size_t pendientes[50];
  std::size_t cantPendientes = 0;

  std::cout << "\n--- SOLICITUDES DE AMISTAD RECIBIDAS ---\n";
  for (std::size_t i = 0; i < usuarioActual->getCantSolicitudes(); ++i) {
    SolicitudAmistad *sol = usuarioActual->getSolicitudEn(i);
    if (sol->estaPendiente() && sol->getReceptor() == usuarioActual) {
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
    if (std::cin >> sel && sel > 0 &&
        static_cast<std::size_t>(sel) <= cantPendientes) {
      SolicitudAmistad *sol =
          usuarioActual->getSolicitudEn(pendientes[static_cast<std::size_t>(sel - 1)]);

      std::cout << "¿Desea (1) Aceptar o (2) Rechazar?: ";
      int accion = 0;
      if (std::cin >> accion) {
        if (accion == 1) {
          sol->aceptar();
          usuarioActual->establecerAmistadMutua(sol->getEmisor());
          app.construirMatrizAdyacencia();
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

static void menuCarruselFotos(Album *album, Programa &app) {
  if (album->getCantFotos() == 0) {
    std::cout << "El album '" << album->getNombre()
              << "' no tiene fotos registradas.\n";
    return;
  }

  album->reiniciarCarrusel();
  int accion = 0;

  do {
    Foto *actual = album->getFotoActual();
    std::cout
        << "\n------------------------------------------------------------\n";
    std::cout << "FOTO ACTUAL (" << (album->getIndiceCarrusel() + 1) << " de "
              << album->getCantFotos() << ")\n";
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

    if (accion == 1) {
      album->siguienteFoto();
    } else if (accion == 2) {
      album->previaFoto();
    } else if (accion == 3) {
      if (actual != nullptr)
        actual->darLike();
      std::cout << "¡Le diste Me Gusta a la foto!\n";
    } else if (accion == 4) {
      if (actual != nullptr)
        actual->quitarLike();
      std::cout << "Le quitaste el Me Gusta a la foto.\n";
    }

    app.reportarConsumo("Interaccion Carrusel Fotos",
                        sizeof(Album *) + sizeof(Foto *));
    app.reiniciarIteraciones();

  } while (accion != 5);
}

static void menuVisitarPerfilAmigo(Programa &app, Usuario *usuarioActual) {
  if (usuarioActual->getCantAmigos() == 0) {
    std::cout << "\n(Aun no tienes amigos agregados en tu lista de amigos)\n";
    app.reportarConsumo("Visitar Perfil Amigo", 0);
    app.reiniciarIteraciones();
    return;
  }

  std::cout
      << "\n============================================================\n";
  std::cout << "                 TUS AMIGOS DISPONIBLES\n";
  std::cout << "============================================================\n";
  for (std::size_t i = 0; i < usuarioActual->getCantAmigos(); ++i) {
    Usuario *a = usuarioActual->getAmigoEn(i);
    std::cout << " [" << (i + 1) << "] @" << a->getUsername() << " ("
              << a->getCiudad() << ", " << a->getPais() << " | "
              << a->getCantAlbumes() << " albumes)\n";
  }
  std::cout << "============================================================\n";
  std::cout << "Seleccione el numero del amigo a visitar (0 para volver): ";
  int seleccion = 0;
  if (!(std::cin >> seleccion) || seleccion <= 0 ||
      static_cast<std::size_t>(seleccion) > usuarioActual->getCantAmigos()) {
    if (seleccion < 0) {
      limpiarBufferCin();
    }
    return;
  }

  Usuario *amigo =
      usuarioActual->getAmigoEn(static_cast<std::size_t>(seleccion - 1));

  std::cout
      << "\n============================================================\n";
  std::cout << "   PERFIL DE @" << amigo->getUsername() << '\n';
  std::cout << "============================================================\n";
  std::cout << "Foto de perfil: " << amigo->getRutaFotoPerfil() << '\n';
  std::cout << "Ciudad: " << amigo->getCiudad()
            << " | Pais: " << amigo->getPais() << '\n';

  // Listar álbumes accesibles (públicos o de categoría amigos)
  Album *visibles[50];
  std::size_t cantVisibles = 0;

  std::cout << "\nAlbumes disponibles:\n";
  for (std::size_t i = 0; i < amigo->getCantAlbumes(); ++i) {
    Album *alb = amigo->getAlbumEn(i);
    if (alb->esAccesiblePorUsuario(usuarioActual, amigo)) {
      visibles[cantVisibles] = alb;
      std::cout << " [" << (cantVisibles + 1) << "] " << alb->getNombre()
                << " (" << alb->getTipoPrivacidad() << ") - "
                << alb->getCantFotos() << " fotos\n";
      ++cantVisibles;
      if (cantVisibles >= 50)
        break;
    }
  }

  if (cantVisibles == 0) {
    std::cout << "(No hay albumes visibles para mostrar)\n";
  } else {
    std::cout
        << "\nSeleccione el numero de album a visualizar (0 para salir): ";
    int sel = 0;
    if (std::cin >> sel && sel > 0 &&
        static_cast<std::size_t>(sel) <= cantVisibles) {
      menuCarruselFotos(visibles[static_cast<std::size_t>(sel - 1)], app);
    }
  }

  app.reportarConsumo("Visitar Perfil Amigo y Albumes",
                      sizeof(visibles) + sizeof(int) + sizeof(Usuario *));
  app.reiniciarIteraciones();
}

static void menuSugerenciasAmistad(Programa &app, Usuario *usuarioActual) {
  int k = 0;
  std::cout << "\nIngrese la cantidad K de amigos sugeridos a consultar (entre "
               "1 y 20): ";
  if (!(std::cin >> k) || k < 1) {
    k = 5;
  }

  Usuario *sugeridos[20];
  std::size_t coincidencias[20];
  std::size_t cant = 0;

  app.reiniciarIteraciones();
  app.sugerirAmigos(usuarioActual, static_cast<std::size_t>(k), sugeridos,
                    coincidencias, cant);

  std::cout << "\n--- TOP " << cant << " SUGERENCIAS DE AMISTAD ---\n";
  if (cant == 0) {
    std::cout << "(No se encontraron sugerencias con amigos en comun)\n";
  } else {
    for (std::size_t i = 0; i < cant; ++i) {
      std::cout << " [" << (i + 1) << "] @" << sugeridos[i]->getUsername()
                << " (" << sugeridos[i]->getCiudad() << ", "
                << sugeridos[i]->getPais() << " | " << coincidencias[i]
                << " amigos en comun)\n";
    }

    std::cout << "\n¿Desea enviar solicitud a alguno de ellos? (Ingrese el "
                 "numero o 0 para omitir): ";
    int sel = 0;
    if (std::cin >> sel && sel > 0 && static_cast<std::size_t>(sel) <= cant) {
      Usuario *elegido = sugeridos[static_cast<std::size_t>(sel - 1)];
      usuarioActual->enviarSolicitudAmistad(elegido, "2026-10-15 16:00");
      std::cout << "Solicitud enviada exitosamente a @"
                << elegido->getUsername() << ".\n";
    }
  }

  app.reportarConsumo("Ver Sugerencias de Amistad (Matriz Top-K)",
                      sizeof(sugeridos) + sizeof(coincidencias));
  app.reiniciarIteraciones();
}

static void menuNotificaciones(Programa &app, Usuario *usuarioActual) {
  int opcion = 0;
  do {
    std::cout
        << "\n============================================================\n";
    std::cout << "                 CENTRO DE NOTIFICACIONES\n";
    std::cout
        << "============================================================\n";
    std::cout << "1. Ver y responder solicitudes de amistad recibidas\n";
    std::cout << "2. Ver sugerencias de amistad (Top-K matriz)\n";
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
      std::size_t nuevos = 0;

      for (std::size_t i = 0; i < usuarioActual->getCantMensajes(); ++i) {
        Mensaje *m = usuarioActual->getMensajeEn(i);
        if (!m->estaLeido() && m->getReceptor() == usuarioActual) {
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

      std::cout << "\n--- MENSAJES RECIBIDOS EN EL RANGO [" << fechaIni << " a "
                << fechaFin << "] ---\n";
      std::size_t encontrados = 0;

      for (std::size_t i = 0; i < usuarioActual->getCantMensajes(); ++i) {
        Mensaje *m = usuarioActual->getMensajeEn(i);
        if (m->getReceptor() == usuarioActual &&
            m->estaEnRango(fechaIni, fechaFin)) {
          std::cout << "De: @"
                    << (m->getEmisor() ? m->getEmisor()->getUsername()
                                       : "anonimo")
                    << " ";
          m->mostrarEnPantalla();
          ++encontrados;
        }
      }

      if (encontrados == 0) {
        std::cout << "(No se encontraron mensajes en el rango indicado)\n";
      }
      app.reportarConsumo("Buscar Mensajes por Rango de Fecha",
                          sizeof(fechaIni) + sizeof(fechaFin));
      app.reiniciarIteraciones();

    } else if (opcion == 5) {
      if (usuarioActual->getCantAmigos() == 0) {
        std::cout
            << "\n(Aun no tienes amigos en tu lista para enviarles mensajes)\n";
        continue;
      }

      std::cout << "\n--- TUS AMIGOS DISPONIBLES PARA MENSAJE ---\n";
      for (std::size_t i = 0; i < usuarioActual->getCantAmigos(); ++i) {
        Usuario *a = usuarioActual->getAmigoEn(i);
        std::cout << " [" << (i + 1) << "] @" << a->getUsername() << '\n';
      }
      std::cout
          << "Seleccione el numero del amigo destinatario (0 para cancelar): ";
      int selAmigo = 0;
      if (!(std::cin >> selAmigo) || selAmigo <= 0 ||
          static_cast<std::size_t>(selAmigo) > usuarioActual->getCantAmigos()) {
        continue;
      }

      Usuario *dest =
          usuarioActual->getAmigoEn(static_cast<std::size_t>(selAmigo - 1));

      std::cout << "Ingrese el texto del mensaje para @" << dest->getUsername()
                << " (hasta 512 caracteres):\n> ";
      limpiarBufferCin();
      char texto[513];
      std::cin.getline(texto, sizeof(texto));

      if (UtilidadesCadena::longitud(texto) == 0) {
        std::cout << "No se puede enviar un mensaje vacio.\n";
      } else {
        usuarioActual->enviarMensaje(dest, texto, "2026-10-15 17:00");
        std::cout << "Mensaje enviado exitosamente a @" << dest->getUsername()
                  << ".\n";
      }
      app.reportarConsumo("Enviar Mensaje Privado",
                          sizeof(texto) + sizeof(Usuario *));
      app.reiniciarIteraciones();
    }
  } while (opcion != 6);
}

// ---------------------------------------------------------------------------
// Función Principal (Punto de Entrada)
// ---------------------------------------------------------------------------

int main() {
  Programa app;
  const char *archivoAlmacenamiento = "datos_udeabook.txt";

  // Requisito I: Carga automática desde almacenamiento permanente
  if (!app.cargarDatos(archivoAlmacenamiento)) {
    std::cerr << "Error: No se pudo abrir o cargar el archivo de datos '"
              << archivoAlmacenamiento << "'.\n";
    return 1;
  }

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
  mostrar3MejoresFotos(usuarioActual);

  int opcion = 0;
  do {
    std::cout
        << "\n============================================================\n";
    std::cout << "                   MENU PRINCIPAL\n";
    std::cout
        << "============================================================\n";
    std::cout << "1. Actualizar set de 3 fotos de mejores amigos\n";
    std::cout << "2. Enviar solicitud de amistad\n";
    std::cout << "3. Visitar perfil de un amigo (y ver albumes/fotos)\n";
    std::cout << "4. Notificaciones (Solicitudes, sugerencias y mensajes)\n";
    std::cout << "5. Cerrar sesion y salir\n";
    std::cout << "Seleccione una opcion: ";

    if (!(std::cin >> opcion)) {
      limpiarBufferCin();
      continue;
    }

    switch (opcion) {
    case 1:
      mostrar3MejoresFotos(usuarioActual);
      app.reportarConsumo("Actualizar Set de Mejores Amigos", 0);
      app.reiniciarIteraciones();
      break;

    case 2:
      accionEnviarSolicitudAmistad(app, usuarioActual);
      break;

    case 3:
      menuVisitarPerfilAmigo(app, usuarioActual);
      break;

    case 4:
      menuNotificaciones(app, usuarioActual);
      break;

    case 5:
      std::cout << "\nGuardando cambios en almacenamiento permanente...\n";
      app.guardarDatos(archivoAlmacenamiento);
      std::cout << "¡Hasta pronto, @" << usuarioActual->getUsername() << "!\n";
      break;

    default:
      std::cout << "Opcion invalida. Intente de nuevo.\n";
      break;
    }

  } while (opcion != 5);

  return 0;
}
