#include <stdlib.h>
#include <libguile.h>
#include "guile_gsl.h"

static void inner_main (void *closure, int argc, char **argv)
{
  /* initialize gsl */
  init_gsl ();

  /* load test script */
  scm_c_primitive_load ("test.scm");

  /*scm_shell (argc, argv);*/

  return;
}

int main (int argc, char *argv[])
{
  scm_boot_guile (argc, argv, inner_main, 0);
  
  return (EXIT_SUCCESS);
}
