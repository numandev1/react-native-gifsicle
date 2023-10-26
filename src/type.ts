type CompressGifOptions = {
  lossy?: number; // rage [0-200]
  colors?: number; // rage [0-255]
  optimize?: number; // rage [1-3]
  height?: number;
  width?: number;
  scale_x?: number;
  scale_y?: number;
};

export type gifsicleType = {
  compressGif: (uri: string, options?: CompressGifOptions) => Promise<string>;
};
