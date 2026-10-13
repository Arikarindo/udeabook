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

int UtilidadesCadena::comparar(const char *a, const char *b) {
  if (a == nullptr && b == nullptr) {
    return 0;
  }
  if (a == nullptr) {
    return -1;
  }
  if (b == nullptr) {
    return 1;
  }
  std::size_t i = 0;
  while (a[i] != '\0' && b[i] != '\0') {
    if (a[i] != b[i]) {
      return (static_cast<unsigned char>(a[i]) <
              static_cast<unsigned char>(b[i]))
                 ? -1
                 : 1;
    }
    ++i;
  }
  if (a[i] == b[i]) {
    return 0;
  }
  return (a[i] == '\0') ? -1 : 1;
}
