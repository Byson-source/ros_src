// Generated by gencpp from file cpp/RtabmapReprocessAction.msg
// DO NOT EDIT!


#ifndef CPP_MESSAGE_RTABMAPREPROCESSACTION_H
#define CPP_MESSAGE_RTABMAPREPROCESSACTION_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <cpp/RtabmapReprocessActionGoal.h>
#include <cpp/RtabmapReprocessActionResult.h>
#include <cpp/RtabmapReprocessActionFeedback.h>

namespace cpp
{
template <class ContainerAllocator>
struct RtabmapReprocessAction_
{
  typedef RtabmapReprocessAction_<ContainerAllocator> Type;

  RtabmapReprocessAction_()
    : action_goal()
    , action_result()
    , action_feedback()  {
    }
  RtabmapReprocessAction_(const ContainerAllocator& _alloc)
    : action_goal(_alloc)
    , action_result(_alloc)
    , action_feedback(_alloc)  {
  (void)_alloc;
    }



   typedef  ::cpp::RtabmapReprocessActionGoal_<ContainerAllocator>  _action_goal_type;
  _action_goal_type action_goal;

   typedef  ::cpp::RtabmapReprocessActionResult_<ContainerAllocator>  _action_result_type;
  _action_result_type action_result;

   typedef  ::cpp::RtabmapReprocessActionFeedback_<ContainerAllocator>  _action_feedback_type;
  _action_feedback_type action_feedback;





  typedef boost::shared_ptr< ::cpp::RtabmapReprocessAction_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::cpp::RtabmapReprocessAction_<ContainerAllocator> const> ConstPtr;

}; // struct RtabmapReprocessAction_

typedef ::cpp::RtabmapReprocessAction_<std::allocator<void> > RtabmapReprocessAction;

typedef boost::shared_ptr< ::cpp::RtabmapReprocessAction > RtabmapReprocessActionPtr;
typedef boost::shared_ptr< ::cpp::RtabmapReprocessAction const> RtabmapReprocessActionConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::cpp::RtabmapReprocessAction_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::cpp::RtabmapReprocessAction_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::cpp::RtabmapReprocessAction_<ContainerAllocator1> & lhs, const ::cpp::RtabmapReprocessAction_<ContainerAllocator2> & rhs)
{
  return lhs.action_goal == rhs.action_goal &&
    lhs.action_result == rhs.action_result &&
    lhs.action_feedback == rhs.action_feedback;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::cpp::RtabmapReprocessAction_<ContainerAllocator1> & lhs, const ::cpp::RtabmapReprocessAction_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace cpp

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::cpp::RtabmapReprocessAction_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::cpp::RtabmapReprocessAction_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::cpp::RtabmapReprocessAction_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::cpp::RtabmapReprocessAction_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::cpp::RtabmapReprocessAction_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::cpp::RtabmapReprocessAction_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::cpp::RtabmapReprocessAction_<ContainerAllocator> >
{
  static const char* value()
  {
    return "d5bd03af64ad5d14c3dd4dea6dd3c5b7";
  }

  static const char* value(const ::cpp::RtabmapReprocessAction_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xd5bd03af64ad5d14ULL;
  static const uint64_t static_value2 = 0xc3dd4dea6dd3c5b7ULL;
};

template<class ContainerAllocator>
struct DataType< ::cpp::RtabmapReprocessAction_<ContainerAllocator> >
{
  static const char* value()
  {
    return "cpp/RtabmapReprocessAction";
  }

  static const char* value(const ::cpp::RtabmapReprocessAction_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::cpp::RtabmapReprocessAction_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"\n"
"RtabmapReprocessActionGoal action_goal\n"
"RtabmapReprocessActionResult action_result\n"
"RtabmapReprocessActionFeedback action_feedback\n"
"\n"
"================================================================================\n"
"MSG: cpp/RtabmapReprocessActionGoal\n"
"# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"\n"
"Header header\n"
"actionlib_msgs/GoalID goal_id\n"
"RtabmapReprocessGoal goal\n"
"\n"
"================================================================================\n"
"MSG: std_msgs/Header\n"
"# Standard metadata for higher-level stamped data types.\n"
"# This is generally used to communicate timestamped data \n"
"# in a particular coordinate frame.\n"
"# \n"
"# sequence ID: consecutively increasing ID \n"
"uint32 seq\n"
"#Two-integer timestamp that is expressed as:\n"
"# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n"
"# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n"
"# time-handling sugar is provided by the client library\n"
"time stamp\n"
"#Frame this data is associated with\n"
"string frame_id\n"
"\n"
"================================================================================\n"
"MSG: actionlib_msgs/GoalID\n"
"# The stamp should store the time at which this goal was requested.\n"
"# It is used by an action server when it tries to preempt all\n"
"# goals that were requested before a certain time\n"
"time stamp\n"
"\n"
"# The id provides a way to associate feedback and\n"
"# result message with specific goal requests. The id\n"
"# specified must be unique.\n"
"string id\n"
"\n"
"\n"
"================================================================================\n"
"MSG: cpp/RtabmapReprocessGoal\n"
"# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"#goal definition\n"
"string[] databasepaths\n"
"string goal_path\n"
"\n"
"================================================================================\n"
"MSG: cpp/RtabmapReprocessActionResult\n"
"# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"\n"
"Header header\n"
"actionlib_msgs/GoalStatus status\n"
"RtabmapReprocessResult result\n"
"\n"
"================================================================================\n"
"MSG: actionlib_msgs/GoalStatus\n"
"GoalID goal_id\n"
"uint8 status\n"
"uint8 PENDING         = 0   # The goal has yet to be processed by the action server\n"
"uint8 ACTIVE          = 1   # The goal is currently being processed by the action server\n"
"uint8 PREEMPTED       = 2   # The goal received a cancel request after it started executing\n"
"                            #   and has since completed its execution (Terminal State)\n"
"uint8 SUCCEEDED       = 3   # The goal was achieved successfully by the action server (Terminal State)\n"
"uint8 ABORTED         = 4   # The goal was aborted during execution by the action server due\n"
"                            #    to some failure (Terminal State)\n"
"uint8 REJECTED        = 5   # The goal was rejected by the action server without being processed,\n"
"                            #    because the goal was unattainable or invalid (Terminal State)\n"
"uint8 PREEMPTING      = 6   # The goal received a cancel request after it started executing\n"
"                            #    and has not yet completed execution\n"
"uint8 RECALLING       = 7   # The goal received a cancel request before it started executing,\n"
"                            #    but the action server has not yet confirmed that the goal is canceled\n"
"uint8 RECALLED        = 8   # The goal received a cancel request before it started executing\n"
"                            #    and was successfully cancelled (Terminal State)\n"
"uint8 LOST            = 9   # An action client can determine that a goal is LOST. This should not be\n"
"                            #    sent over the wire by an action server\n"
"\n"
"#Allow for the user to associate a string with GoalStatus for debugging\n"
"string text\n"
"\n"
"\n"
"================================================================================\n"
"MSG: cpp/RtabmapReprocessResult\n"
"# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"#result definition\n"
"int32 result\n"
"\n"
"================================================================================\n"
"MSG: cpp/RtabmapReprocessActionFeedback\n"
"# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"\n"
"Header header\n"
"actionlib_msgs/GoalStatus status\n"
"RtabmapReprocessFeedback feedback\n"
"\n"
"================================================================================\n"
"MSG: cpp/RtabmapReprocessFeedback\n"
"# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"#feedback\n"
"int32 state\n"
;
  }

  static const char* value(const ::cpp::RtabmapReprocessAction_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::cpp::RtabmapReprocessAction_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.action_goal);
      stream.next(m.action_result);
      stream.next(m.action_feedback);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct RtabmapReprocessAction_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::cpp::RtabmapReprocessAction_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::cpp::RtabmapReprocessAction_<ContainerAllocator>& v)
  {
    s << indent << "action_goal: ";
    s << std::endl;
    Printer< ::cpp::RtabmapReprocessActionGoal_<ContainerAllocator> >::stream(s, indent + "  ", v.action_goal);
    s << indent << "action_result: ";
    s << std::endl;
    Printer< ::cpp::RtabmapReprocessActionResult_<ContainerAllocator> >::stream(s, indent + "  ", v.action_result);
    s << indent << "action_feedback: ";
    s << std::endl;
    Printer< ::cpp::RtabmapReprocessActionFeedback_<ContainerAllocator> >::stream(s, indent + "  ", v.action_feedback);
  }
};

} // namespace message_operations
} // namespace ros

#endif // CPP_MESSAGE_RTABMAPREPROCESSACTION_H
