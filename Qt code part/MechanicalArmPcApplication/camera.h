#ifndef CAMERA
#define CAMERA

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <windows.h>
#include <setupapi.h>
#include <usbiodef.h>
#include <usbioctl.h>
#include <devguid.h>
#include <winioctl.h>
#include <initguid.h>
#include <dbt.h>
#include <map>
#include <wchar.h>


class Camera{
  public:
  int chooseCamera();
  int showCameraCount();

};

#endif // CAMERA

