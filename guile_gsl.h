#ifndef _guile_gsl_h_
#define _guile_gsl_h_

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

/* gsl matrix smob type */
struct gsl_matrix_smob
{
  gsl_matrix *gsl_matrix;
  SCM update_func;
};

/* gsl vector smob type */
struct gsl_vector_smob
{
  gsl_vector *gsl_vector;
  SCM update_func;
};

extern void init_gsl (void);

/* GSL vector function declarations */
extern scm_t_bits gsl_vector_tag;
extern SCM make_gsl_vector (SCM s_size);
extern gsl_vector* scm_to_gsl_vector (SCM scm_vector);
extern SCM scm_from_gsl_vector (gsl_vector *gsl_vector);
extern SCM zero_gsl_vector (SCM gsl_vector_smob);
extern SCM get_gsl_vector (SCM gsl_vector_smob, SCM scm_index);
extern SCM set_all_gsl_vector (SCM gsl_vector_smob, SCM scm_index);
extern size_t free_gsl_vector (SCM gsl_vector_smob);
extern int print_gsl_vector (SCM gsl_vector_smob,\
    SCM port, scm_print_state *pstate);

/* GSL matrix function declarations */
extern scm_t_bits gsl_matrix_tag;
extern SCM make_gsl_matrix (SCM s_size1, SCM s_size2);
extern SCM set_all_gsl_matrix (SCM gsl_matrix_smob, SCM number);
extern SCM zero_gsl_matrix (SCM gsl_matrix_smob);
extern SCM get_gsl_matrix (SCM gsl_matrix_smob, SCM scm_row, SCM scm_col);
extern size_t free_gsl_matrix (SCM gsl_matrix_smob);
extern int print_gsl_matrix (SCM gsl_matrix_smob,\
    SCM port, scm_print_state *pstate);

#endif
