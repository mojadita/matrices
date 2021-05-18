#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* structure to hold all the data for a matrix */
struct matrix {
    int m_rows, m_cols;
    double **m_data;
};

/* this are simple providers of matrix data, depending on how we
 * want to initialize the matrix. */

/* initializes each element to a constant provided by reference */
double prov_const(void *cb_par, int row, int col)
{
    double *val_ref = cb_par;
    return *val_ref;
}

double prov_list(void *cb_par, int row, int col)
{
    double **p = cb_par;
    return *(*p)++;
}

double prov_copy(void *cb_par, int row, int col)
{
    struct matrix *src = cb_par;

    assert(row < src->m_rows && col < src->m_cols);
    return src->m_data[row][col];
}

/* initializes kroneker matrix. Identity */
double prov_ident(void *unused, int row, int col)
{
    return row == col ? 1.0 : 0.0;
}

/* provides elements from file stream */
double prov_file(void *cb_par, int row, int col)
{
    FILE *in = cb_par;
    double ret_val;
    if (isatty(fileno(in))) {
        fprintf(stderr,
                "mat[%d][%d]: ",
                row, col);
        fflush(stderr);
    }
    int res = fscanf(in, "%lg", &ret_val);
    assert(res == 1);
    return ret_val;
}

/* this creates the whole matrix with just one malloc */
struct matrix *mat_creat(
    int rows, int cols, /* dimensions */
    double prov(void *cd, int row, int col), /* initializer */
    void *cd) /* initializer callback data */
{
    struct matrix *ret_val = malloc(
        sizeof *ret_val
        + rows * sizeof *ret_val->m_data
        + rows * cols * sizeof **ret_val->m_data);

    assert(ret_val != NULL);

    double **aux = (double **) (ret_val + 1);
    double *aux2 = (double *) (aux + rows);

    ret_val->m_rows = rows;
    ret_val->m_cols = cols;
    ret_val->m_data = aux;
    for (int row = 0; row < rows; row++) {
        *aux++ = aux2; /* the pointer to the row */
        if (prov) {
            for (int col = 0; col < cols; col++) {
                /* this provides each element of the row */
                *aux2++ = prov(cd, row, col);
            }
        } else {
            /* no provider, no cell initialization */
            aux2 += cols;
        }
    }
    return ret_val;
}

struct matrix *mat_add(struct matrix *dst, struct matrix *b)
{
    assert(dst->m_rows == b->m_rows
        && dst->m_cols == b->m_cols);

    for (int row = 0; row < dst->m_rows; row++) {
        for (int col = 0; col < dst->m_cols; col++) {
            dst->m_data[row][col] += b->m_data[row][col];
        }
    }
    return dst;
}

struct matrix *mat_mscalar(struct matrix *dst, double b)
{
    for (int row = 0; row < dst->m_rows; row++) {
        for (int col = 0; col < dst->m_cols; col++) {
            dst->m_data[row][col] *= b;
        }
    }
    return dst;
}

struct matrix *mat_prod(struct matrix *a, struct matrix *b)
{
    assert(a->m_cols == b->m_rows);

    struct matrix *ret_val
        = mat_creat(a->m_rows, b->m_cols, NULL, NULL);

    for(int row = 0; row < a->m_rows; row++) {
        for (int col = 0; col < b->m_cols; col++) {
            double accum = 0.0;
            for (int k = 0; k < a->m_cols; k++) {
                accum += a->m_data[row][k]
                       * b->m_data[k][col];
            }
            ret_val->m_data[row][col] = accum;
        }
    }
    return ret_val;
}

struct matrix *mat_pow(struct matrix *a, unsigned N)
{
    /* ensure matrix is square */
    assert(a->m_rows == a->m_cols);

    struct matrix *ret_val
        = mat_creat(a->m_rows, a->m_cols,
                prov_ident, NULL);

    if (N == 0) return ret_val;  /* a**0 -> I */

    /* search for most significant bit */
    unsigned bit = 0;
    while ((1 << bit) < N) bit++;
    bit = 1 << bit;

    while (bit) {
        /* square it */
        struct matrix *aux = mat_prod(ret_val, ret_val);
        free(ret_val); /* must free after multiplying */
        ret_val = aux; /* assign the new matrix. */

        if (bit & N) { /* multiply by a */
            aux = mat_prod(ret_val, a);
            free(ret_val);
            ret_val = aux;
        }
        bit >>= 1;
    }
    return ret_val;
}

ssize_t mat_print(struct matrix *m, FILE *out, char *fmt)
{

#define ACT() do{         \
        if (n < 0) {      \
            return n;     \
        } else {          \
            ret_val += n; \
        }                 \
    } while (0)

    ssize_t ret_val = 0, n;
    char *sep1 = "{";
    for (int row = 0; row < m->m_rows; row++) {
        n = fprintf(out, "%s", sep1);
        ACT();
        sep1 = ",\n ";
        char *sep2 = "{";
        for (int col = 0; col < m->m_cols; col++) {
            n = fprintf(out, "%s", sep2);
            ACT();
            sep2 = ", ";
            n = fprintf(out, fmt, m->m_data[row][col]);
            ACT();
        }
        n = fprintf(out, "}");
        ACT();
    }
    n = fprintf(out, "}\n");
    ACT();
    return ret_val;
}


int main(int argc, char **argv)
{
    static double values[] = {
        1.0, 2.0, 3.0, 4.0,
        0.0, 1.0, 5.0, 6.0,
        0.0, 0.0, 1.0, 7.0,
        0.0, 0.0, 0.0, 1.0,
    };
    static double coefs[] = {
        1.0, -4.0, 6.0, -4.0, 1.0,
    };
    static double zero = 0.0;
    double *p = values;

    struct matrix *M = mat_creat(4, 4, prov_list, &p);
    printf("M =\n");
    mat_print(M, stdout, "%g");

    struct matrix *P = mat_creat(4, 4, prov_const, &zero);

    printf("P[M] = ");
    for (int coef = 0; coef <= 4; coef++) {
        if (coef) {
            if (coefs[coef] > 0.0) {
                printf(" +");
            } else {
                printf(" ");
            }
        }
        printf("%g * %s", coefs[coef], coef ? "A" : "I");
        if (coef > 1) {
            printf("^%d", coef);
        }
    }
    printf(" =\n");
    for (int coef = 0; coef <= 4; coef++) {
        printf("M\n");
        mat_print(M, stdout, "%g");
        printf("**%d\n", coef);
        struct matrix *TN = mat_pow(M, coef);
        mat_print(TN, stdout, "%g");
        TN = mat_mscalar(TN, coefs[coef]);
        printf("*%lg\n", coefs[coef]);
        mat_print(TN, stdout, "%g");
        P = mat_add(P, TN);
        printf("+\n");
        mat_print(P, stdout, "%g");
        free(TN);
    }
    free(P);
    free(M);
}
