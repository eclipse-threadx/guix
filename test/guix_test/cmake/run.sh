#!/bin/bash

cd $(dirname $0)

# GUIX has no test harness of its own. This clone supplies the runner, the CMake
# toolchain file and the ThreadX library the tests link against, so which ref it
# sits on is part of the build definition rather than a convenience.
threadx_ref=v6.4.1_cert
threadx_url=https://github.com/eclipse-threadx/threadx.git

# True when the clone sits on $threadx_ref. A shallow clone holds one commit, so
# HEAD says where the clone is and not what it was asked for. A branch ref leaves
# that branch checked out and is answered by its name; a tag ref leaves HEAD
# detached, and is answered by the tag resolving to HEAD. Asking only whether the
# tag is present would pass on any clone that carries the tag set.
on_ref()
{
    [ "$(git -C threadx rev-parse --abbrev-ref HEAD 2>/dev/null)" = "$threadx_ref" ] && return 0

    local tag_commit
    tag_commit=$(git -C threadx rev-parse -q --verify "refs/tags/$threadx_ref^{commit}" 2>/dev/null) || return 1
    [ "$tag_commit" = "$(git -C threadx rev-parse HEAD 2>/dev/null)" ]
}

if [ -d threadx ]; then
    # Compared against threadx's own path because git searches upwards: asked
    # inside a directory that is not a clone, it answers for the repository this
    # tree sits in, and the harness would then be taken for pinned.
    if [ "$(git -C threadx rev-parse --show-toplevel 2>/dev/null)" != "$(cd threadx && pwd -P)" ]; then
        echo "$(pwd)/threadx is not a git clone. Remove it and run again." >&2
        exit 1
    fi

    # Replaced rather than fetched onto the ref, so that one path serves a branch
    # and a tag alike and leaves the clone as a first clone would leave it.
    if ! on_ref; then
        echo "Test harness clone is at $(git -C threadx rev-parse --short HEAD 2>/dev/null), not $threadx_ref. Replacing it." >&2
        rm -rf threadx || exit 1
        git -c advice.detachedHead=false clone $threadx_url --depth 1 --branch $threadx_ref threadx || exit 1
    fi
else
    git -c advice.detachedHead=false clone $threadx_url --depth 1 --branch $threadx_ref threadx || exit 1
fi

echo "Test harness: threadx $threadx_ref at $(git -C threadx rev-parse --short HEAD)"

# Recreated on every run, so a link left pointing elsewhere cannot be reused.
ln -sfn threadx/scripts/cmake_bootstrap.sh .run.sh

./.run.sh $*
