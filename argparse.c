/* vim: set ts=8 sw=8 tw=0 noet : */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "argparse.h"

static char *to_upper(const char *name)
{
	int len = strlen(name);
	char *upper = malloc(len+1), *c = upper;
	for (int i = 0; i < len; i++) {
		*upper = toupper(*name);
		name++;
		upper++;
	}
	return c;
}

int add_option(struct Options *opts, const char sname, char *name,
	       	char *lname, char *desc, bool req, bool is_flag)
{
	if ((sname < 'A' || sname > 'z') || (sname > 'Z' && sname < 'a')) {
		printf("Invalid option short name\n");
		exit(-1);
	}
	if (opts->num_opts == 0) {
		opts->opts = (struct Option *) malloc(sizeof(struct Option));
	} else {
		opts->opts = (struct Option *) realloc (opts->opts,
				sizeof(struct Option) * (opts->num_opts + 1));
	}
	name = to_upper(name);
	struct Option *option =  opts->opts + opts->num_opts;
	*option = (struct Option) {
		.sname = sname,
		.name = name,
		.lname = lname,
		.desc = desc,
		.req = req,
		.is_flag = is_flag
	};
	if (option->req)
		opts->num_req++;
	opts->num_opts++;
	return 0;
}

void print_help(const struct Options *opts,const char *progname)
{
	int num_opts = opts->num_opts;
	struct Option opt;

	printf("Usage: %s ", progname);
	for (int i = 0; i < num_opts; i++) {
		opt = opts->opts[i];
		if (opt.req)
			printf(" -%c %s", opt.sname, opt.name);
		else
			printf(" [-%c %s]", opt.sname, opt.name);
	}
	putchar('\n');
	
	for (int i = 0; i < num_opts; i++) {
		opt = opts->opts[i];
		printf("-%c, --%-20s  %s\n", opt.sname, opt.lname, opt.desc);
	}
}

static int get_num_args(const int argc, const char **argv)
{
	int nargs = 0;
	char map[128] = {0};	/* to find duplicate options */
	for (int i = 1; i < argc; i++) {
		if (*argv[i] != '-' || map[(int)argv[i][1]] != 0)
			return -1;

		map[(int)argv[i][1]]++;
		nargs++;
		if (i + 1 != argc && *argv[i+1] != '-')
			i++;
	}
	return nargs;
}

/*
 * Returns: -1 -- option not found or some mis format
 *           0 -- parse success
 *           1 -- not found; option is a flag or optional
 */
static int parse_arg(struct Option *opt, struct Arg *arg, int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		if (*argv[i] != '-')
			continue;
		
		if (argv[i][1] != opt->sname && strcmp(argv[i] + 2, opt->lname))
			continue;

		arg->opt = opt;
		if (opt->is_flag)
			return 0;
		if (i + 1 == argc || *argv[i+1] == '-')
			return -1;
		arg->value = argv[i+1];
		return 0;
	}
	printf("not found\n");
	if (opt->is_flag || !opt->req)
		return 1;
	return -1;
}

struct Args *parse_args(const struct Options *opts,
		const int argc, char **argv)
{
	struct Args *args = (struct Args *) malloc (sizeof (struct Args));
	if ((args->num_args = get_num_args(argc, (const char **)argv)) == -1
			|| args->num_args > opts->num_opts)
		return NULL;
	args->args = (struct Arg *) malloc (sizeof (struct Arg)*args->num_args);
	for (int i = 0, j = 0; i < opts->num_opts; i++) {
		switch (parse_arg(opts->opts + i, args->args + j, argc, argv)) {
		case 0: j++;
			break;
		case -1:
			return NULL;
		default:
			;
		}
	}
	return args;
}

char *get_arg(const struct Args *args, const char sname, const char *name)
{
	if (sname == 0 && name == NULL)
		return NULL;

	struct Arg *arg = args->args;
	for (int i = 0; i < args->num_args; i++, arg++) {
		if (sname != 0 && arg->opt->sname == sname) {
			if (arg->opt->is_flag)
				return "1";
			return arg->value;
		}
		if (name != NULL && (!strcmp(arg->opt->name, name)
				       	|| !strcmp(arg->opt->lname, name))) {
			if (arg->opt->is_flag)
				return "1";
			return arg->value;
		}
	}
	return NULL;
}
