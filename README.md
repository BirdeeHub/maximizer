How do you maximize a window and keep it maximized
without a window manager?

If your session just runs a terminal emulator,
for example, when making a minimal installer image,
and does not run a window manager, this requires some work.

This is a short C program that does this.

It maximizes the named window and when a screen change event occurs,
it maximizes the window again.

run it on its own or as a service.

use xprop to get the window name to provide.

packaged only for nix, but to build on another system,
`-lX11 -lXrandr` are the only extra flags you need to provide to your c compiler.
