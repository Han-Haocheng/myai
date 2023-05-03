

#include <functional>
#include <iostream>
#include <fstream>
#include <windows.h>


void CaptureScreen(const char *filename) {
  HWND hwnd  = GetDesktopWindow();
  HDC hdc    = GetDC(hwnd);
  HDC hdcMem = CreateCompatibleDC(hdc);

  RECT rect;
  GetClientRect(hwnd, &rect);

  int width          = rect.right - rect.left;
  int height         = rect.bottom - rect.top;

  HBITMAP hBitmap    = CreateCompatibleBitmap(hdc, width, height);
  HBITMAP hBitmapOld = (HBITMAP) SelectObject(hdcMem, hBitmap);

  BitBlt(hdcMem, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

  BITMAP bmp;
  GetObject(hBitmap, sizeof(BITMAP), &bmp);

  BITMAPINFOHEADER bi;
  bi.biSize          = sizeof(BITMAPINFOHEADER);
  bi.biWidth         = width;
  bi.biHeight        = height;
  bi.biPlanes        = 1;
  bi.biBitCount      = 24;
  bi.biCompression   = BI_RGB;
  bi.biSizeImage     = 0;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrUsed       = 0;
  bi.biClrImportant  = 0;

  BITMAPFILEHEADER bmfHeader;
  bmfHeader.bfType      = 0x4d42;
  bmfHeader.bfSize      = (DWORD) (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmp.bmWidthBytes * bmp.bmHeight);
  bmfHeader.bfReserved1 = 0;
  bmfHeader.bfReserved2 = 0;
  bmfHeader.bfOffBits   = (DWORD) sizeof(BITMAPFILEHEADER) + (DWORD) sizeof(BITMAPINFOHEADER);

  std::ofstream file(filename, std::ios::out | std::ios::binary);
  if (file.is_open()) {
    file.write((const char *) &bmfHeader, sizeof(BITMAPFILEHEADER));
    file.write((const char *) &bi, sizeof(BITMAPINFOHEADER));
    char *buffer = new char[width * height * 3];
    GetDIBits(hdcMem,
              hBitmap,
              0,
              height,
              buffer,
              (BITMAPINFO *) &bi,
              DIB_RGB_COLORS);

    file.write(buffer, width * height * 3);
    delete[] buffer;
    file.close();
  }

  SelectObject(hdcMem, hBitmapOld);
  DeleteObject(hBitmap);
  DeleteDC(hdcMem);
  ReleaseDC(hwnd, hdc);
}

int main() {
  // 获取桌面窗口句柄
  const char *filename = "screenshot.bmp";// 保存截屏结果的文件名
  CaptureScreen(filename);
  return 0;
}