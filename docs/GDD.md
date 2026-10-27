# 7 Seconds to Do! — Game Design Document (GDD)

> **Memorize the path. You have 7 seconds!**  
> An independent game by **Hadryn**.

| Field | Details |
|---|---|
| Document status | Initial consolidated GDD; ongoing planning |
| Reference playable version | **Alpha v0.0.4_7** (approved integrated build) |
| Developer / public identity | **Hadryn** |
| Proposed genre | Platformer with puzzle, precision, and time-trial elements |
| Project status | Alpha development |
| Planned distribution | Free, without ads or mandatory purchases |
| Narrative | No story; gameplay-focused |

> **Transparency note:** This document combines confirmed design decisions with future goals. **Planned does not mean implemented.** Updating the GDD does not modify the playable game.

### Current approved version

**Alpha v0.0.4_7** has passed final integrated gameplay testing. Level 3, **The Gauntlet**, combines moving platforms, falling Gravity Platforms, enemy patrols, and camera tracking. The two original levels are **Zigzagging** (formerly *Zigue-Zagueando*) and **Parkour**, with their maps preserved. The original turbo trail was restored in Level 3, and temporary notifications were removed.

### Protagonist concept — Keyla

**Keyla** has been approved as the game's future mascot and protagonist: a **living padlock** whose name plays on the word *key*. She collects the key that unlocks the exit door. Her name is **Keyla** in **Brazilian Portuguese, English, and Spanish**. The current black square placeholder and **16 × 16 px rectangular hitbox** remain unchanged in Alpha v0.0.4_7. Her character design, expressions, and animations are planned for **Alpha v0.1.0 — The Great Update**. This is a visual/identity plan, not a change to the current collision geometry.

## 1. Game Overview

**7 Seconds to Do!** is an independent platform game combining **memorization, speed, learning, and varied challenges**. Players first explore and memorize a route; once they collect the red key, they have **exactly seven seconds** to reach the exit door.

The experience aims to balance **learning and urgency**. Failure should encourage an enjoyable retry instead of excessive punishment.

**Positioning:** *Easy to learn, hard to master.* The intended audience includes casual players, platformer fans, speedrunners, and players who enjoy memory and reasoning challenges.

**Creator's vision:** make this the first officially published independent game by Hadryn.

## 2. Core Gameplay Loop

Every level has **two mandatory stages**, in this order:

### 2.1 Presentation

- Players explore the environment and memorize the route and obstacles.
- There is no seven-second countdown during memorization.
- Reaching the corresponding door ends Presentation and begins Challenge.

### 2.2 Challenge

1. The player searches for the **red key**.
2. **Collecting the key** opens the green door, increases movement speed, and immediately starts the timer.
3. The player must touch the door within **7 seconds**; contact from any side counts.
4. If time expires, the door closes, the key respawns in its original location, and the player **remains in their current position**.
5. Attempts are **unlimited**, with no penalty for the number of failures.

The countdown is always seven seconds for every level. There are no planned bonus seconds or shorter time limits based on difficulty.

### 2.3 Future Puzzles

In future levels, the key may be hidden or require previously learned mechanics to find or reach it. The seven-second countdown still begins **only when the key is collected**.

## 3. Physics and Controls — Approved Foundation

**Alpha v0.0.4_5** was approved through practical testing, with particular improvements to responsiveness and jump reliability near platform edges. **Alpha v0.0.4_6** preserves these controls while adding horizontal camera tracking.

| Rule | Agreed specification |
|---|---|
| Player | Square, currently **16 × 16 px** |
| Horizontal movement | Immediate response, without horizontal acceleration or inertia |
| Normal speed | **175 px/s** |
| Speed after key collection | **230 px/s**, while the timer is active |
| Jump | Fixed height of approximately **6 tiles / 96 px** in open space |
| Air control | Full control, including immediate direction reversal |
| Coyote time | Enabled; reference value **0.10 s** |
| Jump buffer | Enabled; reference value **0.10 s** |
| Platforms | Fully solid on all four sides |
| Green door | Stage completes upon contact from any side, including an edge |
| Timer | Strict **7 seconds**, with no extra completion grace period |
| Timeout | Respawns only the key, closes the door, and returns movement to normal speed |

The coyote-time and jump-buffer durations are reference parameters that may receive technical tuning; their continued presence is an approved design decision.

### 3.1 Camera and Level Dimensions — Implemented in Alpha v0.0.4_6

- The gameplay viewport remains **640 × 320 px**, using **16 × 16 px tiles**.
- Level height remains fixed at **20 tiles**.
- Level width may exceed the viewport and varies by level.
- The camera scrolls **horizontally only**, staying within level boundaries.
- Directional framing targets approximately **35% of screen width** while moving right and **65%** while moving left.
- Direction changes use a smoothed camera transition, accepted for this Alpha despite minor remaining visual discomfort during reversals.
- A **120-tile-wide Camera Laboratory** level was added to validate long-level behavior.

## 4. Campaign Structure

| Final-game goal | Quantity |
|---|---:|
| Worlds | **6** |
| Levels per world | **5** |
| Total levels | **30** |
| Stages per level | **2**: Presentation + Challenge |
| Planned playable stages | **60** |
| Target first-completion time | **30 minutes to 1 hour** |

The campaign is intended to be sequential, with progressive difficulty. Each mechanic should be introduced, practiced, and later combined with others. The duration is a **design target**, not a tested result for a complete game.

### 4.1 Worlds

Each world will have five levels. Concepts discussed so far:

- **Starting world / fundamentals:** running, jumping, and route memorization; includes the two original levels.
- **Fire:** a fire-themed setting and related hazards.
- **Ice:** an ice-themed setting and related hazards.
- **Wind:** wind-themed challenges.
- **Two additional worlds:** not yet defined.

**Note:** final world names, order, and world-specific mechanics have not been approved.

### 4.2 Levels in the Current Alpha

| Level | Name | Content |
|---|---|---|
| 1 | **Zigzagging** | Presentation + Challenge |
| 2 | **Parkour** | Presentation + Challenge |
| 3 (published in v0.0.4_6) | **Camera Laboratory** | Presentation + Challenge; 120 tiles wide; to be replaced by the unreleased Stage 3 below |
| 3 (v0.0.4_7 candidate) | **The Gauntlet** | Presentation + Challenge; 150 tiles wide; horizontal/vertical platforms, Gravity Platforms, and enemies |

The intended **30-level campaign** is still in planning. The experimental camera level is not necessarily a final campaign level. The number of current levels should not be treated as a percentage of total development completion.

## 5. Planned Future Mechanics

The following mechanics have been selected for **planning**, without implying they have already been implemented:

- Moving platforms, horizontally or vertically;
- Collapsing platforms;
- Spikes and traps;
- Buttons and other mechanisms;
- Teleporters;
- Gravity changes;
- Enemies or moving obstacles;
- Dash (a fast burst of movement);
- Hidden-key puzzles requiring previously learned mechanics.

### 5.1 Mechanics Discussed for the Next Experimental Update

The proposed **Alpha v0.0.4_7** focuses on testing moving platforms and enemies, in small, separately validated gameplay experiments. Design topics discussed include horizontal/vertical movement, carrying the player, patrol behavior, fatal or stompable enemies, and difficulty variations. These remain **planned** until implemented, tested, and approved.

**Design principle:** deliver varied and fresh challenges while preserving the central seven-second mechanic.

## 6. Progression, Scoring, and Competition — Planning

The project envisions:

- Sequential progression and level unlocking;
- Saved progress;
- Personal records;
- Performance medals or stars;
- Rankings among players.

**Still to define:** how records are calculated, medal requirements, the technical leaderboard implementation, and progress persistence. These systems **must not be presented as current Alpha features**.

## 7. Visual Direction

The selected direction is **minimalist**, potentially with **modern geometric elements**. Pixel art is an influence rather than a strict requirement; clarity and stable gameplay take priority.

### 7.1 Current Visual Elements

- Predominantly white background;
- Black square player;
- Yellow platforms;
- Red key;
- Red door when closed, green when open.

### 7.2 Protagonist — Planning

The officially approved future protagonist is **Keyla**, a living padlock with personality, facial expressions, and small animations. The existing square placeholder remains until **Alpha v0.1.0 — The Great Update**. This **does not imply a narrative**: the project intentionally focuses on gameplay.

Future worlds may have distinct visual identities while keeping obstacles and critical objects immediately recognizable.

## 8. Audio — Planning

### 8.1 Music

**Dynamic music** is planned to create different moods:

- **Presentation:** supports observation and memorization;
- **Challenge:** reinforces urgency, particularly during the countdown.

A final music genre has not been selected.

### 8.2 Sound Effects

Selected direction: **retro / 8-bit**.

Planned sounds include movement and jumps, collecting the key, countdown warnings, door opening/closing, stage completion, timeout, environments/obstacles, and protagonist expressions.

Because unlimited retries are central to the experience, sounds should remain noticeable without becoming exhausting.

## 9. Audience, Platforms, and Release

**Target audiences:** casual players, platformer fans, speedrunners, reasoning and memory enthusiasts, children and teenagers, and general audiences. An official age rating has not yet been established.

**Planned platforms:**

1. Browser (**Web / HTML5**);
2. **Windows**;
3. **Android**;
4. **Consoles** (a future possibility subject to platform requirements and adaptations).

The strategy is **gradual release**; the exact rollout order beyond web-first has not been finalized. The current implementation uses HTML5/JavaScript in a browser.

**Business model:** free, without ads or mandatory purchases.

**Intended launch languages:** Brazilian Portuguese, English, and Spanish. The current playable interface is in Brazilian Portuguese; GitHub documentation is maintained in English.

**Release date:** not defined.

## 10. Creative References

The developer has cited **Celeste** and **Geometry Dash**, particularly for platforming challenges, memorization, and puzzle possibilities. The project should develop its own identity without reproducing those games' content.

## 11. Current Alpha vs. Future Vision

| Aspect | Alpha v0.0.4_6 | Planned full release |
|---|---|---|
| Levels | 2 original + 1 experimental camera level | 30 campaign levels |
| Presentation and Challenge | Implemented | Mandatory in every level |
| Timer and key | Implemented | Core rule retained |
| Refined physics | Implemented and approved | Retained and continuously tested |
| Dynamic camera / longer levels | Implemented | Further level-design use |
| Themed worlds | Not implemented | 6 worlds |
| Advanced obstacles and puzzles | Not implemented | New mechanics and hidden keys |
| Character expressions | Planned | Small animations and expressions |
| Music and sound | Planning | Dynamic music and 8-bit effects |
| Records, medals, rankings | Planning | Systems to be specified |
| Internationalization | Planning | PT-BR, EN, and ES |
| Distribution | Browser prototype / GitHub Pages | Free, with gradual platform expansion |

## 12. Development Principles

1. **Learn before racing:** Presentation is fundamental to the game's identity.
2. **Seven seconds means seven seconds:** a fixed, strict countdown after collecting the key.
3. **Failure should not be excessively punishing:** unlimited attempts and a quick challenge reset.
4. **Predictable controls:** precision depends on immediate response and consistent collisions.
5. **Progressive difficulty:** introduce mechanics individually, then combine them.
6. **Variety without losing identity:** each world brings different situations while preserving the core concept.
7. **Transparent documentation:** distinguish implemented features, approved work, and ideas that remain planned.

## 13. Outstanding Design Decisions

- Names and order of the six worlds;
- Specific mechanics for each themed world;
- Record and medal criteria;
- Online ranking operation;
- Save system;
- Detailed character animations and expressions;
- Music composition and sound implementation;
- World and level-selection interfaces;
- Mobile and console control adaptations;
- Release schedule and playtesting with players.

---

**Developed by Hadryn**  
**7 Seconds to Do! — Memorize the path. You have 7 seconds!**

*Planning document. The integrated Alpha v0.0.4_7 build has been approved; other future features remain planned.*
