{ stdenv, libX11, libXrandr, ... }:
stdenv.mkDerivation rec {
  name = "maximize_window";
  src = ./maximizer.c;
  dontUnpack = true;
  buildInputs = [ libX11 libXrandr ];
  buildPhase = ''
    $CC -o ${name} $src -lX11 -lXrandr
  '';
  installPhase = ''
    mkdir -p $out/bin
    cp ${name} $out/bin/
  '';
}
