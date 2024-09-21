How do you maximize a window and keep it maximized
without a window manager?

If your Xsession just runs a terminal emulator,
for example, when making a minimal installer image,
and does not run a window manager, this requires some work.

This is a short C program that does this. (without polling)

It maximizes the named window and when a screen change event occurs,
it maximizes the window again.

run it on its own or as a service,
as long as it is ran the first time while the program is running
for the initial maximization.

use xprop to get the window name to provide.

on nix using X11:
```bash
# If you wanted to maximize a terminal emulator, find its name with xprop
# For alacritty, it is "Alacritty"
nix run github:BirdeeHub/maximizer -- Alacritty
```

other X11:

also requires:

- gcc

```bash
git clone https://github.com/BirdeeHub/maximizer
cd maximizer && ./build.sh

# If you wanted to maximize a terminal emulator, find its name with xprop
# For alacritty, it is "Alacritty"
./maximize_program Alacritty
```

Why?

I made an installer iso with the following window manager configuration,
and the terminal emulator was tiny...

```nix
services.xserver.enable = true;
services.displayManager.defaultSession = "xterm-installer";
services.xserver.desktopManager.session = (let
  maximizer = "${inputs.maximizer.packages.${pkgs.system}.default}/bin/maximize_program";
  launchScript = pkgs.writeShellScript "mysh" /*bash*/ ''
    # a tiny c program that uses libX11 to make xterm fullscreen.
    ${maximizer} xterm > /dev/null 2>&1 &
    # tmux launcher script
    exec ${tx}/bin/tx
  '';
in [
  { name = "xterm-installer";
    start = /*bash*/ ''
      ${pkgs.xorg.xrdb}/bin/xrdb -merge ${pkgs.writeText "Xresources" ''
        xterm*termName: xterm-256color
        xterm*faceName: ${nerd_font_string} Nerd Font
        xterm*faceSize: 12
        xterm*background: black
        xterm*foreground: white
        xterm*title: xterm
        xterm*loginShell: true
      ''}
      ${pkgs.xterm}/bin/xterm -name xterm -e ${launchScript} &
      waitPID=$!
    '';
  }
]);
```
