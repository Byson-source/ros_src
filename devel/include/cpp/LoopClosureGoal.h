// Generated by gencpp from file cpp/LoopClosureGoal.msg
// DO NOT EDIT!


#ifndef CPP_MESSAGE_LOOPCLOSUREGOAL_H
#define CPP_MESSAGE_LOOPCLOSUREGOAL_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace cpp
{
template <class ContainerAllocator>
struct LoopClosureGoal_
{
  typedef LoopClosureGoal_<ContainerAllocator> Type;

  LoopClosureGoal_()
    : imagepath()  {
    }
  LoopClosureGoal_(const ContainerAllocator& _alloc)
    : imagepath(_alloc)  {
  (void)_alloc;
    }



   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _imagepath_type;
  _imagepath_type imagepath;





  typedef boost::shared_ptr< ::cpp::LoopClosureGoal_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::cpp::LoopClosureGoal_<ContainerAllocator> const> ConstPtr;

}; // struct LoopClosureGoal_

typedef ::cpp::LoopClosureGoal_<std::allocator<void> > LoopClosureGoal;

typedef boost::shared_ptr< ::cpp::LoopClosureGoal > LoopClosureGoalPtr;
typedef boost::shared_ptr< ::cpp::LoopClosureGoal const> LoopClosureGoalConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::cpp::LoopClosureGoal_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::cpp::LoopClosureGoal_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::cpp::LoopClosureGoal_<ContainerAllocator1> & lhs, const ::cpp::LoopClosureGoal_<ContainerAllocator2> & rhs)
{
  return lhs.imagepath == rhs.imagepath;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::cpp::LoopClosureGoal_<ContainerAllocator1> & lhs, const ::cpp::LoopClosureGoal_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace cpp

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::cpp::LoopClosureGoal_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::cpp::LoopClosureGoal_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::cpp::LoopClosureGoal_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::cpp::LoopClosureGoal_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::cpp::LoopClosureGoal_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::cpp::LoopClosureGoal_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::cpp::LoopClosureGoal_<ContainerAllocator> >
{
  static const char* value()
  {
    return "bc491c80e6e5522eb310dc6a0d6adc73";
  }

  static const char* value(const ::cpp::LoopClosureGoal_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xbc491c80e6e5522eULL;
  static const uint64_t static_value2 = 0xb310dc6a0d6adc73ULL;
};

template<class ContainerAllocator>
struct DataType< ::cpp::LoopClosureGoal_<ContainerAllocator> >
{
  static const char* value()
  {
    return "cpp/LoopClosureGoal";
  }

  static const char* value(const ::cpp::LoopClosureGoal_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::cpp::LoopClosureGoal_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"#goal definition\n"
"string imagepath\n"
;
  }

  static const char* value(const ::cpp::LoopClosureGoal_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::cpp::LoopClosureGoal_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.imagepath);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct LoopClosureGoal_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::cpp::LoopClosureGoal_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::cpp::LoopClosureGoal_<ContainerAllocator>& v)
  {
    s << indent << "imagepath: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.imagepath);
  }
};

} // namespace message_operations
} // namespace ros

#endif // CPP_MESSAGE_LOOPCLOSUREGOAL_H
