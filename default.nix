{ stdenv, libX11, libXrandr, ... }:
stdenv.mkDerivation rec {
  name = "maximize_alacritty";
  src = ./maximizer.c;
  dontUnpack = true;
  buildInputs = [ libX11 libXrandr ];
  buildPhase = ''
    gcc -o ${name} $src -lX11 -lXrandr
  '';
  installPhase = ''
    mkdir -p $out/bin
    cp ${name} $out/bin/
  '';
}
