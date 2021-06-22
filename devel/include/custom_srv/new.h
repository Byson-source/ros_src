// Generated by gencpp from file custom_srv/new.msg
// DO NOT EDIT!


#ifndef CUSTOM_SRV_MESSAGE_NEW_H
#define CUSTOM_SRV_MESSAGE_NEW_H

#include <ros/service_traits.h>


#include <custom_srv/newRequest.h>
#include <custom_srv/newResponse.h>


namespace custom_srv
{

struct new
{

typedef newRequest Request;
typedef newResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct new
} // namespace custom_srv


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::custom_srv::new > {
  static const char* value()
  {
    return "bedb2791e8aadec249d6cb1fea325ad5";
  }

  static const char* value(const ::custom_srv::new&) { return value(); }
};

template<>
struct DataType< ::custom_srv::new > {
  static const char* value()
  {
    return "custom_srv/new";
  }

  static const char* value(const ::custom_srv::new&) { return value(); }
};


// service_traits::MD5Sum< ::custom_srv::newRequest> should match
// service_traits::MD5Sum< ::custom_srv::new >
template<>
struct MD5Sum< ::custom_srv::newRequest>
{
  static const char* value()
  {
    return MD5Sum< ::custom_srv::new >::value();
  }
  static const char* value(const ::custom_srv::newRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::custom_srv::newRequest> should match
// service_traits::DataType< ::custom_srv::new >
template<>
struct DataType< ::custom_srv::newRequest>
{
  static const char* value()
  {
    return DataType< ::custom_srv::new >::value();
  }
  static const char* value(const ::custom_srv::newRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::custom_srv::newResponse> should match
// service_traits::MD5Sum< ::custom_srv::new >
template<>
struct MD5Sum< ::custom_srv::newResponse>
{
  static const char* value()
  {
    return MD5Sum< ::custom_srv::new >::value();
  }
  static const char* value(const ::custom_srv::newResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::custom_srv::newResponse> should match
// service_traits::DataType< ::custom_srv::new >
template<>
struct DataType< ::custom_srv::newResponse>
{
  static const char* value()
  {
    return DataType< ::custom_srv::new >::value();
  }
  static const char* value(const ::custom_srv::newResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // CUSTOM_SRV_MESSAGE_NEW_H
