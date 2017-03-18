/* vim: set ts=8 sw=8 tw=0 noet : */
#include <stdbool.h>

struct Option {
	char sname;
	char *name;
	char *lname;
	char *desc;
	bool req;
	bool is_flag;
};

struct Options {
	struct Option *opts;
	int num_opts;
	int num_req;
};
typedef struct Options * Options;

struct Arg {
	struct Option *opt;
	char *value;
};

struct Args {
	struct Arg *args;
	int num_args;
};
typedef struct Args * Args;

Options new_options();
int add_option(Options opts, const char sname, char *name,
	       	char *lname, char *desc, bool req, bool is_flag);
void print_help(const Options opts, const char *progname);
struct Args *parse_args(const Options opts,
		const int argc, char **argv);
char *get_arg(const Args args, const char sname, const char *name);
void print_args(Args args);
