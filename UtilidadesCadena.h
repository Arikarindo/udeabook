#ifndef UTILIDADES_CADENA_H
#define UTILIDADES_CADENA_H

#include <cstddef> // Para std::size_t

class UtilidadesCadena {
public:
  // Calcula la longitud de una cadena sin contar el terminador nulo '\0'
  static std::size_t longitud(const char *cadena);

  // Reserva memoria dinámica exacta (new char[]) y copia el contenido
  // Siempre garantiza un terminador nulo '\0' al final
  static char *duplicar(const char *origen);
};

#endif // UTILIDADES_CADENA_H
