# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.1] - 2021-02-20

### Added
- `OUTPUT_DIR` option to control where artifacts are built.
- Implemented unsubscribe functionality.
- Implemented installation targets for system-wide installation. (Thanks xloem!)

### Changed
- Upgraded SimpleDBus to version 1.1.1
- Increased the speed of the Linux async thread polling to every 50us.
- Linux implementation uses OnServicesResolved to trigger `callback_on_device_connected`.

### Fixed
- Added missing declaration for `NativeBleInternal::unsubscribe` in MacOS. (Thanks Mach1!)


## [1.0.0] - 2020-08-14

### Added
- External facing API with specific implementations for Windows, Linux and macOS.

