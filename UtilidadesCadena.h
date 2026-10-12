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

  // Compara si dos cadenas de caracteres son exactamente iguales
  static bool sonIguales(const char *a, const char *b);

  // Concatena dos cadenas en una nueva cadena asignada en memoria dinámica
  static char *concatenar(const char *a, const char *b);

  // Copia segura hacia un arreglo preexistente sin desbordar la capacidad
  static void copiar(char *destino, const char *origen, std::size_t capacidad);

  // Comparación lexicográfica: < 0 si a < b, 0 si a == b, > 0 si a > b
  // Ideal para ordenar y verificar rangos de fechas "AAAA-MM-DD"
  static int comparar(const char *a, const char *b);
};

#endif // UTILIDADES_CADENA_H
