with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "nix-shell";
  buildInputs = [ bash valgrind ];
}
