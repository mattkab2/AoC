with import (fetchTarball("channel:nixpkgs-unstable")) {};
clang13Stdenv.mkDerivation {
  name = "clang-nix-shell";
  buildInputs = [ bash valgrind ];
}
