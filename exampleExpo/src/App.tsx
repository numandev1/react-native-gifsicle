import React, { useEffect, useState } from 'react';
import { StyleSheet, View, Text } from 'react-native';
import RNGifsicle from 'react-native-gifsicle';
export default function App() {
  const [jniValue, setJniValue] = useState('');
  const [publicValue, setPublicValue] = useState('');
  useEffect(() => {
    const value1 = RNGifsicle.secureFor('secure3');
    console.log(value1, 'value1');
    setJniValue(value1);
    const value2 = RNGifsicle.APP_NAME;
    setPublicValue(value2);
  }, []);
  return (
    <View style={styles.container}>
      <Text>KEY FROM SECURE (JNI): {jniValue}</Text>
      <Text>KEY FROM PUBLIC: {publicValue}</Text>
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
