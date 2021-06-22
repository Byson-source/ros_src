// Generated by gencpp from file T/RectangleAreaServiceRequest.msg
// DO NOT EDIT!


#ifndef T_MESSAGE_RECTANGLEAREASERVICEREQUEST_H
#define T_MESSAGE_RECTANGLEAREASERVICEREQUEST_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace T
{
template <class ContainerAllocator>
struct RectangleAreaServiceRequest_
{
  typedef RectangleAreaServiceRequest_<ContainerAllocator> Type;

  RectangleAreaServiceRequest_()
    : a(0.0)
    , b(0.0)  {
    }
  RectangleAreaServiceRequest_(const ContainerAllocator& _alloc)
    : a(0.0)
    , b(0.0)  {
  (void)_alloc;
    }



   typedef double _a_type;
  _a_type a;

   typedef double _b_type;
  _b_type b;





  typedef boost::shared_ptr< ::T::RectangleAreaServiceRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::T::RectangleAreaServiceRequest_<ContainerAllocator> const> ConstPtr;

}; // struct RectangleAreaServiceRequest_

typedef ::T::RectangleAreaServiceRequest_<std::allocator<void> > RectangleAreaServiceRequest;

typedef boost::shared_ptr< ::T::RectangleAreaServiceRequest > RectangleAreaServiceRequestPtr;
typedef boost::shared_ptr< ::T::RectangleAreaServiceRequest const> RectangleAreaServiceRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::T::RectangleAreaServiceRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::T::RectangleAreaServiceRequest_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::T::RectangleAreaServiceRequest_<ContainerAllocator1> & lhs, const ::T::RectangleAreaServiceRequest_<ContainerAllocator2> & rhs)
{
  return lhs.a == rhs.a &&
    lhs.b == rhs.b;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::T::RectangleAreaServiceRequest_<ContainerAllocator1> & lhs, const ::T::RectangleAreaServiceRequest_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace T

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::T::RectangleAreaServiceRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::T::RectangleAreaServiceRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::T::RectangleAreaServiceRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::T::RectangleAreaServiceRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::T::RectangleAreaServiceRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::T::RectangleAreaServiceRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::T::RectangleAreaServiceRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "6f4f9f1b571de73ae8592a1438fd23f3";
  }

  static const char* value(const ::T::RectangleAreaServiceRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x6f4f9f1b571de73aULL;
  static const uint64_t static_value2 = 0xe8592a1438fd23f3ULL;
};

template<class ContainerAllocator>
struct DataType< ::T::RectangleAreaServiceRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "T/RectangleAreaServiceRequest";
  }

  static const char* value(const ::T::RectangleAreaServiceRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::T::RectangleAreaServiceRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float64 a\n"
"float64 b\n"
;
  }

  static const char* value(const ::T::RectangleAreaServiceRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::T::RectangleAreaServiceRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.a);
      stream.next(m.b);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct RectangleAreaServiceRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::T::RectangleAreaServiceRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::T::RectangleAreaServiceRequest_<ContainerAllocator>& v)
  {
    s << indent << "a: ";
    Printer<double>::stream(s, indent + "  ", v.a);
    s << indent << "b: ";
    Printer<double>::stream(s, indent + "  ", v.b);
  }
};

} // namespace message_operations
} // namespace ros

#endif // T_MESSAGE_RECTANGLEAREASERVICEREQUEST_H
