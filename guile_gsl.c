#include <stdlib.h>
#include <libguile.h>
#include "guile_gsl.h"

/* define tags for instances of GSL vector and matrix types */
scm_t_bits gsl_vector_tag;
scm_t_bits gsl_matrix_tag;

void
init_gsl (void)
{
  /* gsl matrix smob */
  gsl_matrix_tag = scm_make_smob_type ("gsl-matrix",
      sizeof (struct gsl_matrix_smob));
  scm_set_smob_free (gsl_matrix_tag, free_gsl_matrix);
  scm_set_smob_print (gsl_matrix_tag, print_gsl_matrix);

  /* register gsl matrix functions */
  scm_c_define_gsubr ("zero-gsl-matrix", 1, 0, 0, zero_gsl_matrix);
  scm_c_define_gsubr ("set-all-gsl-matrix", 2, 0, 0, set_all_gsl_matrix);
  scm_c_define_gsubr ("make-gsl-matrix", 2, 0, 0, make_gsl_matrix);
  scm_c_define_gsubr ("get-gsl-matrix", 3, 0, 0, get_gsl_matrix);

  /* gsl vector smob */
  gsl_vector_tag = scm_make_smob_type ("gsl-vector",
      sizeof (struct gsl_vector_smob));
  scm_set_smob_free (gsl_vector_tag, free_gsl_vector);
  scm_set_smob_print (gsl_vector_tag, print_gsl_vector);

  /* register gsl vector functions */
  scm_c_define_gsubr ("zero-gsl-vector", 1, 0, 0, zero_gsl_vector);
  scm_c_define_gsubr ("set-all-gsl-vector", 2, 0, 0, set_all_gsl_vector);
  scm_c_define_gsubr ("make-gsl-vector", 1, 0, 0, make_gsl_vector);
  scm_c_define_gsubr ("get-gsl-vector", 2, 0, 0, get_gsl_vector);
}

SCM
make_gsl_matrix (SCM s_size1, SCM s_size2)
{
  SCM smob;
  struct gsl_matrix_smob *gsl_matrix_smob;
  size_t size1 = scm_to_int (s_size1);
  size_t size2 = scm_to_int (s_size2);

  /* allocate the memory block */
  gsl_matrix_smob = (struct gsl_matrix_smob *)
    scm_gc_malloc (sizeof (struct gsl_matrix_smob), "gsl-matrix");

  /* initialize */
  gsl_matrix_smob->update_func = SCM_BOOL_F;

  /* create the smob */
  SCM_NEWSMOB (smob, gsl_matrix_tag, gsl_matrix_smob);

  /* finish the initialization */
  gsl_matrix_smob->gsl_matrix = scm_gc_malloc (1, "gsl matrix");
  gsl_matrix_smob->gsl_matrix = gsl_matrix_calloc (size1, size2);

  return (smob);
}

SCM
get_gsl_matrix (SCM gsl_matrix_smob, SCM scm_row, SCM scm_col)
{
  struct gsl_matrix_smob *smob;

  /* check smob is of a suitable type */
  scm_assert_smob_type (gsl_matrix_tag, gsl_matrix_smob);

  /* retrieve immediate word of smob */
  smob = (struct gsl_matrix_smob *) SCM_SMOB_DATA (gsl_matrix_smob);

  /* get gsl matrix element */
  int row = scm_to_int (scm_row);
  int col = scm_to_int (scm_col);
  double element = gsl_matrix_get (smob->gsl_matrix, row, col);
  SCM scm_element = scm_int2num (element);

  /* invoke the update function */
  if (scm_is_true (smob->update_func))
    scm_call_0 (smob->update_func);

  scm_remember_upto_here_1 (gsl_matrix_smob);

  return (scm_element);
}

SCM
set_all_gsl_matrix (SCM gsl_matrix_smob, SCM scm_index)
{
  struct gsl_matrix_smob *smob;

  /* check smob is of a suitable type */
  scm_assert_smob_type (gsl_matrix_tag, gsl_matrix_smob);

  /* retrieve immediate word of smob */
  smob = (struct gsl_matrix_smob *) SCM_SMOB_DATA (gsl_matrix_smob);

  /* set gsl matrix elements */
  gsl_matrix_set_all (smob->gsl_matrix,\
      scm_num2double (scm_index, 0, "set_all_gsl_matrix ()"));

  /* invoke the update function */
  if (scm_is_true (smob->update_func))
    scm_call_0 (smob->update_func);

  scm_remember_upto_here_1 (gsl_matrix_smob);

  return (SCM_UNSPECIFIED);
}

SCM
zero_gsl_matrix (SCM gsl_matrix_smob)
{
  struct gsl_matrix_smob *smob;

  /* check smob is of a suitable type */
  scm_assert_smob_type (gsl_matrix_tag, gsl_matrix_smob);

  /* retrieve immediate word of smob */
  smob = (struct gsl_matrix_smob *) SCM_SMOB_DATA (gsl_matrix_smob);

  /* zero the gsl matrix */
  gsl_matrix_set_zero (smob->gsl_matrix);

  /* invoke the update function */
  if (scm_is_true (smob->update_func))
    scm_call_0 (smob->update_func);

  scm_remember_upto_here_1 (gsl_matrix_smob);

  return (SCM_UNSPECIFIED);
}

size_t
free_gsl_matrix (SCM gsl_matrix_smob)
{
  struct gsl_matrix_smob *smob = (struct gsl_matrix_smob *)
    SCM_SMOB_DATA (gsl_matrix_smob);

  /* free resources used by the smob */
  gsl_matrix_free (smob->gsl_matrix);

  return (0);
}

int
print_gsl_matrix (SCM gsl_matrix_smob, SCM port, scm_print_state *pstate)
{
  struct gsl_matrix_smob *smob = (struct gsl_matrix_smob *)
    SCM_SMOB_DATA (gsl_matrix_smob);

  //SCM scm_matrix = scm_from_gsl_matrix (smob->gsl_matrix);

  scm_puts ("#<gsl-matrix \n", port);
  gsl_matrix_fprintf (stdout, smob->gsl_matrix, "%f");
  //scm_display (scm_vector, port);
  scm_puts (">", port);
  scm_newline (port);

  /* non-zero means success */
  return (1);
}

SCM
make_gsl_vector (SCM s_size)
{
  SCM smob;
  struct gsl_vector_smob *gsl_vector_smob;
  size_t size = scm_to_int (s_size);

  /* allocate the memory block */
  gsl_vector_smob = (struct gsl_vector_smob *)
    scm_gc_malloc (sizeof (struct gsl_vector_smob), "gsl-vector");

  /* initialize */
  gsl_vector_smob->update_func = SCM_BOOL_F;

  /* create the smob */
  SCM_NEWSMOB (smob, gsl_vector_tag, gsl_vector_smob);

  /* finish the initialization */
  gsl_vector_smob->gsl_vector = scm_gc_malloc (1, "gsl vector");
  gsl_vector_smob->gsl_vector = gsl_vector_calloc (size);

  return (smob);
}

SCM
get_gsl_vector (SCM gsl_vector_smob, SCM scm_index)
{
  struct gsl_vector_smob *smob;

  /* check smob is of a suitable type */
  scm_assert_smob_type (gsl_vector_tag, gsl_vector_smob);

  /* retrieve immediate word of smob */
  smob = (struct gsl_vector_smob *) SCM_SMOB_DATA (gsl_vector_smob);

  /* set gsl vector elements */
  /* get gsl vector element */
  int index = scm_to_int (scm_index);
  double element = gsl_vector_get (smob->gsl_vector, index);
  SCM scm_element = scm_int2num (element);

  /* invoke the update function */
  if (scm_is_true (smob->update_func))
    scm_call_0 (smob->update_func);

  scm_remember_upto_here_1 (gsl_vector_smob);

  return (scm_element);
}

SCM
set_all_gsl_vector (SCM gsl_vector_smob, SCM scm_index)
{
  struct gsl_vector_smob *smob;

  /* check smob is of a suitable type */
  scm_assert_smob_type (gsl_vector_tag, gsl_vector_smob);

  /* retrieve immediate word of smob */
  smob = (struct gsl_vector_smob *) SCM_SMOB_DATA (gsl_vector_smob);

  /* set gsl vector elements */
  gsl_vector_set_all (smob->gsl_vector,\
      scm_num2double (scm_index, 0, "set_all_gsl_vector ()"));

  /* invoke the update function */
  if (scm_is_true (smob->update_func))
    scm_call_0 (smob->update_func);

  scm_remember_upto_here_1 (gsl_vector_smob);

  return (SCM_UNSPECIFIED);
}

SCM
zero_gsl_vector (SCM gsl_vector_smob)
{
  struct gsl_vector_smob *smob;

  /* check smob is of a suitable type */
  scm_assert_smob_type (gsl_vector_tag, gsl_vector_smob);

  /* retrieve immediate word of smob */
  smob = (struct gsl_vector_smob *) SCM_SMOB_DATA (gsl_vector_smob);

  /* zero the gsl vector */
  gsl_vector_set_zero (smob->gsl_vector);

  /* invoke the update function */
  if (scm_is_true (smob->update_func))
    scm_call_0 (smob->update_func);

  scm_remember_upto_here_1 (gsl_vector_smob);

  return (SCM_UNSPECIFIED);
}

size_t
free_gsl_vector (SCM gsl_vector_smob)
{
  struct gsl_vector_smob *smob = (struct gsl_vector_smob *)
    SCM_SMOB_DATA (gsl_vector_smob);

  /* free resources used by the smob */
  gsl_vector_free (smob->gsl_vector);

  return (0);
}

int
print_gsl_vector (SCM gsl_vector_smob, SCM port, scm_print_state *pstate)
{
  struct gsl_vector_smob *smob = (struct gsl_vector_smob *)
    SCM_SMOB_DATA (gsl_vector_smob);

  SCM scm_vector = scm_from_gsl_vector (smob->gsl_vector);

  /*gsl_vector_fprintf (stdout, smob->gsl_vector, "%f");*/

  scm_puts ("#<gsl-vector ", port);
  scm_display (scm_vector, port);
  scm_puts (">", port);
  scm_newline (port);

  /* non-zero means success */
  return (1);
}

/* convert from the gsl to scm vector type */
SCM scm_from_gsl_vector (gsl_vector *gsl_vector)
{
  /* initialize, make once input is checked */
  SCM scm_vector = NULL;

  /* check that gsl_vector is allocated */
  if (gsl_vector)
  {
    /* make the scm vector */
    scm_vector = scm_c_make_vector (gsl_vector->size, SCM_UNSPECIFIED);

    int i;

    for (i = 0; i < gsl_vector->size; i++)
    {
      /* get a gsl vector element */ 
      double gsl_element = gsl_vector_get (gsl_vector, i);

      /* convert the double into an SCM inum */ 
      SCM scm_element = scm_double2num (gsl_element);

      /* set the i-th element in the vector to scm_element */
      scm_vector_set_x (scm_vector, scm_int2num (i), scm_element);
    }
  }

  return (scm_vector);
}

/* convert from the scm to gsl vector type */
gsl_vector* scm_to_gsl_vector (SCM scm_vector)
{
  /* initialize, allocate once input is checked */
  gsl_vector *gsl_vector = NULL;

  /* check that input is a scm vector */
  if (scm_is_vector (scm_vector))
  {
    /* allocate gsl vector */
    int length = scm_c_vector_length (scm_vector);
    gsl_vector = gsl_vector_alloc (length);

    int i;

    for(i = 0; i < length; i++)
    {
      /* get the ith element of the scm vector */ 
      SCM scm_element = scm_vector_ref (scm_vector, scm_int2num (i));

      /* convert it into a C double */
      double gsl_element = scm_num2double (scm_element, 0, "scm_to_gsl_vector ()");

      /* assign it to the gsl vector element */
      gsl_vector_set (gsl_vector, i, gsl_element);
    }
  }

  return (gsl_vector);
}
