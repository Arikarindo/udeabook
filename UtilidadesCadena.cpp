#include "UtilidadesCadena.h"

std::size_t UtilidadesCadena::longitud(const char *cadena) {
  if (cadena == nullptr) {
    return 0;
  }
  std::size_t len = 0;
  while (cadena[len] != '\0') {
    ++len;
  }
  return len;
}

char *UtilidadesCadena::duplicar(const char *origen) {
  const std::size_t len = longitud(origen);
  char *destino = new char[len + 1];
  for (std::size_t i = 0; i < len; ++i) {
    destino[i] = origen[i];
  }
  destino[len] = '\0';
  return destino;
}
