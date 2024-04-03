#include "args.h"

int check_args(int argc, char * const * argv, args_t * args)
{
	int opt;
	args->mode = 0;
	args->port = 0;

	while ((opt = getopt(argc, argv, "hp:tcn:")) != -1) {

		/* Mode already defined */
		if ((args->mode & MASK_MODE) && (opt == 'p' || opt == 't' || opt == 'c')) {
			eprintf("Cannot use multiple modes");
			return ERR;
		}

		/* Port number already defined */
		if (args->port && opt == 'n') {
			eprintf("Cannot use multiple ports");
			return ERR;
		}

		switch (opt) {
		case 'n':
			if ((args->port = (uint16_t) atoi(optarg)) == 0) {
				eprintf("Failed to parse port number");
				return ERR;
			}
			break;
		case 'p':
			if ((args->pool_size = (uint8_t) atoi(optarg)) == 0) {
				eprintf("Failed to parse thread pool size");
				return ERR;
			}
			args->mode |= MODE_TPOOL;
			break;
		case 't':
			args->mode |= MODE_THREAD;
			break;
		case 'c':
			args->mode |= MODE_CHILDP;
			break;
		case 'h':
			eprintf(USAGE, argv[0]);
			return ERR;
		default: /* unrecognized option */
			eprintf(USAGE, argv[0]);
			return ERR;
		}
	}

	if (args->port == 0 || (args->mode & MASK_MODE) == 0) {
		eprintf("Port or mode unset (%u,0x%X)", args->port, args->mode & MASK_MODE);
		eprintf(USAGE, argv[0]);
		return ERR;
	}

	return OK;
}

