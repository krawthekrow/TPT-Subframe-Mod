The Powder Toy (Subframe Chipmaker Mod)
=======================================

This is a mod of The Powder Toy for subframe chipmakers. Subframe technology refers to a collection of tricks used to reduce computation cycles in electronic components to a single frame.

For the full experience, add the lines from [autorun.example.lua](autorun.example.lua) into your autorun.lua.

Features
========

New features:
- Particle order reloading (shortcut: Shift-F5).
- Subframe animation (shortcut: Shift-Space). Runs the simulation particle-by-particle rather than frame-by-frame.
- Subframe recording (Lua: `tpt.record_subframe(true)`). Starts a recording of particle-by-particle simulation and automatically stops recording at the end of the frame. You can stop the recording with `tpt.record_subframe(false)` without stopping particle-by-particle simulation.
- (v1.1) Stack tool (shortcut: Shift-S). Dragging on a stack of particles unstacks them; selecting multiple particles in different positions stacks them in order of their positions.
- (v1.1) Stack mode (shortcut: Shift-D to toggle). Allows you to draw or paste over existing particles, and makes right-click delete one stacked particle at a time. This helps to make things like transparent DTEC.
- (v1.7) Config tool (shortcut: C). Sets particle properties in a few clicks. DRAY: Sets tmp, then tmp2. CRAY: Sets tmp2, then tmp. LDTC: Sets life, then tmp. DTEC/TSNS/LSNS/VSNS: Sets tmp2. FILT: Sets tmp. CONV: Sets tmp (click on another particle with the type you want to set the tmp to).
- (v1.8) Timelapse recording (Lua: `tpt.setrecordinterval(<frames>)`). Changes the interval that frames are captured when recording. Useful when making timelapses.
- (v1.10) Stack edit (shortcut: X, Shift-X, PageUp, PageDown, Home, End). Config tool, property tool, ctype-draw and subframe debugging (Shift-F) target particles at the selected depth. When stack mode is enabled (Shift-D), particles are created and deleted at the selected depth. Note that using the brush with stack edit messes with particle order, so this is best combined with automatic particle order reloading (`tpt.autoreload_enable(1)`).

New features enabled by the Lua command `tpt.autoreload_enable(1)`:
- Automatic particle order reloading: If you use the brush or do a copy-paste and then advance the simulation, the frame is completed and particle order is reloaded automatically.
- Backups: Overwriting a local save "<save>.cps" creates a backup of the file you're overwriting at "<save>.cps.backup". Subframe development is dangerous.

Property tool changes:
- P opens the property tool window rather than takes a screenshot. You can still take screenshots with F2.
- (v1.3) "filt:v" in property tool translates into (v<<8) + 125, to aid the configuration of CRAY that creates FILT with a preset tmp.
- (v1.6) 30th-bit handling. Adding "c:" before typing in a number into the property tool when setting ctype sets the 30th bit. For example, "c:50" sets the ctype to (50 | 0x20000000). Works with hex too -- just add "0x", like in "c:0xC0FFEE". (This used to be "c", but was changed due to collisions with element names.)
- (v1.11) Properties in property tool are rearranged so that electronic properties are more conveniently accessible when using arrow keys.
- (v1.12) Property tool stays on the selected property when an invalid value is entered.

HUD changes:
- Stacked particles are shown in the HUD.
- FILT ctypes are displayed in the HUD. In the original game, this was only shown for non-FILT coloured particles like BRAY.
- (v1.6) The property tool no longer sets the 31st and 32nd bits of colored particle ctypes.
- (v1.6) Ctype display modes (shortcut: Ctrl-J to cycle). This changes how the ctype of a colored particle is displayed in the debug HUD. The ctype is displayed in hex by default, but you can switch it back to decimal. For decimal, you can choose to enable 30th-bit handling. This ignores the 30th bit and, if the 29th bit is set, displays the ctype as a negative number.
- (v1.10) The HUD is slightly more compact.
- (v1.11) The subframe debugging position is shown in the HUD.
- (v1.11) CRAY FILT mode is shown in the debug HUD.
- (v1.12) CONV's tmp is presented as an element name, to the left of its ctype.
- (v1.14) WALL type is always shown in the top right HUD.

Other quality-of-life changes:
- Alt-F continues updating particles until it encouters an "interesting" update (create\_part, delete\_part or part\_change\_type called), and does not display any log messages until the frame is completed.
- You can paste and place stamps outside the simulation boundary.
- When pasting or placing stamps, the white dotted paste boundary is not shown.
- If you attempt to open a save or close the window with the ESC button (the case for closing with the 'X' button has not been implemented) when there are unsaved changes, a confirmation warning is displayed.
- If you attempt to save when you're in the middle of a frame, a confirmation warning is displayed.
- If you attempt to save when your particles are not in subframe order (and automatic particle order reloading is disabled), a confirmation warning is displayed.
- The position in frame is saved into undo history during subframe debugging, so you can undo into the middle of a frame without destroying everything.
- Applying deco to a stack applies it preferentially to energy particles.
- (v1.1) Creating CRAY with the brush gives it a ctype of SPRK automatically. If you want to create a ctype-less CRAY, use the property tool.
- (v1.10) Tab no longer changes the brush, since it conflicts with Alt-Tab. You can still change the brush with Shift-Tab.
- (v1.12) When pasting or placing stamps, you may use the WASD keys to adjust its position instead of the arrow keys.
- (v1.12) BRAY is visible even when its life is low.
- (v1.13) Paste and stamp previews respect changes to render options, including whether decorations are enabled.
- (v1.14) The ruler tool (enable with `tpt.setdebug(0x4)`, or `tpt.setdebug(0xC)` to use with subframe debug mode) shows dimensions not just for lines, but also for boxes.

New Lua interfaces:
- `tpt.autoreload_enable(enable: int)`: Enables autoreload if `enable = 1`, disables autoreload otherwise. Default: `enable = 1`.
- `tpt.record_subframe(start: bool)`: Start or stop subframe recording.
- `tpt.setrecordinterval(num_frames: int)`: Changes the recording interval to once every `num_frames` frames.
- `sim.reloadParticleOrder()`: Reloads particle order.
- (v1.13) `tpt.set_bray_life_brightness_threshold(thres: int)`: Make BRAY visible even when its life is low. `thres` is added to BRAY's alpha value when it is in the non-solid (i.e. `life != 2`) state.
- (v1.14) `event.prehuddraw`: Event that gets fired just before the HUD is rendered. This allows Lua scripts to draw interfaces below the HUD.

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
- Prevent Tab from changing the brush.
- Fix non-brush-created CRAY having ctype SPRK.
- Make subframe recording only accessible from Lua.
- Make particle order reloading faster.
- Make HUD more compact.
- Confirm when saving unclean state instead of reloading particle order.
- Only show stack tool warning once when holding down on stack.
- Add stack edit feature.

v1.11:
- Show subframe debugging position in the HUD.
- Show CRAY FILT mode in the debug HUD.
- Change stack mode shortcut to Shift-D.
- Prevent C from switching out of config tool.
- Rearrange properties in property tool.
- Re-enable autoreload when saving.
- If disabled, prevent autoreload when subframe debugging.
- Fix stack edit ctype display bug.
- Fix save location on command line open.
- Fix property tool rect/line bug.
- Fix interesting change detection not detecting killed particles.

v1.12:
- Merge changes in 96.2.
- Make stack edit snap to current stack when scrolling out of bounds.
- Change stack edit shortcut to X and Shift-X.
- Scroll through the stack when X/Shift-X is held down.
- Add Home, End, PageUp and PageDown shortcuts for stack edit.
- Make subframe debugging stop on particle selected by stack edit.
- Make stack tool stack and unstack based on cursor drag direction.
- Make C switch out of config tool to the previously selected tool.
- Make BRAY visible even when its life is low.
- Display CONV's tmp as an element name next to its ctype in the debug HUD.
- Indicate config tool and stack edit selections with a colored pointer.
- Highlight currently configured property with colors instead of brackets.
- Keep property tool on selected property when an invalid value is received.
- Fix CRAY overlay color scheme in the config tool.

v1.13:
- Fix automatic particle order reloading initialization bug.
- Prevent Alt-F from skipping uninteresting updates.
- Add Lua hook to set BRAY life brightness threshold.
- Make stamp previews aware of render settings.

v1.14:
- Allow stack tool to unstack to box.
- Extend config tool to VSNS tmp2.
- Extend stack mode to pasting and placing stamps.
- Extend ruler tool to show box dimensions.
- Put HUD particle info below the zoom window.
- Always show wall type in HUD.
- Show particle IDs in HUD when zoomed.
- Add Lua event that triggers before the HUD is drawn.
- Make BRAY life brightness offset also affect tmp=1 BRAY.
- Fix menu glitch when toggling config tool from deco menu.
- Fix vanilla file brower search query change bug.
