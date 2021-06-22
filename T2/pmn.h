#ifndef PMN_H
#define PMN_H

#include <stdbool.h>

enum {
      PNM_P1      = 1, /* ASCII PBM */
      PNM_P2      = 2, /* ASCII PGM */
      PNM_P3      = 3, /* ASCII PPM */
      PNM_P4      = 4, /* BINARY PBM */
      PNM_P5      = 5, /* BINARY PGM */
      PNM_P6      = 6, /* BINARY PPM */
      PNM_UNKNOWN = 0
};

enum {
      RED   = 0,
      GREEN = 1,
      BLUE  = 2
};


int leia_cabecalho( FILE * src,
                    unsigned int  * linhas,
                    unsigned int  * cols,
                    unsigned int  * valormax );

bool leia_imagem( FILE * src,
                  unsigned int linhas,
                  unsigned int cols,
                  unsigned char valormax,
                  unsigned char img[linhas][cols][3] );

bool escreve_imagem( char nome[],
                     unsigned int linhas,
                     unsigned int cols,
                     unsigned char valormax,
                     unsigned char img[linhas][cols][3] );

#endif
