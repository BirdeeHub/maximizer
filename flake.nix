{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
  };
  outputs = { nixpkgs, ... }@inputs: let
    forEachSystem = with builtins; f: let # flake-utils.lib.eachSystem
      op = attrs: system: let
        ret = f system;
        op = attrs: key: attrs // {
          ${key} = (attrs.${key} or { })
          // { ${system} = ret.${key}; };
        };
      in foldl' op attrs (attrNames ret);
    in foldl' op { } nixpkgs.lib.platforms.all;

    appOverlay = final: prev: {
      # any pkgs overrides made here will be inherited in the arguments of default.nix
      # because we used final.callPackage instead of prev.callPackage
      resize_alacritty = final.callPackage ./. {};
    };
  in {
    overlays.default = appOverlay;
  } // (
    forEachSystem (system: let
      pkgs = import nixpkgs { inherit system; overlays = [ appOverlay ]; };
    in{
      packages = {
        default = pkgs.resize_alacritty;
      };
    })
  );
}
