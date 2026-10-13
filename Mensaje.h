#ifndef MENSAJE_H
#define MENSAJE_H

#include "Notificacion.h"
#include <cstddef>

class Mensaje : public Notificacion {
private:
  char *texto; // Texto del mensaje (hasta 512 caracteres en memoria dinámica)
  bool leido;

public:
  // --- Constructores y Destructor ---
  Mensaje(Usuario *emisor, Usuario *receptor, const char *fechaHora,
          const char *textoMensaje);
  Mensaje(const Mensaje &otro);
  virtual ~Mensaje() override;

  // --- Operador de Asignación Profunda ---
  Mensaje &operator=(const Mensaje &otro);

  // --- Operaciones de Negocio ---
  void marcarComoLeido();
  bool estaLeido() const;
  const char *getTexto() const;

  // Comprueba si la fecha del mensaje cae dentro del rango [fechaInicio,
  // fechaFin]
  bool estaEnRango(const char *fechaInicio, const char *fechaFin) const;

  // --- Implementación de Métodos Virtuales ---
  virtual void mostrarEnPantalla() const override;
  virtual const char *getTipo() const override;
};

#endif // MENSAJE_H
