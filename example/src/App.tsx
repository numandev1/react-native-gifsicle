import React, { useState } from 'react';
import { StyleSheet, Text, View, Pressable } from 'react-native';
import RNKeys from 'react-native-gifsicle';

import * as ImagePicker from 'react-native-image-picker';

export default function App() {
  const [jniValue] = useState(RNKeys.APP_NAME);
  const [publicValue] = useState('');

  const onPress = () => {
    ImagePicker.launchImageLibrary(
      {
        mediaType: 'photo',
      },
      async (result: ImagePicker.ImagePickerResponse) => {
        if (result.assets) {
          const source: any = result.assets[0];
          if (source) {
            const uri = source.uri;
            console.log(RNKeys.compressGif, 'sss');
            const _uri = uri;
            console.log(_uri, 'uri');
            const resultt = await RNKeys.compressGif(_uri, {});
            console.log('uri', resultt);
          }
        }
      }
    ).catch((err) => {
      console.log(err, 'error');
    });
  };

  return (
    <View style={styles.container}>
      <Text>KEY FROM SECURE (JNI): {jniValue}</Text>
      <Text>KEY FROM PUBLIC: {publicValue}</Text>
      <Pressable
        style={{ padding: 15, backgroundColor: 'red' }}
        onPress={onPress}
      >
        <Text>Test</Text>
      </Pressable>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
});
