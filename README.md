# Timber (Tmber) — Simple SFML Axe Game

Short description
- Small C++ game built with SFML. Chop tree branches, avoid being crushed, collect score and extra time.

Requirements
- Windows (tested with Visual Studio).
- C++14-compatible compiler (MSVC).
- SFML (2.5.x recommended) — headers and libs must be available to the project.
- Project expects these asset folders at the executable working directory:
  - `graphics/` (background, tree, branch, cloud, bee, player, axe, log, rip, …)
  - `sound/` (chop.wav, death.wav, out_of_time.wav)
  - `fonts/` (KOMIKAP_.ttf)

Build with Visual Studio (recommended)
1. Open the solution / project in Visual Studio.
   - Use __Solution Explorer__ to find and open `Tmber.vcxproj`.
2. Configure SFML include & lib paths:
   - Right-click the project → __Properties__.
   - Under __Configuration__ choose the config you build (e.g., __Debug__ or __Release__).
   - Set SFML include directory under __VC++ Directories__ → __Include Directories__.
   - Set SFML lib directory under __VC++ Directories__ → __Library Directories__.
   - Under __C/C++__ → __General__ add any required preprocessor definitions if you use dynamic SFML linking (e.g., `SFML_STATIC` for static).
   - Under __Linker__ → __Input__ add required SFML .lib files (for dynamic linking: `sfml-graphics.lib`, `sfml-window.lib`, `sfml-system.lib`, `sfml-audio.lib`, etc.).
   - If using dynamic SFML DLLs, copy the SFML DLLs to the executable output folder (e.g., `Debug/` or `Release/`).
3. Build
   - Use __Build__ → __Build Solution__ or press __F7__.
   - If linking errors occur, verify the lib names and configuration (Debug vs Release may require `-d` suffixed libs).

Run
- Run from Visual Studio with __Debug__ → __Start Debugging__ (F5) or __Start Without Debugging__ (Ctrl+F5).
- Or run the produced `.exe` from the output directory; ensure the `graphics/`, `sound/`, and `fonts/` folders and any required DLLs are placed next to the exe.

Controls
- Left Arrow — Chop left (player moves left).
- Right Arrow — Chop right (player moves right).
- Enter — Start / Restart game.
- Escape — Quit.

Project layout (important files)
- `Tmber.cpp` — main game source (game loop, rendering, input, update logic).
- `graphics/` — image assets used by the game.
- `sound/` — audio files used by the game.
- `fonts/` — font files.
- `README.md` — this file.

Troubleshooting
- SFML include or linker errors:
  - Ensure you set correct include/lib paths and link the correct SFML libraries for the active configuration.
  - Match Debug/Release with debug or release SFML libs (or use the non-suffixed libs with dynamic linking).
- Missing assets (black screen / runtime errors):
  - The game loads relative paths like `graphics/background.png`. Run the exe with the working directory containing the `graphics/`, `sound/`, and `fonts/` folders.
- Window size / fullscreen issues:
  - The project creates a fullscreen window at 1928×1080. If your display differs, edit the `VideoMode` call in `Tmber.cpp` to match your resolution or replace `Style::Fullscreen` with `Style::Default` for windowed mode.

Contributing
- Keep C++14 compatibility.
- Add new art/sound into the respective assets folders; update resource file paths in `Tmber.cpp` as needed.

License
- No license indicated. Add an appropriate `LICENSE` file if you plan to publish or share.

If you want, I can:
- Add a prepared `README.md` file to the repo.
- Provide a short Visual Studio step-by-step (screenshots omitted) to configure SFML.
- Convert fullscreen to windowed mode by editing `Tmber.cpp`.
