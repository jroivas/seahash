#!/bin/bash

MYDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
output_dir="$(pwd)/test_output"
mkdir -p "${output_dir}"

ls "${MYDIR}/test"/*.input | while read f; do
    b=$(basename "$f" .input)
    "${MYDIR}"/seahash "$f" > "${output_dir}/${b}.output"
    d=$(diff -u "${MYDIR}/test/${b}.output" "${output_dir}/${b}.output")
    if [ -n "$d" ] ; then
        echo "*** ERROR at test $b:"
        echo "$d"
    fi
done
