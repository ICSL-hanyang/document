#include <string>
#include <vector>

#include <gazebo/gazebo_config.h>
#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#define IMAGE_TOPIC "~/iris/distorted_camera/link/camera/image"
//"~/iris/camera/link/PX4Flow/image"
//"~/iris/camera/link/camera/image"
//"~/quadrotor/camera/camera/link/camera/image"


void cb(ConstImageStampedPtr &msg)
{
    // std::cout << msg->image().width() << std::endl;
    // std::cout << msg->image().height() << std::endl;
    // std::cout << msg->image().pixel_format() << std::endl;
    // std::cout << std::endl;

    int width;
    int height;
    char *data;

    width = (int) msg->image().width();
    height = (int) msg->image().height();
    data = new char[msg->image().data().length() + 1];

    memcpy(data, msg->image().data().c_str(), msg->image().data().length());
    cv::Mat image(height, width, CV_8UC3, data);

    cv::imshow("camera", image);
    cv::waitKey(1);
    delete data;  // DO NOT FORGET TO DELETE THIS,
                  // ELSE GAZEBO WILL TAKE ALL YOUR MEMORY
}

int main(int argc, char **argv)
{
    gazebo::client::setup(argc, argv);
    gazebo::transport::NodePtr node(new gazebo::transport::Node());
    gazebo::transport::SubscriberPtr sub;

    // setup
    node->Init();
    sub = node->Subscribe(IMAGE_TOPIC, cb);

    // loop
    while (true) {
        gazebo::common::Time::MSleep(10);
    }

    // clean up
    gazebo::client::shutdown();

    return 0;
}
