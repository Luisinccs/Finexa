# Multiplatform Conversion (macOS, iOS) for FinexaCommonApple

This project was converted from a macOS-only library to a multiplatform library supporting macOS and iOS.

## Steps taken:

1.  **Modify Project Build Settings**:
    *   Opened `FinexaCommonApple.xcodeproj/project.pbxproj`.
    *   Updated the `XCBuildConfiguration` sections (both Debug and Release).
    *   Changed `SDKROOT` from `macosx` to `iphoneos`. This allows Xcode to use the appropriate SDK depending on the target platform.
    *   Added `SUPPORTED_PLATFORMS` set to `"iphoneos iphonesimulator macosx"`. This enables the multiplatform capability in Xcode.
    *   Added `TARGETED_DEVICE_FAMILY` set to `"1,2"`. This specifies support for iPhone (1) and iPad (2).
    *   Added `IPHONEOS_DEPLOYMENT_TARGET` set to `17.5`.
    *   Updated `MACOSX_DEPLOYMENT_TARGET` to `14.5`.

2.  **Verification**:
    *   Ran `xcodebuild -project Common/Compiler/Apple/FinexaCommonApple.xcodeproj -scheme FinexaCommonApple -sdk macosx build` -> **SUCCEEDED**.
    *   Ran `xcodebuild -project Common/Compiler/Apple/FinexaCommonApple.xcodeproj -scheme FinexaCommonApple -sdk iphonesimulator build` -> **SUCCEEDED**.

3.  **Result**:
    *   The library can now be linked into both macOS and iOS applications.
