#!/bin/bash
#
# Not one GUI toolkit is compiled into the converter binary.
#
# This is the promise the second binary exists for, and it is one no reading of
# the CMake files can prove: what proves it is the produced file. The binary is
# inspected twice over - through the shared libraries it loads and through the
# symbols it imports - so a toolkit reaching it either way is caught.

. "$(dirname "$0")/common.sh"

itest_init "$@"

# The runtime libraries of the four interchangeable toolkits of this project
# and of the stack each of them stands on.
FORBIDDEN_LIBRARIES='libgtk|libgtkmm|libgdk|libglibmm|libgiomm|libatkmm|libpangomm|libcairomm|libsigc|libQt[0-9]|libwx_'

# The namespaces those toolkits put into the symbol table of whoever uses them.
FORBIDDEN_SYMBOLS='Gtk::|Gdk::|Glib::|Gio::|QApplication|QWidget|wxApp|wxWindow'

if ! command -v ldd > /dev/null 2>&1 ; then
    itest_fail "No ldd available to inspect ${CONVERTER_BINARY}"
fi

LINKED_LIBRARIES="$(ldd "${CONVERTER_BINARY}")"

if printf '%s' "${LINKED_LIBRARIES}" | grep -Eq -- "${FORBIDDEN_LIBRARIES}" ; then
    itest_fail "A GUI toolkit is linked into ${CONVERTER_BINARY}:
$(printf '%s' "${LINKED_LIBRARIES}" | grep -E -- "${FORBIDDEN_LIBRARIES}")"
fi

# The symbol check catches a toolkit which was linked statically, which the
# loaded libraries above would say nothing about. It is skipped rather than
# failed when the binary carries no symbol table at all, since a stripped
# binary is a legitimate build of this project.
if command -v nm > /dev/null 2>&1 ; then
    IMPORTED_SYMBOLS="$(nm -C "${CONVERTER_BINARY}" 2> /dev/null || true)"

    if [[ -n "${IMPORTED_SYMBOLS}" ]] ; then
        if printf '%s' "${IMPORTED_SYMBOLS}" | grep -Eq -- "${FORBIDDEN_SYMBOLS}" ; then
            itest_fail "A GUI toolkit symbol is compiled into ${CONVERTER_BINARY}:
$(printf '%s' "${IMPORTED_SYMBOLS}" | grep -E -- "${FORBIDDEN_SYMBOLS}" | head -20)"
        fi
    fi
fi
