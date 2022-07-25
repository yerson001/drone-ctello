#include<iostream>
#include"ctello.h"
#include"opencv2/core.hpp"
#include"opencv2/highgui.hpp"
#include"opencv2/imgcodecs.hpp"

const char* const TELLO_STREAM_URL{"udp://0.0.0.0:11111"};
using namespace std;

using ctello::Tello;
using cv::CAP_FFMPEG;
using cv::imshow;
using cv::VideoCapture;
using cv::waitKey;

int main()
{
  char c;

  Tello tello{};
  if(!tello.Bind())
  {
    return 0;
  }

  tello.SendCommand("streamon");
  while(!(tello.ReceiveResponse()));
  VideoCapture cap{TELLO_STREAM_URL, CAP_FFMPEG};
  bool busy{false};
  while(true)
  {
    cv::Mat frame;
    cap >> frame;
    if(const auto response = tello.ReceiveResponse())
    {
      std::cout <<"tello: "<< *response << std::endl;
      busy = false;
    }
    //comandos de control
    system("stty raw");
    c = getchar();
    system("stty cooked");
    system("clear");
    std::cout << c <<" pressed"<< std::endl;

    if(c=='t')
    {
      tello.SendCommand("takeoff");
      busy = true;
    }
    if(c=='l')
    {
      tello.SendCommand("land");
      busy = true;
    }
    if(c=='a')
    {
      tello.SendCommand("left 20");
      busy = true;
    }
    if(c=='d')
    {
      tello.SendCommand("right 20");
      busy = true;
    }
    if(c=='w')
    {
      tello.SendCommand("forward 20");
      busy = true;
    }
    if(c=='s')
    {
      tello.SendCommand("back 20");
      busy = true;
    }
   if(c=='A')
    {
      tello.SendCommand("up 20");
      busy = true;
    }
   if(c=='B')
    {
      tello.SendCommand("down 20");
      busy = true;
    }

   if(c=='C')
    {
      tello.SendCommand("ccw 20");
      busy = true;
    }
   if(c=='D')
    {
      tello.SendCommand("cw 20");
      busy = true;
    }
   
    imshow("tello", frame);
    if(waitKey(1) == 27)
    {
      break;
    }
    if(c=='.'){
      system("stty cooked");
      exit(0);
    }
  }

  return 0;
}
