#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dlfcn.h>

int main (int l_param, char * wparam[]){
  int i;
  int tab[21]={1,2,3,4,5,6,7,8,9,0,0,1,2,3,4,5,6,7,8,9,0};

/*
1) otworz biblioteke
2) przypisz wskaznikom f1, f2 f3 adresy funkcji z biblioteki sumuj, srednia i mediana
3) stworz Makefile kompilujacy biblioteke 'bibl1' ladowana dynamicznie oraz kompilujacy ten program
4) Stosowne pliki powinny znajdowac sie w folderach '.', './bin', './'lib'. Nalezy uzyc: LD_LIBRARY_PATH
5) W Makefile nalezy dodac: test
*/


  // 1) Otwórz bibliotekę
  void *handle = dlopen("./lib/bibl1.so", RTLD_LAZY);
  if (!handle) {
      fprintf(stderr, "Error opening library: %s\n", dlerror());
      exit(EXIT_FAILURE);
  }

  // 2) Przypisz wskaźnikom f1, f2, f3 adresy funkcji z biblioteki
  int (*f1)(int *, int) = dlsym(handle, "sumuj");
  double (*f2)(int *, int) = dlsym(handle, "srednia");
  int (*f3)(int *, int) = dlsym(handle, "mediana");

  char *error;
  if ((error = dlerror()) != NULL) {
      fprintf(stderr, "Error loading symbols: %s\n", error);
      dlclose(handle);
      exit(EXIT_FAILURE);
  }

  // 4) Wywołaj funkcje i wypisz wyniki
  for (i = 0; i < 5; i++) {
      printf("Wynik: %d, %lf, %d\n", f1(tab + 2 * i, 21 - 2 * i), f2(tab + 2 * i, 21 - 2 * i), f3(tab + 2 * i, 21 - 2 * i));
  }

  // Zamknij bibliotekę
  dlclose(handle);
  return 0;
}
