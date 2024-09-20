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
