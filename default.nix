{ stdenv, libX11, libXrandr, APPNAME ? "REPLACE_ME", ... }:
stdenv.mkDerivation {
  name = APPNAME;
  src = ./maximizer.c;
  dontUnpack = true;
  buildInputs = [ libX11 libXrandr ];
  buildPhase = ''
    $CC -o ${APPNAME} $src -lX11 -lXrandr
  '';
  installPhase = ''
    mkdir -p $out/bin
    cp ${APPNAME} $out/bin/
  '';
}
