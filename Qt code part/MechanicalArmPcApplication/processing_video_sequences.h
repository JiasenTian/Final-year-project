#ifndef PROCESSING_VIDEO_SEQUENCES
#define PROCESSING_VIDEO_SEQUENCES



class process_video_data{
  public:

  process_video_data();
  void sendData();
  void transformVideoData(int x, int y, int z);
  char* readData();
  private:
  char transformFromIntToChar(int);
};

#endif // PROCESSING_VIDEO_SEQUENCES

