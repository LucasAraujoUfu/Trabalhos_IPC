#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pmn.h"

/*
  Fonte:
  https://stackoverflow.com/questions/48511809/converting-ppm-file-from-p3-to-p6-using-c
*/
static inline int fpeek(FILE * stream)
{
  int c = fgetc(stream);
  ungetc(c, stream);
  return c;
}

static inline bool pnm_is_whitespace(const char c)
{
  return (c == '\t' || c == '\n' || c == '\v' ||
          c == '\f' || c == '\r' || c == ' ');
}


/*
  The above function parses the input in a very similar fashion that %d does in the scanf()
  family of functions, except that it correctly skips any PNM comment lines preceding the
  value. If there is an error, it returns -1, in which case the file is not really in PNM
  format. Otherwise it returns the nonnegative (0 or larger) value specified in the file,
  without consuming the character that followes the number.
*/

static inline int pnm_header_value(FILE *src) {
  int c;

  /* Skip leading whitespace and comments. */
  c = fgetc(src);
  while (1) {
    if (c == EOF) {
      /* File/stream ends before the value. */
      return -1;
    } else if (c == '#') {
      /* Comment. Skip the rest of the line. */
      do {
        c = fgetc(src);
      } while (c != EOF && c != '\r' && c != '\n');
    } else if (pnm_is_whitespace(c)) {
      /* Skip whitespace. */
      c = fgetc(src);
    } else
      break;
  }

  /* Parse the nonnegative integer decimal number. */
  if (c >= '0' && c <= '9') {
    int  result = (c - '0');

    c = fgetc(src);
    while (c >= '0' && c <= '9') {
      const int  old = result;

      /* Add digit to number. */
      result = 10*result + (c - '0');

      /* Overflow? */
      if (result < old)
        return -1;

      /* Next digit. */
      c = fgetc(src);
    }

    /* Do not consume the separator. */
    if (c != EOF)
      ungetc(c, src);

    /* Success. */
    return result;
  }

  /* Invalid character. */
  return -1;
}



/*
  If you supply a FILE handle to pnm_header(), and pointers to the cols, linhas, and maxval
  (set to 1 for PBM format headers), it will return PNM_P1 to PNM_P6 with the three
  variables filled with the header information, or PNM_UNKNOWN if the header cannot be
  parsed correctly.
*/

int leia_cabecalho( FILE * src,
                    unsigned int  * linhas,
                    unsigned int  * cols,
                    unsigned int  * valormax)
{
  int  format = PNM_UNKNOWN;
  int  value;

  if (!src)
    return PNM_UNKNOWN;

  /* The image always begins with a 'P'. */
  if (fgetc(src) != 'P')
    return PNM_UNKNOWN;

  /* The next character determines the format. */
  switch (fgetc(src)) {
  case '1': format = PNM_P1; break;
  case '2': format = PNM_P2; break;
  case '3': format = PNM_P3; break;
  case '4': format = PNM_P4; break;
  case '5': format = PNM_P5; break;
  case '6': format = PNM_P6; break;
  default:
    return PNM_UNKNOWN;
  }

  /* The next character must be a whitespace. */
  if (!pnm_is_whitespace(fgetc(src)))
    return PNM_UNKNOWN;

  /* Next item is the cols as a decimal string. */
  value = pnm_header_value(src);
  if (value < 1)
    return PNM_UNKNOWN;
  if (cols)
    *cols = value;

  /* Next item is the linhas as a decimal string. */
  value = pnm_header_value(src);
  if (value < 1)
    return PNM_UNKNOWN;
  if (linhas)
    *linhas = value;

  /* Maxdepth, for all but P1 and P4 formats. */
  if (format == PNM_P1 || format == PNM_P4) {
    if (valormax)
      *valormax = 1;
  } else {
    value = pnm_header_value(src);
    if (value < 1)
      return PNM_UNKNOWN;
    if (valormax)
      *valormax = value;
  }

  /* The final character in the header must be a whitespace. */
  if (!pnm_is_whitespace(fgetc(src)))
    return PNM_UNKNOWN;

  /* Success. */
  return format;
}

static inline bool leia_linha( FILE * fp,
                               unsigned int cols,
                               unsigned char linha[cols][3],
                               unsigned char valormax) {
  unsigned int col;

  for (col = 0; col < cols; ++col)
    for( int k = RED; k <= BLUE; k++ ) {
      int cor = pnm_header_value(fp);
      if (cor < 0 || cor > valormax)
        return false;
      linha[col][k] = (unsigned char) cor;
    }
  return true;
} // leia_linha


bool leia_imagem( FILE * src,
                  unsigned int linhas,
                  unsigned int cols,
                  unsigned char valormax,
                  unsigned char img[linhas][cols][3]) {

  unsigned int linha = 0;

  for( linha = 0; linha < linhas; ++linha )
    if (leia_linha(src, cols, img[linha], valormax) == false){
      printf("Erro na leitura da linha %d\n", linha);
      return false;
    }

  return true;
}


static inline bool escreve_linha( FILE * fp,
                                  unsigned int cols,
                                  unsigned char valormax,
                                  unsigned char linha[cols][3]) {
  unsigned int col;

  for (col = 0; col < cols; ++col)
    for( int k = RED; k <= BLUE; k++ ) {
      int cor = linha[col][k];
      if (cor < 0 || cor > valormax)
        return false;
      fprintf(fp, "%u ", cor);
    }

  fputs("\n", fp);

  return true;
} // escreve_linha


static inline void escreve_cabecalho( FILE * fp,
                                      unsigned int linhas,
                                      unsigned int cols,
                                      unsigned char valormax) {
  fprintf(fp, "P3\n");
  fprintf(fp, "%u %u\n", cols, linhas);
  fprintf(fp, "%u\n", valormax);
}


bool escreve_imagem( char nome[],
                     unsigned int linhas,
                     unsigned int cols,
                     unsigned char valormax,
                     unsigned char img[linhas][cols][3]) {
  FILE *fp = fopen( nome, "w" );

  escreve_cabecalho(fp, linhas, cols, valormax);

  unsigned int linha = 0;

  for( linha = 0; linha < linhas; ++linha )
    if (escreve_linha(fp, cols, valormax, img[linha]) == false) {
      printf("Erro na escrita da linha %d\n", linha);
      fclose(fp);
      return false;
    }

  fclose(fp);

  return true;
}
