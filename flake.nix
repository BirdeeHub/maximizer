{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
  };
  outputs = { nixpkgs, ... }: let
    forAllSys = nixpkgs.lib.genAttrs nixpkgs.lib.platforms.all;
    APPNAME = "maximize_program";
    appOverlay = final: prev: {
      ${APPNAME} = final.callPackage ./. { inherit APPNAME; };
    };
  in {
    overlays.default = appOverlay;
    packages = forAllSys (system: let
      pkgs = import nixpkgs {
        inherit system;
        overlays = [ appOverlay ];
      };
    in {
      default = pkgs.${APPNAME};
    });
  };
}
