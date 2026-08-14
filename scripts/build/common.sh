if [[ -z ${PROJECT_ROOT} ]] ; then
    echo "#### No PROJECT_ROOT var given (scripts common.sh)"
    exit 1
fi

. "${PROJECT_ROOT}/scripts/common.sh"

delete_previous_build()
{
    SCRIPT_BUILD_DIR=$1

    if [[ -z ${SCRIPT_BUILD_DIR} ]] ; then
        log_fatal "No build dir path provided"
    fi

    if [[ -d "${SCRIPT_BUILD_DIR}" ]] ; then
        log "Erasing previous build dir: ${SCRIPT_BUILD_DIR}"

        rm -fr "${SCRIPT_BUILD_DIR}"

        if [[ -d "${SCRIPT_BUILD_DIR}" ]] ; then
            log_fatal "Fail to delete previous Release build dir at ${SCRIPT_BUILD_DIR}"
        fi
    fi
}

check_build_dir()
{
    SCRIPT_BUILD_DIR=$1

    if [[ -z ${SCRIPT_BUILD_DIR} ]] ; then
        log_fatal "No build dir path provided"
    fi

    if [[ ! -d "${SCRIPT_BUILD_DIR}" ]] ; then
        log_fatal "No build dir for build/install @ ${SCRIPT_BUILD_DIR}"
    fi

    log "Build dir is available @ ${SCRIPT_BUILD_DIR}"
}

# Drops the script own control flags from the forwarded arguments, leaving
# CMAKE_ARGS with what cmake may receive. The -D defines are accepted by the
# configure stage only, so the first argument tells whether to keep them.
filter_script_args()
{
    KEEP_DEFINES=$1
    shift

    CMAKE_ARGS=()

    for arg in "$@" ; do
        case "${arg}" in
            --no-reconfigure|--install)
                ;;
            -D*)
                if [[ ${KEEP_DEFINES} == yes ]] ; then
                    CMAKE_ARGS+=("${arg}")
                fi
                ;;
            *)
                CMAKE_ARGS+=("${arg}")
                ;;
        esac
    done
}

INSTALL_PREFIX="/usr"
RELEASE_BUILD_DIR="${PROJECT_ROOT}/build/release"
DEBUG_BUILD_DIR="${PROJECT_ROOT}/build/debug"