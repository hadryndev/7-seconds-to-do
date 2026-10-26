# Changelog — 7 Seconds to Do!

A record of major changes to the game, developed by **Hadryn**.

> **Current approved playable version:** Alpha v0.0.4_6  
> **Status:** Alpha development. The number after the underscore identifies an incremental build within the corresponding Alpha version.
>
> This document records confirmed project changes. Earlier intermediate builds without individual detailed records are grouped together; they were not necessarily public releases.

## Alpha v0.0.4_7 — Moving Platforms Lab 01 (Unreleased)

**Status: experimental laboratory tested and approved; not yet integrated into the published game.**

### Moving Platform Prototype
- Tested horizontally and vertically moving platforms that carry the player.
- Preserved the approved **16 × 16 px rectangular collision box** while rendering the character as a more cartoonish rounded square.
- Kept existing movement and jump physics unchanged.

### Bugs Found and Fixed During Laboratory Testing
- **Player teleportation on moving-platform underside collisions:** jumping into the underside of the vertical (purple) platform could unexpectedly snap the player to either horizontal edge, particularly while holding a direction or reversing direction mid-jump. **Fix:** separated horizontal side-entry checks from vertical floor/ceiling collision handling to prevent an underside hit from triggering sideways repositioning.
- **Moving platforms resetting on player death/respawn:** platform motion restarted whenever the player respawned. **Fix:** separated player-only respawn from a full laboratory reset, allowing moving platforms to continue from their current positions and directions.

### Validation
- The developer retested the previously reported reproduction cases, including jumping into the purple platform from below and changing direction, and reported no remaining glitches.
- Respawning now leaves the moving platforms running continuously in the laboratory.

> These corrections were verified in the **v0.0.4_7 laboratory prototype**, not in the currently published **Alpha v0.0.4_6**. They will become part of the public changelog when integrated and released.

## Alpha v0.0.4_6 — Dynamic Camera and Expanded Levels

**Status: approved and deployed.**

### New Features
- Added a **dynamic horizontal camera**, with framing near **35% of screen width when moving right** and **65% when moving left**.
- Smoothed camera transitions when reversing direction. Further comfort adjustments may be considered based on feedback.
- Implemented **variable-width levels**, while retaining a fixed **20-tile height** and **640 × 320 px viewport**.
- Added **Level 3 — Camera Laboratory**, an experimental level **120 tiles wide**, with Presentation and Challenge stages.
- Kept camera movement within level boundaries.

### Compatibility
- Preserved both original levels: **Zigue-Zagueando** and **Parkour**.
- Preserved approved player physics, movement speeds, collisions, and the strict seven-second gameplay loop.
- Updated the version label, credits metadata, and in-game changelog.

## Alpha v0.0.4_5 — Physics, Collision Handling, and Documentation

**Status: approved after hands-on testing.**

### Improvements
- Immediate horizontal movement without noticeable acceleration or inertia.
- Complete horizontal control in mid-air, including instant direction changes.
- Maintained speeds of **175 px/s** normally and **230 px/s** after collecting the key.
- Fixed jump height of approximately **6 tiles (96 px)** in open space.
- **Coyote time** of **0.10 seconds**, allowing a brief window to jump after leaving a platform edge.
- **Jump buffer** of **0.10 seconds**, retaining a jump command shortly before landing.
- Fully solid platforms on all four sides, with separate horizontal and vertical collision resolution.
- Green door contact recognized from any side, including edges.
- Strict **7-second** countdown after collecting the key, using a monotonic clock without intentional extra time.
- When time expires, the door closes, the key respawns, and the character remains at their current position.

### Interface and Documentation
- Added the **Changelog** option to the menu.
- Updated credits with **Hadryn**, the official tagline, and a summary of GDD goals.
- Preserved the **Zigue-Zagueando** and **Parkour** stage layouts.

### Test Results
- The developer approved the build and reported significantly improved responsiveness.
- Jump inputs near platform edges became more reliable with the timing adjustments.

## Alpha v0.0.4_1 to v0.0.4_4 — Level Expansion and Revisions

**Consolidated record of builds preceding v0.0.4_5.**

- Brought **Level 1 — Zigue-Zagueando** and **Level 2 — Parkour** together in one game.
- Established the **Presentation** (memorize the route) and **Challenge** (collect the key and reach the door) sequence.
- Adjusted routes and platforms, especially in Parkour, using stage-editor maps.
- Revised jumping and positioning to suit the layouts.
- Improved transitions between the two levels.

> Individual changes cannot reliably be attributed to each build from _1 through _4, so they are grouped here.

## Alpha v0.0.3_1 to v0.0.3_6 — Gameplay Prototypes

**Consolidated record of the Alpha v0.0.3 series.**

- Established the game in **HTML5, CSS, and JavaScript (Canvas 2D)**.
- Developed the minimal visual language: **black player square**, **yellow platforms**, **red key**, and **green/red door** on a white background.
- Built and revised the Level 1 route.
- Implemented and refined movement, jumping, collisions, and transitions between Presentation and Challenge.
- Introduced reference speeds of **175 px/s** and **230 px/s**.
- Improved the seven-second timer and its visual warnings.
- Developed a **separate level editor** to design and export layouts.
- Began preparing Level 2 — Parkour.

> Individual details of builds _1 through _6 were not recorded separately.

---

## Planned for Future Versions — Not Yet Implemented

The following features belong to the **Game Design Document (GDD)** and **must not be interpreted as available in the current Alpha**:

- A **30-level campaign** across **6 worlds**, with five levels each.
- Themed worlds such as fire, ice, and wind.
- Moving or collapsing platforms, traps, switches, teleporters, gravity changes, moving hazards, and dash.
- Enemies and more advanced platforming encounters.
- Puzzles involving hidden keys and previously learned mechanics.
- A geometric protagonist with expressions and small animations.
- Dynamic music and retro / 8-bit sound effects.
- Progress saving, personal bests, medals or stars, and rankings.
- Brazilian Portuguese, English, and Spanish interfaces for the intended launch.
- Free distribution and gradual expansion to other platforms.

**Tagline:** *Memorize the path. You have 7 seconds!*

**Developer:** Hadryn
