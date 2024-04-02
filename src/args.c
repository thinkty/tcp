#include "args.h"

int check_args(int argc, char * const * argv, args_t * args)
{
	args->handler = MODE_UNDEF;
	args->port = 0;

	int opt;
	while ((opt = getopt(argc, argv, "hp:tcn:")) != -1) {

		/* Handler already defined */
		if (args->handler != MODE_UNDEF && (opt == 'p' || opt == 't' || opt == 'c')) {
			eprintf("Cannot use multiple handlers");
			eprintf(USAGE, argv[0]);
			return ERR;
		}

		/* Port number already defined */
		if (args->port != 0 && opt == 'n') {
			eprintf("Cannot use multiple ports");
			eprintf(USAGE, argv[0]);
			return ERR;
		}

		switch (opt) {
		case 'p': /* thread pool handler */
			if ((args->pool_size = (uint32_t) atoi(optarg)) == 0) {
				eprintf("Failed to parse thread pool size");
				return ERR;
			}
			args->handler = MODE_TPOOL;
			break;
		case 't': /* thread handler */
			args->handler = MODE_THREAD;
			break;
		case 'c': /* child process handler */
			args->handler = MODE_CHILDP;
			break;
		case 'n': /* port number */
			if ((args->port = (uint16_t) atoi(optarg)) == 0) {
				eprintf("Failed to parse port number");
				return ERR;
			}
			break;
		case 'h': /* help */
			eprintf(USAGE, argv[0]);
			return ERR;
		default: /* unrecognized option */
			eprintf(USAGE, argv[0]);
			return ERR;
		}
	}

	if (args->port == 0 || args->handler == MODE_UNDEF) {
		eprintf(USAGE, argv[0]);
		return ERR;
	}

	return OK;
}

