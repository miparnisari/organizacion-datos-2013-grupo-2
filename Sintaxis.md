# Para compilar, ejecutar y debuggear #

  * Para compilar completamente en terminal: `make clean;make` dentro del directorio `src`
  * Para ver pérdidas de memoria en terminal: `make clean; make; sh valgrind.sh` dentro del directorio `tests`
  * Para debuggear en terminal: `gdb ./main ....`, luego `run`, y si pincha ponen `backtrace`.


# Convenciones de sintaxis #

  * **El código que suban al repositorio debe poder compilar, aunque no ande bien.**
  * Idioma del código: castellano
  * Variables y métodos: `con_underscore`
  * Dentro de los métodos, el `{` de inicio va en la linea siguiente a la firma del método, y el `}` de final va en una linea al final, a la misma altura que el inicio de la firma del método.
```
int main()
{

   // ... codigo ..

}
```

  * TODAS las clases con su `.h` y su `.cpp`. Si no, el Makefile no va andar bien.
  * NO usar `using namespace std;` [porque puede traer problemas](http://stackoverflow.com/questions/1452721/why-is-using-namespace-std-considered-a-bad-practice-in-c).
  * Para marcar algo a revisar : `//FIXME`
  * Para marcar algo a completar: `//TODO`
  * Los métodos que sean `private`, deben empezar con `__`. Ejemplo: `__calcular_espacio_libre()`