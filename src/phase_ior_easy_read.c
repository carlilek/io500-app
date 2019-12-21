#include <sys/stat.h>
#include <unistd.h>
#include <mpi.h>

#include <io500-phase.h>
#include <phase_ior.h>

typedef struct{
  char * api;
  int odirect;

  char * command;
  IOR_point_t * res;
} opt_ior_easy_read;

static opt_ior_easy_read o;

static ini_option_t option[] = {
  {"API", "The API to be used", 0, INI_STRING, "POSIX", & o.api},
  {"posix.odirect", "Use ODirect", 0, INI_BOOL, NULL, & o.odirect},
  {NULL} };


static void validate(void){

}


static double run(void){
  opt_ior_easy d = ior_easy_o;

  u_argv_t * argv = u_argv_create();
  ior_easy_add_params(argv);
  u_argv_push(argv, "-r");
  u_argv_push(argv, "-R");
  u_argv_push_default_if_set(argv, "-a", d.api, o.api);
  u_argv_push_default_if_set_bool(argv, "--posix.odirect", d.odirect, o.odirect);

  o.command = u_flatten_argv(argv);

  r0printf("exe=%s\n", o.command);
  if(opt.dry_run){
    u_argv_free(argv);
    return 0;
  }
  FILE * out = u_res_file_prep(p_ior_easy_read.name);
  return ior_process_read(argv, out, & o.res);
}


u_phase_t p_ior_easy_read = {
  "ior-easy-read",
  option,
  validate,
  run,
  0
};