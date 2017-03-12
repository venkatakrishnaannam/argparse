/* vim: set ts=8 sw=8 tw=0 noet : */
#include <stdbool.h>

struct Options {
	struct Option *opts;
	int num_opts;
	int num_req;
};

struct Option {
	char sname;
	char *name;
	char *lname;
	char *desc;
	bool req;
	bool is_flag;
};

struct Arg {
	struct Option *opt;
	char *value;
};

struct Args {
	struct Arg *args;
	int num_args;
};

int add_option(struct Options *opts, const char sname, char *name,
	       	char *lname, char *desc, bool req, bool is_flag);
void print_help(const struct Options *opts, const char *progname);
struct Args *parse_args(const struct Options *opts,
		const int argc, char **argv);
char *get_arg(const struct Args *args, const char sname, const char *name);
