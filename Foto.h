#ifndef FOTO_H
#define FOTO_H

class Foto {
private:
  char *rutaArchivo;  // Ruta absoluta en notación Linux
  char *fechaCaptura; // Formato "AAAA-MM-DD"
  int tamanoBytes;    // Peso del archivo en disco en bytes
  int conteoLikes;    // Contador de likes

public:
  // --- Constructores y Destructor ---
  Foto(); // Constructor por defecto
  Foto(const char *ruta, const char *fecha, int tamano);
  Foto(const Foto &otra); // Constructor de copia (copia profunda)
  ~Foto();                // Destructor (libera memoria dinámica)

  // --- Sobrecarga de Operadores ---
  Foto &operator=(const Foto &otra); // Operador de asignación (copia profunda)

  // --- Modificadores ---
  void darLike();    // Incrementa en 1
  void quitarLike(); // Decrementa en 1 (mínimo 0)

  // --- Consultas (Getters const) ---
  int getLikes() const;
  const char *getRuta() const;
  const char *getFechaCaptura() const;
  int getTamanoBytes() const;

  // --- Visualización ---
  void mostrarRuta() const;
};

#endif // FOTO_H
