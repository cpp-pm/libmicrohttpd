/*
     This file is part of libmicrohttpd
     Copyright (C) 2017 Christian Grothoff, Silvio Clecio (silvioprog)

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
/**
 * @file msgs_i18n.c
 * @brief example for how to use translate libmicrohttpd messages
 * @author Christian Grothoff, Silvio Clecio (silvioprog)
 */

/*
 * suposing you are in Brazil:
 *
 * # generate the PO file
 * $ msginit --input=po/libmicrohttpd.pot --locale=pt_BR --output=libmicrohttpd.po
 * # open the generated .po in any program like Poedit and translate the MHD messages; once done, let's go to the test:
 * mkdir -p src/examples/locale/pt_BR/LC_MESSAGES
 * mv libmicrohttpd.mo libmicrohttpd.po src/examples/locale/pt_BR/LC_MESSAGES
 * cd src/examples/
 * gcc -o msgs_i18n msgs_i18n.c -lmicrohttpd
 * export LANGUAGE=pt_BR
 * ./msgs_i18n
 * # it may print: Opção inválida 4196490! (Você terminou a lista com MHD_OPTION_END?)
 *
 * # tip: if you get any problem in your i18n application, you can debug it using `strace` tool, e.g:
 * $ strace -e trace=open ./msgs_i18n
 * 
 */

#include <stdio.h>
#include <locale.h>
#include <libintl.h>
#include <microhttpd.h>

#ifndef _
#define _(fm) dgettext("libmicrohttpd", fm)
#endif

static int
ahc_echo(void *cls,
         struct MHD_Connection *cnc,
         const char *url,
         const char *mt,
         const char *ver,
         const char *upd,
         size_t *upsz,
         void **ptr) {
    return MHD_NO;
}

static void
error_handler(void *cls,
              const char *fm,
              va_list ap) {
    vprintf(_(fm), ap);
}

int
main() {
    setlocale(LC_ALL, "");
    /* notice I'm using PO files in the directory "libmicrohttpd/src/examples/locale", please change it matching
     * where the MHD PO files are installed */
    bindtextdomain("libmicrohttpd", "locale");
    MHD_start_daemon(MHD_USE_SELECT_INTERNALLY | MHD_FEATURE_MESSAGES | MHD_USE_ERROR_LOG,
                     8080,
                     NULL, NULL,
                     &ahc_echo, NULL,
                     MHD_OPTION_EXTERNAL_LOGGER, &error_handler, NULL
                     /* MHD_OPTION_END - to raise the error "Invalid option ..." we are going to translate */);
    return 1; /* purposely */
}
