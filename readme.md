Fan patch attempts for the original PlanetSide.

## Building the mouse patch dll
1. Install the Visual Studio Build Tools. You'll need the Visual C++ compiler, maybe the Windows Platform SDK as well.
2. Inside build_mouse_patch.bat, modify the path of vcvarsall.bat to match your Visual Studio installation.
3. Run build_mouse_patch.bat.
4. Copy dinput8.dll from build_mouse_patch\\ to your PlanetSide game folder.
5. Run the game. A console window will open up alongside the game if it's working.

- Vehicle steering wheels won't work.
- Zoomed weapons are still busted.
- I still feel stickiness to 2 axes.
- Who knows what else is broken by this.
