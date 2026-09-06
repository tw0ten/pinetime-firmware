{
  outputs =
    { self, nixpkgs }:
    let
      lib = nixpkgs.lib;
    in
    {
      devShells = lib.genAttrs lib.systems.flakeExposed (
        system:
        let
          pkgs = import nixpkgs {
            inherit system;
            config = {
              allowUnfree = true;
              permittedInsecurePackages = [
                "python3.14-ecdsa-0.19.2"
              ];
            };
          };

          nrf5-sdk = pkgs.nrf5-sdk;
          gcc-arm-embedded = pkgs.gcc-arm-embedded;
        in
        {
          default = pkgs.mkShell {
            packages = [
              nrf5-sdk
              gcc-arm-embedded
              pkgs.cmake
              pkgs.ninja
              pkgs.nodejs
              pkgs.lv_font_conv
              pkgs.SDL2
              pkgs.adafruit-nrfutil
              (pkgs.python3.withPackages (
                pkgs: with pkgs; [
                  cryptography
                  intelhex
                  click
                  cbor
                ]
              ))
            ];

            ARM_NONE_EABI_TOOLCHAIN_PATH = "${gcc-arm-embedded}";
            NRF5_SDK_PATH = "${nrf5-sdk}/share/nRF5_SDK";
          };
        }
      );
    };
}
