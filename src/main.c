#include <stdio.h>
#include <stdlib.h>
#include <commons/txt.h>

int foo(void) {
    int dos = 2;
    return dos;
}

int goo(void) {
    int theNumber = 126;
    return theNumber;
}

int main(int argc, char* argv[]) {
    txt_write_in_stdout("Hola, Operativos!!\n");
    return 0;
}
