import React, { useState } from 'react';
import {
  StyleSheet,
  Text,
  SafeAreaView,
  Pressable,
  Image,
  useWindowDimensions,
  View,
} from 'react-native';
import { stat } from 'react-native-fs';
import RNGifsicle from 'react-native-gifsicle';
import prettyBytes from 'pretty-bytes';
import * as ImagePicker from 'react-native-image-picker';

export default function App() {
  const [sourceImage, setSourceImage] = useState('');
  const [sourceSize, setSourceSize] = useState('');
  const [destImage, setDestImage] = useState('');
  const [destSize, setDestSize] = useState('');
  const { width, height } = useWindowDimensions();

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
            setSourceImage(uri);
            console.log('source uri=>', uri);
            const sourceFileStat = await stat(uri);
            setSourceSize(prettyBytes(sourceFileStat?.size || 0));
            const compressedUri = await RNGifsicle.compressGif(uri, {
              // lossy: 100,
              // colors: 250,
              // scale_x: 2,
              // scale_y: 2,
              // optimize:3
              // height: 250,
              // width: 300,
            });
            console.log(compressedUri, 'compressedUri');
            setDestImage(compressedUri);
            const destFileStat = await stat(compressedUri);
            setDestSize(prettyBytes(destFileStat?.size || 0));
          }
        }
      }
    ).catch((err) => {
      console.log(err, 'error');
    });
  };

  return (
    <SafeAreaView style={styles.container}>
      <View style={styles.section}>
        {!!sourceImage && (
          <>
            <Text style={styles.title}>Source Gif: {sourceSize}</Text>
            <Image
              source={{ uri: sourceImage }}
              resizeMode="contain"
              style={{
                height: height * 0.3,
                width: width,
              }}
            />
          </>
        )}
      </View>
      <Pressable style={styles.button} onPress={onPress}>
        <Text style={styles.buttonText}>Select Gif</Text>
      </Pressable>
      <View style={styles.section}>
        {!!destImage && (
          <>
            <Text style={styles.title}>Compressed Gif: {destSize}</Text>
            <Image
              source={{ uri: destImage }}
              resizeMode="contain"
              style={{
                height: height * 0.3,
                width: width,
              }}
            />
          </>
        )}
      </View>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'space-between',
  },
  title: {
    fontSize: 30,
    fontWeight: 'bold',
    marginVertical: 16,
  },
  button: {
    padding: 15,
    backgroundColor: 'black',
    borderRadius: 8,
    marginVertical: 16,
  },
  buttonText: { color: '#FFFFFF', fontWeight: 'bold' },
  section: { flex: 1, alignItems: 'center' },
});
