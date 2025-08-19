{
  description = "InfiniTime";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-24.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        nodePackages = pkgs.nodePackages;

      in {
        devShell = pkgs.mkShell {
          name = "infinitime-dev-shell";

          buildInputs = with pkgs; [
            cmake
            ninja
            nodejs
            nodePackages.lv_font_conv
            SDL2
          ];
        };
      });
}
