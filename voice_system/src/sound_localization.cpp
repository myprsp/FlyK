#include <ros/ros.h> 
#include <serial/serial.h>  //ROS已经内置了的串口包 
#include <std_msgs/String.h> 
#include<std_msgs/Int32.h>
#include <std_msgs/Empty.h> 
#include <iostream>

using namespace std; 
std::string& trim(std::string &); 

serial::Serial ser; //声明串口对象 
bool r=false;
std::string rev="";

std::string& trim(std::string &s)   
{  
    if (s.empty())   
    {  
        return s;  
    }  
  
    s.erase(0,s.find_first_not_of(" "));  
    s.erase(s.find_last_not_of(" ") + 1);  
    return s;  
}  

//回调函数 
void write_callback(const std_msgs::String::ConstPtr& msg) 
{ 
    ROS_INFO_STREAM("Writing to serial port" <<msg->data); 
    ser.write(msg->data);   //发送串口数据 
} 
  
int main (int argc, char** argv) 
{ 
    //初始化节点 
    ros::init(argc, argv, "sound_localization"); 
    //声明节点句柄 
    ros::NodeHandle nh; 
  
    //订阅主题，并配置回调函数 
    ros::Subscriber write_sub = nh.subscribe("sound_localization_write", 1000, write_callback); 
    //发布主题 
    ros::Publisher read_pub = nh.advertise<std_msgs::String>("sound_localization_read", 1000);
    ros::Publisher start_asr_pub = nh.advertise<std_msgs::Int32>("/voice/xf_asr_topic", 1);
    ros::Publisher sound_angle_pub = nh.advertise<std_msgs::Int32>("/voice/xf_angle_topic", 1);
    try 
    { 
        //获取串口参数
        std::string port;
        ros::param::param<std::string>("~port", port, "/dev/ttyUSB0");//SoundLocalization
        int baud;
        ros::param::param<int>("~baud", baud, 115200);
        //设置串口属性，并打开串口 
        ser.setPort(port); 
        ser.setBaudrate(baud); 
        serial::Timeout to = serial::Timeout::simpleTimeout(1000); 
        ser.setTimeout(to); 
        ser.open(); 
    } 
    catch (serial::IOException& e) 
    { 
        ROS_ERROR_STREAM("SoundLocalization Unable to open port "); 
        return -1; 
    } 
  
    //检测串口是否已经打开，并给出提示信息 
    if(ser.isOpen()) 
    { 
        ROS_INFO_STREAM("SoundLocalization Port initialized"); 
    } 
    else 
    { 
        return -1; 
    } 
  
    //指定循环的频率 
    ros::Rate loop_rate(50); 
    while(ros::ok()) 
    { 
        if(rev.length()>400)
        {
            rev="";
        }
        // ser.write("abcd");   //发送串口数据 
        if(ser.available()){
            //ROS_INFO_STREAM("Reading from SoundLocalization port\n"); 
            rev =rev+ser.read(ser.available());
            const char *show;
            string r="Enter your CMD:";
            show=strstr(rev.c_str(),r.c_str());//返回指向第一次出现r位置的指针，如果没找到则返回NULL。
            if(show!=NULL)
            {
                std_msgs::String result;
                std_msgs::Int32 start_asr; 
                result.data = rev;
                start_asr.data=1;
                ROS_INFO_STREAM("Read: " << result.data); 
                read_pub.publish(result);
                start_asr_pub.publish(start_asr);
                int a=rev.find("angle:")+6;
                string angle(rev.substr(a,3));
                angle=trim(angle);
                char* end;
                int b= static_cast<int>(strtol(angle.c_str(),&end,10));
                std_msgs::Int32 sound_angle;
                sound_angle.data=b;
                sound_angle_pub.publish(sound_angle);
                rev="";
            }
            
        } 

        //处理ROS的信息，比如订阅消息,并调用回调函数 
        ros::spinOnce(); 
        loop_rate.sleep(); 
  
    } 
} 