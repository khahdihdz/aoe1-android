# AoE1 Web

Cross-device browser port of the open-source Age of Empires 1-compatible engine from [FolkertVanVerseveld/aoe](https://github.com/FolkertVanVerseveld/aoe).

## Platform strategy

This repository is **Web-only**. There is no Android APK, Gradle project, Android NDK target, or platform-specific native app.

The browser client is designed to run on:
- Desktop and laptop browsers (Windows, macOS, Linux, ChromeOS).
- Android phones and tablets.
- iPhone and iPad browsers.
- Touch, mouse, and keyboard input where supported.

The web UI uses responsive layout and browser-native APIs. Actual engine execution requires a successful Emscripten/WebAssembly port; the current page is a development launcher, not yet a playable game.

## Current status

- Responsive web launcher and local game-data folder selection.
- Browser capability checks for WebAssembly and WebGL 2.
- Pinned upstream source in `upstream/aoe` (Apache-2.0).
- GitHub Pages and Cloudflare Pages deployment workflows.
- Emscripten engine integration, browser networking, and playable game runtime remain in progress.

## Game data

A legitimate copy of AoE / Rise of Rome data is required. Proprietary game assets are not included in this repository. Selected files are read locally by the browser and are not uploaded by the launcher.

## Deployment

### GitHub Pages
Enable GitHub Pages with **GitHub Actions** as the deployment source. The `Web - GitHub Pages` workflow publishes the `web/` directory.

### Cloudflare Pages
Create a Pages project named `aoe1-web` and add repository secrets:
- `CLOUDFLARE_API_TOKEN`
- `CLOUDFLARE_ACCOUNT_ID`

The `Web - Cloudflare Pages` workflow deploys the `web/` directory. A successful validation run may skip deployment when secrets are missing.

## Development

Serve the `web/` directory with any static HTTP server. Do not open `index.html` directly as some browser APIs require HTTP(S).

See [docs/WEB_PORT_PLAN.md](docs/WEB_PORT_PLAN.md) for the engine audit, Emscripten strategy, browser networking constraints, and milestones.

## Credits and license

The upstream engine is licensed under Apache-2.0. See [upstream/aoe](https://github.com/FolkertVanVerseveld/aoe) and retain its license and notices when distributing derived builds.
