#!/bin/bash

SDL_HEADER="/opt/homebrew/include/SDL3/SDL_scancode.h"
OUTPUT="include/Q-Tip/Window/Keys.h"

{
    echo "#pragma once"
    echo
    echo "namespace QTip {"
    echo
    echo "enum class Key {"

    awk '
        /SDL_SCANCODE_/ {
            if ($0 ~ /^[[:space:]]*SDL_SCANCODE_[A-Za-z0-9_]+[[:space:]]*=/) {
                name = $1
                sub(/^SDL_SCANCODE_/, "", name)
                value = $3
                sub(/,$/, "", value)

                printf "    Key_%s = %s,\n", name, value
            }
        }
    ' "$SDL_HEADER"

    echo "};"
    echo
    echo "}"
} > "$OUTPUT"
