#ifndef NOTIFICACION_H
#define NOTIFICACION_H
class Usuario;

/*
 * Clase Base Abstracta: Notificacion
 * -----------------------------------
 * Modela un evento dirigido entre dos usuarios (emisor y receptor)
 * ocurrido en una fecha y hora específica.
 *
 * Utiliza memoria dinámica para la fechaHora y destructores virtuales
 * para garantizar la correcta destrucción polimórfica.
 */
class Notificacion {
protected:
  Usuario *emisor;   // Usuario que origina la notificación (asociación)
  Usuario *receptor; // Usuario destinatario de la notificación (asociación)
  char *fechaHora;   // Cadena dinámica con la fecha/hora del evento
  bool procesada;    // Estado de procesamiento de la notificación

public:
  // --- Constructor y Destructor Virtual ---
  Notificacion(Usuario *emisor, Usuario *receptor, const char *fechaHora);
  Notificacion(const Notificacion &otra);
  virtual ~Notificacion();

  // --- Operador de asignación profunda ---
  Notificacion &operator=(const Notificacion &otra);

  // --- Métodos Virtuales Puros ---
  virtual void mostrarEnPantalla() const = 0;
  virtual const char *getTipo() const = 0;

  // --- Consultas y Modificadores ---
  Usuario *getEmisor() const;
  Usuario *getReceptor() const;
  const char *getFechaHora() const;
  bool estaProcesada() const;
  void setProcesada(bool estado);
};

#endif // NOTIFICACION_H
