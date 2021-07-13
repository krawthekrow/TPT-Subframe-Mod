The Powder Toy (Subframe Chipmaker Mod)
=======================================

This is a mod of The Powder Toy for subframe chipmakers. Subframe technology refers to a collection of tricks used to reduce computation cycles in electronic components to a single frame.

For the full experience, add the lines from [autorun.example.lua](autorun.example.lua) into your autorun.lua.

Features
========

New features:

- Particle order reloading (shortcut: Shift-F5).
- Subframe animation (shortcut: Shift-Space). Runs the simulation particle-by-particle rather than frame-by-frame.
- Subframe recording (shortcut: Shift-R). Starts a recording of the particle-by-particle simulation and automatically stops recording at the end of the frame. Recording can still be stopped with R but particle-by-particle simulation will still continue.
- (v1.1) Stack tool (shortcut: Shift-S). Clicking on a stack of particles unstacks them; selecting multiple particles in different positions stacks them in order of their positions.
- (v1.1) Stack mode (shortcut: Shift-semicolon to toggle). Allows you to draw over existing particles, and makes right-click delete one stacked particle at a time. This helps to make transparent DTEC.
- (v1.7) Config tool (shortcut: C). Sets particle properties in a few clicks. DRAY: Sets tmp, then tmp2. CRAY: Sets tmp2, then tmp. LDTC: Sets life, then tmp. DTEC/TSNS/LSNS: Sets tmp2. FILT: Sets tmp. CONV: Sets tmp (click on another particle with the type you want to set the tmp to).
- (v1.8) Timelapse recording (Lua: `tpt.setrecordinterval(<frames>)`). Changes the interval that frames are captured when recording. Useful when making timelapses.

New features enabled by the Lua command `tpt.autoreload_enable(1)`:

- Automatic particle order reloading: If you use the brush or do a copy-paste and then advance the simulation, the frame is completed and particle order is reloaded automatically.
- Backups: Overwriting a local save "<save>.cps" creates a backup of the file you're overwriting at "<save>.cps.backup". Subframe development is dangerous.

Property tool changes:
- P opens the property tool window rather than takes a screenshot. You can still take screenshots with F2.
- (v1.3) "filt:v" in property tool translates into (v<<8) + 125, to aid the configuration of CRAY that creates FILT with a preset tmp.
- (v1.6) 30th-bit handling. Adding "c:" before typing in a number into the property tool when setting ctype sets the 30th bit. For example, "c:50" sets the ctype to (50 | 0x20000000). Works with hex too -- just add "0x", like in "c:0xC0FFEE". (This used to be "c", but was changed due to collisions with element names.)

HUD changes:
- Stacked particles are shown in the HUD.
- FILT ctypes are displayed in the HUD. In the original game, this was only shown for non-FILT coloured particles like BRAY.
- (v1.6) The property tool no longer sets the 31st and 32nd bits of colored particle ctypes.
- (v1.6) Ctype display modes (shortcut: Ctrl-J to cycle). This changes how the ctype of a colored particle is displayed in the debug HUD. The ctype is displayed in hex by default, but you can switch it back to decimal. For decimal, you can choose to enable 30th-bit handling. This ignores the 30th bit and, if the 29th bit is set, displays the ctype as a negative number.

Other quality-of-life changes:
- Alt-F continues updating particles until it encouters an "interesting" update (create\_part, delete\_part or part\_change\_type called), and does not display any log messages until the frame is completed.
- You can paste and place stamps outside the simulation boundary.
- When pasting or placing stamps, the white dotted paste boundary is not shown.
- If you attempt to open a save or close the window with the ESC button (the case for closing with the 'X' button has not been implemented) when there are unsaved changes, a confirmation warning is displayed.
- Saving completes the frame and reloads particle order, so you don't get surprised by corrupted saves.
- The position in frame is saved into undo history during subframe debugging, so you can undo into the middle of a frame without destroying everything.
- Applying deco to a stack applies it preferentially to energy particles.
- (v1.1) Creating CRAY with the brush gives it a ctype of SPRK automatically. If you want to create a ctype-less CRAY, use the property tool.

Note that the original game already supports the following subframe debugging features (enable with the Lua command `tpt.setdebug(0x8)`):

- Shift-F updates all particles up to the particle where your mouse is at.
- Alt-F updates a single particle. This behaviour has been modified in this mod to continue updating particles until an "interesting" update occurs.

Changelog
=========

v1.0:
- Original release.

v1.1:
- Merge changes in snapshot.
- Add stack tool.
- Add stack mode.
- Give drawn CRAY a ctype of SPRK.

v1.2:
- Merge changes in 92.0.
- Change DTEC colour.

v1.3:
- Merge changes in 92.5.
- Add "filt:v" property tool syntax.

v1.4:
- Fix crash when pasting walls outside window boundaries.
- Notify in HUD when more than 5 particles are below the cursor.
- Warn instead of block when stacking more than 5 particles with stack tool.
- Unstack as many particles as possible when unstacking with stack tool.
- Merge changes in snapshot, including fix for Mac.
- Extend 30th-bit handling to PHOT.

v1.5:
- Don't trigger particle order reloading when SPRKing with brush.

v1.6:
- Merge changes in 93.3.
- Allow property tool to set temperature in kelvin.
- Make property tool 30th-bit handling opt-in.
- Allow spectral data to be displayed in hex and unmodified decimal.

v1.7:
- Merge changes in 94.0.
- Add Lua function to reload particle order.
- Fix bug with floats being treated as temp in the prop tool.
- Fix "filt:" prop tool shortcut.
- Add a config tool.
- Fix crash when pasting outside window boundaries.
- Add Lua function to set autoreload, separate from debug flags.
- Make subframe reloading, simulation and recording enabled by default.

v1.8:
- Merge changes in 94.1.
- Add Lua timelapse feature.

v1.9:
- Merge changes in 95.0.
- Fix stack tool pmap id bug.
- Fix prop tool particle name recognition bug.
- Extend config tool to TSNS/LSNS tmp2.
- Fix pasting/stamping ID reordering bug.

v1.10:
- Merge changes in 96.0.
- Fix config tool large stack crash bug.
- Fix local load unsaved changes bug.
- Change "c" prop tool shortcut to "c:".
- Move DRAY/DTEC coloring into Lua.
