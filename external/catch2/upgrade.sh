#!/usr/bin/bash

cd "$(dirname "$0")"

ASSETS="catch.hpp catch.hpp.asc"

LATEST=$(curl https://api.github.com/repos/catchorg/Catch2/releases/latest)

function get_asset() {
    URL=$(jq -r ".assets[] | select(.name == \"$1\") | .browser_download_url" <<< $LATEST)
    curl -LO $URL
}

get_asset "catch.hpp"
get_asset "catch.hpp.asc"
gpg --verify "catch.hpp.asc" || exit 1
rm "catch.hpp.asc"

VERSION=$(jq -r ".tag_name" <<< $LATEST)

echo "Catch.hpp is now at version $VERSION!"
