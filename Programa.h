#ifndef PROGRAMA_H
#define PROGRAMA_H

#include "Usuario.h"

/*
 * Clase auxiliar para ranking de recomendaciones (sin structs)
 */
class SugerenciaAmigo {
public:
  Usuario *usuario;
  int amigosEnComun;

  SugerenciaAmigo();
  SugerenciaAmigo(Usuario *u, int c);
};

/*
 * Clase Controladora: Programa
 * Inicio de ejecución de Udeabook
 *
 */
class Programa {
private:
  Usuario **usuarios; // Arreglo dinámico de todos los usuarios
  int cantUsuarios;
  int capUsuarios;

  bool **matrizAdyacencia; // Matriz dinámica de adyacencia (cantUsuarios x
                           // cantUsuarios)
  int tamañoMatriz;        // Dimensión actual de la matriz cuadrada

  mutable int
      contadorIteraciones;    // Contador de iteraciones directas e indirectas
  char rutaDatabase[256];     // Ruta real del archivo .txt cargado

  // --- Helpers de Grafo y Memoria ---
  void redimensionarUsuarios(int nuevaCapacidad);
  void liberarMatriz();
  void liberarUsuarios();

public:
  Programa();
  ~Programa();

  // --- Gestión de Usuarios ---
  void registrarUsuario(Usuario *nuevoUsuario);
  Usuario *buscarUsuario(const char *username) const;
  int obtenerIndiceUsuario(const Usuario *u) const;
  int getCantUsuarios() const;
  Usuario *getUsuarioEn(int indice) const;

  // --- Autenticación ---
  Usuario *autenticarUsuario(const char *username, const char *password) const;

  // --- Grafo de Amistades (Matriz de Adyacencia) ---
  void construirMatrizAdyacencia();
  void registrarAmistadEnMatriz(const Usuario *uA, const Usuario *uB);
  bool sonAmigosPorMatriz(int i, int j) const;
  int contarAmigosEnComunPorMatriz(int i, int j) const;

  // --- Sugerencias de Amistad  ---
  // Obtiene hasta K sugerencias rankeadas por cantidad de amigos en común
  void sugerirAmigos(const Usuario *actual, int k, Usuario **sugeridos,
                     int *amigosComun, int &cantSugeridos) const;

  // --- Persistencia en Disco ---
  bool cargarDatos(const char *rutaArchivo);
  bool guardarDatos(const char *rutaArchivo) const;

  // --- Métricas de Recursos ---
  void reiniciarIteraciones();
  void registrarIteraciones(int cantidad) const;
  int getIteraciones() const;

  int calcularMemoriaTotal(int memoriaLocal = 0) const;
  void reportarConsumo(const char *nombreFuncionalidad,
                       int memoriaLocal = 0) const;
};

#endif // PROGRAMA_H
