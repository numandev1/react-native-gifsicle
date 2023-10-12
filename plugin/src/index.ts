import { ConfigPlugin, createRunOncePlugin } from '@expo/config-plugins';

import {
  withAndroidBuildscriptDependency,
  withAndroidGradleProperties,
} from './android';
import { withPreActionScript } from './ios';
import { PluginConfigType } from './pluginConfig';

/**
 * A config plugin for configuring `react-native-gifsicle`
 */
const withRnKeys: ConfigPlugin<PluginConfigType> = (config, props) => {
  //Android;
  config = withAndroidBuildscriptDependency(config, props);
  config = withAndroidGradleProperties(config, props);

  // IOS
  config = withPreActionScript(config, props);

  return config;
};

const pak = require('../../package.json');
export default createRunOncePlugin(withRnKeys, pak.name, pak.version);
