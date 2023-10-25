<div align="center">
  <img src="./media/logo.gif">
  </br>
  <img width="500" src="./media/gifsicle.png">
</div>

<div align="center">

[![GitHub Repo stars](https://img.shields.io/badge/React_Native-20232A?style=for-the-badge&logo=react&logoColor=61DAFB)](#installation)
[![GitHub Repo stars](https://img.shields.io/badge/Expo-1B1F23?style=for-the-badge&logo=expo&logoColor=white)](#managed-expo)
[![GitHub Repo stars](https://img.shields.io/static/v1?style=for-the-badge&message=Discord&color=5865F2&logo=Discord&logoColor=FFFFFF&label=)](https://discord.gg/fgPHnZpH9d)
[![GitHub Repo stars](https://img.shields.io/github/stars/numandev1/react-native-gifsicle?style=for-the-badge&logo=github)](https://github.com/numandev1/react-native-gifsicle/stargazers)
![npm](https://img.shields.io/npm/dt/react-native-gifsicle?style=for-the-badge)

</div>

<p align="center"><b>REACT-NATIVE-GIFSICLE</b> is a react-native package, which helps us to <b>Compress</b> and <b>Manipulate</b>, It is a using <a href="https://github.com/kohler/gifsicle">Gifsicle</a> under the hood

# Table of Contents

<details>
<summary>Open Table of Contents</summary>

- [Installation](#installation)
  - [For React Native](#installation)
  - [Managed Expo](#managed-expo)
- [Basic Usage](#basic-usage)
- [Setup](#video)
  - [IOS](#ios)
  - [Android](#android)
  </details>

## Installation

```sh
yarn add react-native-gifsicle
```

##### [New Architecture (Turbo Module) Supported](https://reactnative.dev/docs/new-architecture-intro)

You can give feedback on [Discord channel](https://discord.gg/fgPHnZpH9d)

### Managed Expo

```
expo install react-native-gifsicle
```

Add the Gifsicle plugin to your Expo config (`app.json`, `app.config.json` or `app.config.js`):

```json
{
  "name": "my app",
  "plugins": ["react-native-gifsicle"]
}
```

Finally, compile the mods:

```
expo prebuild
```

## Basic Usage

## Javascript

```js
import RNGifsicle from 'react-native-gifsicle';

const manipulatedUri = await RNGifsicle.compressGif(uri, {
  lossy: 100,
  colors: 250,
  // scale_x: 2,
  // scale_y: 2,
  // optimize:3
  // height: 250,
  // width: 300,
});
```

## Setup

Install the package:

```
yarn add react-native-gifsicle
```

Link the library:

(Note: For React Native 0.60 or greater, [autolinking](https://reactnative.dev/blog/2019/07/03/version-60#native-modules-are-now-autolinked) is available)

or later. For earlier versions you need to manually link the module.)

```
react-native link react-native-gifsicle
```

### IOS

if cocoapods are used in the project then pod has to be installed as well:

```
(cd ios; pod install)
```

- **Manual Link (iOS)**

  1. In XCode, in the project navigator, right click `Libraries` ➜ `Add Files to [your project's name]`
  2. Go to `node_modules` ➜ `react-native-gifsicle` and add `Gifsicle.xcodeproj`
  3. Expand the `Gifsicle.xcodeproj` ➜ `Products` folder
  4. In the project navigator, select your project. Add `Gifsicle.a` to your project's `Build Phases` ➜ `Link Binary With Libraries`
  5. And go the Build Settings tab. Make sure All is toggled on (instead of Basic)
  6. Look for Header Search Paths and add `$(SRCROOT)/../node_modules/react-native-gifsicle/ios/**` as `non-recursive`

## API

### `compressGif(uri, options)`

Compresses a GIF located at the specified URI using the provided options.

#### Parameters

- `uri` (String): The URI of the GIF to compress.
- `options` (Object): The options for compression. Available options are:
    - `lossy` (Number): Alter image colors to shrink output file size at the cost of artifacts and noise. `default=200`
    - `optimize` (Number): Optimize output GIFs. `default=3`
    - `colors` (Number): Reduce the number of colors to N. `default=255`
    - `scale_x` (Number): Scale the image in the x-axis.
    - `scale_y` (Number): Scale the image in the y-axis.
    - `height` (Number): Resize the output GIF height.
    - `width` (Number): Resize the output GIF width.

#### Returns

- A Promise that resolves to a URI of the compressed GIF.

## Consider supporting with a ⭐️ [star on GitHub](https://github.com/numandev1/react-native-gifsicle/stargazers)

If you are using the library in one of your projects, consider supporting it with a star. It takes a lot of time and effort to keep this maintained and address issues and bugs. Thank you.

## Credits goes to

[Gifsicle](https://github.com/kohler/gifsicle) by [kohler](https://github.com/kohler)

[Author of this package](https://github.com/numandev1/react-native-gifsicle.git) by [Numan](https://github.com/numandev1)
