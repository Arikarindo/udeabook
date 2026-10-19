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

bool UtilidadesCadena::sonIguales(const char *a, const char *b) {
  if (a == nullptr || b == nullptr) {
    return a == b;
  }
  std::size_t i = 0;
  while (a[i] != '\0' && b[i] != '\0') {
    if (a[i] != b[i]) {
      return false;
    }
    ++i;
  }
  return a[i] == b[i];
}

char *UtilidadesCadena::concatenar(const char *a, const char *b) {
  const std::size_t lenA = longitud(a);
  const std::size_t lenB = longitud(b);
  char *resultado = new char[lenA + lenB + 1];

  for (std::size_t i = 0; i < lenA; ++i) {
    resultado[i] = a[i];
  }
  for (std::size_t j = 0; j < lenB; ++j) {
    resultado[lenA + j] = b[j];
  }
  resultado[lenA + lenB] = '\0';
  return resultado;
}

void UtilidadesCadena::copiar(char *destino, const char *origen,
                              std::size_t capacidad) {
  if (destino == nullptr || capacidad == 0) {
    return;
  }
  if (origen == nullptr) {
    destino[0] = '\0';
    return;
  }
  std::size_t i = 0;
  while (i + 1 < capacidad && origen[i] != '\0') {
    destino[i] = origen[i];
    ++i;
  }
  destino[i] = '\0';
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
