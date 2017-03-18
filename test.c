/* vim: set ts=8 sw=8 tw=0 noet : */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../argparse.h"

int main(int argc, char **argv)
{
	Options opts = new_options();

	/* 
	 * void add_option(opts, sname, name, lname, desc, is_optional, is_flag)
	 * arguments:
	 *      opts        -- Options variable created with `new_options()`
	 *      char sname  -- short name(only character without '-') ex: 'N' for -N
	 *      char *name  -- name of the option
	 *      char *lname -- long option name(without '--')
	 *              ex: 'long-option' for --long-option
	 *      char *desc  -- description 
	 *      bool is_required -- Is this a required argument(true/false)
	 *      bool is_flag -- Is this a flag(true/false)
	 *      note: is_flag => !is_required
	 */
	add_option(opts, 'N', "nusers", "num-users",
			"Number of users", true, false);
	add_option(opts, 'W', "collision_window", "collision-window",
			"Initial collision window size", true, false);
	add_option(opts, 'p', "packet_gen_prob", "packet-gen-prob",
			"Packet generation probability", false, true);
	add_option(opts, 'M', "max_packets", "max-packets",
			"Maximum number of packets to send", true, false);

	/* parse arguments and store them in args */
	Args args = parse_args(opts, argc, argv);
	if (args == NULL) {
		print_help(opts, argv[0]);
		exit(0);
	}
	puts("Arguments parsed successfully");
	print_args(args);
	/* 
	 * char *get_arg(args, sname, name)
	 * arguments:
	 *      args -- Args variable initialise with return value of parse_args()
	 *      sname   --  short name of option
	 *      name    --  name or long name of option
	 *      note: atleast one of (sname, name) should be given
	 *  returns:
	 *      pointer to string if option is parsed successfully
	 *      NULL otherwise
	 *      check for NULL before you call any other functions on the result
	 */
	char *arg = get_arg(args, 'N', NULL);
	if (arg != NULL)
		printf("nusers = %d\n", atoi(arg));
	arg = get_arg(args, 'W', NULL);
	if (arg != NULL)
		printf("collision window = %d\n", atoi(arg));
	return 0;
}
