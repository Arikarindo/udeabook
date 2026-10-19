// ===========================================================================
// Programa.h — Controladora del Sistema, Grafo y Métricas (NIVEL 5)
// ===========================================================================
// Centraliza:
//   - Gestión de todos los usuarios registrados (Usuario**).
//   - Grafo de relaciones mediante Matriz de Adyacencia dinámica (bool**).
//   - Algoritmo de sugerencias de amigos basado en amigos en común (Top-K).
//   - Persistencia: lectura y escritura hacia almacenamiento permanente.
//   - Medición estricta del consumo de recursos (Iteraciones y Memoria RAM).
// ===========================================================================

#ifndef PROGRAMA_H
#define PROGRAMA_H

#include "Usuario.h"
#include <cstddef>

/*
 * Clase auxiliar para ranking de recomendaciones (sin structs)
 */
class SugerenciaAmigo {
public:
  Usuario *usuario;
  std::size_t amigosEnComun;

  SugerenciaAmigo();
  SugerenciaAmigo(Usuario *u, std::size_t c);
};

/*
 * Clase Controladora: Programa
 * ----------------------------
 * Orquesta el ciclo de vida del sistema UdeABook, la autenticación,
 * el grafo de relaciones y el cómputo de métricas de eficiencia.
 */
class Programa {
private:
  Usuario **usuarios;           // Arreglo dinámico de todos los usuarios
  std::size_t cantUsuarios;
  std::size_t capUsuarios;

  bool **matrizAdyacencia;      // Matriz dinámica de adyacencia (cantUsuarios x cantUsuarios)
  std::size_t tamañoMatriz;     // Dimensión actual de la matriz cuadrada

  mutable std::size_t contadorIteraciones; // Contador de iteraciones directas e indirectas
  char rutaPersistencia[256];              // Ruta real del archivo .txt cargado

  // --- Helpers de Grafo y Memoria ---
  void redimensionarUsuarios(std::size_t nuevaCapacidad);
  void liberarMatriz();
  void liberarUsuarios();

public:
  Programa();
  ~Programa();

  // --- Gestión de Usuarios ---
  void registrarUsuario(Usuario *nuevoUsuario);
  Usuario *buscarUsuario(const char *username) const;
  int obtenerIndiceUsuario(const Usuario *u) const;
  std::size_t getCantUsuarios() const;
  Usuario *getUsuarioEn(std::size_t indice) const;

  // --- Autenticación ---
  Usuario *autenticarUsuario(const char *username, const char *password) const;

  // --- Grafo de Amistades (Matriz de Adyacencia) ---
  void construirMatrizAdyacencia();
  bool sonAmigosPorMatriz(std::size_t i, std::size_t j) const;
  std::size_t contarAmigosEnComunPorMatriz(std::size_t i, std::size_t j) const;

  // --- Sugerencias de Amistad (Requisito V) ---
  // Obtiene hasta K sugerencias rankeadas por cantidad de amigos en común
  void sugerirAmigos(const Usuario *actual, std::size_t k,
                     Usuario **sugeridos, std::size_t *amigosComun,
                     std::size_t &cantSugeridos) const;

  // --- Persistencia en Disco (Requisito I) ---
  bool cargarDatos(const char *rutaArchivo);
  bool guardarDatos(const char *rutaArchivo) const;

  // --- Métricas de Recursos (Requisito VII) ---
  void reiniciarIteraciones();
  void registrarIteraciones(std::size_t cantidad) const;
  std::size_t getIteraciones() const;

  std::size_t calcularMemoriaTotal(std::size_t memoriaLocal = 0) const;
  void reportarConsumo(const char *nombreFuncionalidad, std::size_t memoriaLocal = 0) const;
};

#endif // PROGRAMA_H
