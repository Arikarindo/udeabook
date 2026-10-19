#ifndef MENSAJE_H
#define MENSAJE_H

#include <cstddef>

class Usuario;

class Mensaje {
private:
  Usuario *emisor;
  Usuario *receptor;
  char *fechaHora;
  char *texto;
  bool leido;

public:
  // --- Constructores y Destructor ---
  Mensaje(Usuario *emisor, Usuario *receptor, const char *fechaHora,
          const char *textoMensaje);
  Mensaje(const Mensaje &otro);
  ~Mensaje();

  // --- Operador de Asignación Profunda ---
  Mensaje &operator=(const Mensaje &otro);

  // --- Operaciones y Consultas ---
  void marcarComoLeido();
  bool estaLeido() const;
  const char *getTexto() const;
  const char *getFechaHora() const;
  Usuario *getEmisor() const;
  Usuario *getReceptor() const;

  // Filtrado por rango de fecha
  bool estaEnRango(const char *fechaInicio, const char *fechaFin) const;

  // Visualización
  void mostrarEnPantalla() const;
};

#endif // MENSAJE_H
