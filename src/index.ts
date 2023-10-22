import { NativeModules, Platform } from 'react-native';
import type { gifsicleType } from './type';

const LINKING_ERROR =
  `The package 'react-native-gifsicle' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo Go\n';

// @ts-expect-error
const isTurboModuleEnabled = global.__turboModuleProxy != null;

// Keep this to sync auto install with the native code
const GifsicleModule = isTurboModuleEnabled
  ? require('./spec/NativeGifsicle').default
  : NativeModules.GifsicleMain;

const GifsicleTurboModule = GifsicleModule
  ? GifsicleModule
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

const installed = GifsicleTurboModule.install();
if (!installed) {
  throw new Error(LINKING_ERROR);
}

const GifsicleTurbo: gifsicleType = global as any;

export default GifsicleTurbo;
