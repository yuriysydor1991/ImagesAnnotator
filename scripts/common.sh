log()
{
    echo -e "#### $(date '+%Y.%m.%d %H:%M:%S') $*"
}

log_fatal()
{
    log "FATAL $*"

    exit 1
}
