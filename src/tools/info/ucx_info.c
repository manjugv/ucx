/**
* Copyright (C) Mellanox Technologies Ltd. 2001-2014.  ALL RIGHTS RESERVED.
*
* See file LICENSE for terms.
*/

#include "ucx_info.h"

#include <ucs/config/parser.h>
#include <ucs/config/global_opts.h>
#include <getopt.h>


static void usage() {
    printf("Usage: ucx_info [options]\n");
    printf("Options are:\n");
    printf("  -v         Version\n");
    printf("  -d         Devices\n");
    printf("  -c         Configuration\n");
    printf("  -a         Show also hidden configuration\n");
    printf("  -b         Build configuration\n");
    printf("  -y         Type information\n");
    printf("  -f         Fully decorated output\n");
    printf("  -t <name>  Print information for a specific transport\n");
    printf("\n");
}

int main(int argc, char **argv)
{
    ucs_config_print_flags_t print_flags;
    unsigned print_opts;
    char *tl_name;
    int c;

    print_opts  = 0;
    print_flags = 0;
    tl_name     = NULL;
    while ((c = getopt(argc, argv, "fahvcydbst:")) != -1) {
        switch (c) {
        case 'f':
            print_flags |= UCS_CONFIG_PRINT_CONFIG | UCS_CONFIG_PRINT_HEADER | UCS_CONFIG_PRINT_DOC;
            break;
        case 'a':
            print_flags |= UCS_CONFIG_PRINT_HIDDEN;
            break;
        case 'c':
            print_flags |= UCS_CONFIG_PRINT_CONFIG;
            break;
        case 'v':
            print_opts |= PRINT_VERSION;
            break;
        case 'd':
            print_opts |= PRINT_DEVICES;
            break;
        case 'b':
            print_opts |= PRINT_BUILD_CONFIG;
            break;
        case 'y':
            print_opts |= PRINT_TYPES;
            break;
        case 's':
            print_opts |= PRINT_SYS_INFO;
            break;
        case 't':
            tl_name = optarg;
            break;
        case 'h':
        default:
            usage();
            return -1;
        }
    }

    if ((print_opts == 0) && (print_flags == 0)) {
        usage();
        return -2;
    }

    if (print_opts & PRINT_VERSION) {
        print_version();
    }

    if (print_opts & PRINT_SYS_INFO) {
        print_sys_info();
    }

    if (print_opts & PRINT_BUILD_CONFIG) {
        print_build_config();
    }

    if (print_opts & PRINT_TYPES) {
        print_type_info(tl_name);
    }

    if (print_flags & UCS_CONFIG_PRINT_CONFIG) {
        ucs_global_opts_print(stdout, "global configuration", print_flags);
        print_ucp_config(print_flags);
        print_uct_config(print_flags, tl_name);
    }

    if (print_opts & PRINT_DEVICES) {
        print_uct_info(print_opts, print_flags, tl_name);
    }
    return 0;
}
