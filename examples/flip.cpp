#include <iostream>
#include "ctello.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"

const char* const TELLO_STREAM_URL{"udp://0.0.0.0:11111"};

using ctello::Tello;
using cv::CAP_FFMPEG;
using cv::imshow;
using cv::VideoCapture;
using cv::waitKey;

int main()
{
    Tello tello{};
    if (!tello.Bind())
    {
        return 0;
    }

    tello.SendCommand("streamon");
    while (!(tello.ReceiveResponse()))
        ;

    VideoCapture capture{TELLO_STREAM_URL, CAP_FFMPEG};
    std::array<std::string, 3> commands{"takeoff", "cw 360", "land"};
    int index{0};
    bool busy{false};
    while (true)
    {
        // See surrounding.
        cv::Mat frame;
        capture >> frame;

        // Listen response
        if (const auto response = tello.ReceiveResponse())
        {
            std::cout << "Tello: " << *response << std::endl;
            busy = false;
        }

        // Act
        if (!busy && index < commands.size())
        {
            const std::string command{commands[index++]};
            tello.SendCommand(command);
            std::cout << "Command: " << command << std::endl;
            busy = true;
        }

        // Show what the Tello sees
        imshow("CTello Stream", frame);
        if (waitKey(1) == 27)
        {
            break;
        }
    }
}
