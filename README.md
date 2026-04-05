# Aim Trainer System (Unreal Engine 5, C++)

## Overview
A modular aim training system designed to simulate high-intensity target scenarios while tracking player performance metrics in real time.

## Key Features
- Dynamic target spawning using NavMesh
- Multi-round gameplay system with varying mechanics:
  - Falling targets (gravity-based)
  - Exploding targets (physics impulses)
  - AI-controlled moving targets
- Real-time performance tracking:
  - Accuracy
  - Shots fired
  - Shots hit
- UI feedback system for player improvement

## System Architecture
- `AAimTrainingCharacter`
  - Handles shooting, accuracy tracking, and player state
- `TargetSpawner`
  - Manages spawning, lifecycle, and replacement of targets
- `GameMode`
  - Controls round flow and session timing

## Technical Highlights
- Line tracing for hit detection
- NavMesh-based spawning for valid placement
- Timer-based session management
- Modular round system for extensibility

## Future Improvements
- Save/load system for player stats
- Difficulty scaling system
- Backend integration for persistent stats
