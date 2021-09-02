;; Auto-generated. Do not edit!


(when (boundp 'rtabmap_ros::GetNodesInRadius)
  (if (not (find-package "RTABMAP_ROS"))
    (make-package "RTABMAP_ROS"))
  (shadow 'GetNodesInRadius (find-package "RTABMAP_ROS")))
(unless (find-package "RTABMAP_ROS::GETNODESINRADIUS")
  (make-package "RTABMAP_ROS::GETNODESINRADIUS"))
(unless (find-package "RTABMAP_ROS::GETNODESINRADIUSREQUEST")
  (make-package "RTABMAP_ROS::GETNODESINRADIUSREQUEST"))
(unless (find-package "RTABMAP_ROS::GETNODESINRADIUSRESPONSE")
  (make-package "RTABMAP_ROS::GETNODESINRADIUSRESPONSE"))

(in-package "ROS")



(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))


(defclass rtabmap_ros::GetNodesInRadiusRequest
  :super ros::object
  :slots (_node_id _x _y _z _radius ))

(defmethod rtabmap_ros::GetNodesInRadiusRequest
  (:init
   (&key
    ((:node_id __node_id) 0)
    ((:x __x) 0.0)
    ((:y __y) 0.0)
    ((:z __z) 0.0)
    ((:radius __radius) 0.0)
    )
   (send-super :init)
   (setq _node_id (round __node_id))
   (setq _x (float __x))
   (setq _y (float __y))
   (setq _z (float __z))
   (setq _radius (float __radius))
   self)
  (:node_id
   (&optional __node_id)
   (if __node_id (setq _node_id __node_id)) _node_id)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:z
   (&optional __z)
   (if __z (setq _z __z)) _z)
  (:radius
   (&optional __radius)
   (if __radius (setq _radius __radius)) _radius)
  (:serialization-length
   ()
   (+
    ;; int32 _node_id
    4
    ;; float32 _x
    4
    ;; float32 _y
    4
    ;; float32 _z
    4
    ;; float32 _radius
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _node_id
       (write-long _node_id s)
     ;; float32 _x
       (sys::poke _x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _y
       (sys::poke _y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _z
       (sys::poke _z (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _radius
       (sys::poke _radius (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _node_id
     (setq _node_id (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32 _x
     (setq _x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _y
     (setq _y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _z
     (setq _z (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _radius
     (setq _radius (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(defclass rtabmap_ros::GetNodesInRadiusResponse
  :super ros::object
  :slots (_ids _poses ))

(defmethod rtabmap_ros::GetNodesInRadiusResponse
  (:init
   (&key
    ((:ids __ids) (make-array 0 :initial-element 0 :element-type :integer))
    ((:poses __poses) ())
    )
   (send-super :init)
   (setq _ids __ids)
   (setq _poses __poses)
   self)
  (:ids
   (&optional __ids)
   (if __ids (setq _ids __ids)) _ids)
  (:poses
   (&rest __poses)
   (if (keywordp (car __poses))
       (send* _poses __poses)
     (progn
       (if __poses (setq _poses (car __poses)))
       _poses)))
  (:serialization-length
   ()
   (+
    ;; int32[] _ids
    (* 4    (length _ids)) 4
    ;; geometry_msgs/Pose[] _poses
    (apply #'+ (send-all _poses :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32[] _ids
     (write-long (length _ids) s)
     (dotimes (i (length _ids))
       (write-long (elt _ids i) s)
       )
     ;; geometry_msgs/Pose[] _poses
     (write-long (length _poses) s)
     (dolist (elem _poses)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32[] _ids
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _ids (instantiate integer-vector n))
     (dotimes (i n)
     (setf (elt _ids i) (sys::peek buf ptr- :integer)) (incf ptr- 4)
     ))
   ;; geometry_msgs/Pose[] _poses
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _poses (let (r) (dotimes (i n) (push (instance geometry_msgs::Pose :init) r)) r))
     (dolist (elem- _poses)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(defclass rtabmap_ros::GetNodesInRadius
  :super ros::object
  :slots ())

(setf (get rtabmap_ros::GetNodesInRadius :md5sum-) "d9919b4d20cd8ddf742167d34c6d6eca")
(setf (get rtabmap_ros::GetNodesInRadius :datatype-) "rtabmap_ros/GetNodesInRadius")
(setf (get rtabmap_ros::GetNodesInRadius :request) rtabmap_ros::GetNodesInRadiusRequest)
(setf (get rtabmap_ros::GetNodesInRadius :response) rtabmap_ros::GetNodesInRadiusResponse)

(defmethod rtabmap_ros::GetNodesInRadiusRequest
  (:response () (instance rtabmap_ros::GetNodesInRadiusResponse :init)))

(setf (get rtabmap_ros::GetNodesInRadiusRequest :md5sum-) "d9919b4d20cd8ddf742167d34c6d6eca")
(setf (get rtabmap_ros::GetNodesInRadiusRequest :datatype-) "rtabmap_ros/GetNodesInRadiusRequest")
(setf (get rtabmap_ros::GetNodesInRadiusRequest :definition-)
      "#request

# If target pose and node_id are all zeros, poses 
# around the latest node in the graph are returned.
# If node_id is not zero, target pose is ignored.

# Node id
int32 node_id

# Target pose:
float32 x
float32 y
float32 z

# Radius, <=0 means that RGBD/LocalRadius will be used
float32 radius

---
#response
int32[] ids
geometry_msgs/Pose[] poses

================================================================================
MSG: geometry_msgs/Pose
# A representation of pose in free space, composed of position and orientation. 
Point position
Quaternion orientation

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: geometry_msgs/Quaternion
# This represents an orientation in free space in quaternion form.

float64 x
float64 y
float64 z
float64 w
")

(setf (get rtabmap_ros::GetNodesInRadiusResponse :md5sum-) "d9919b4d20cd8ddf742167d34c6d6eca")
(setf (get rtabmap_ros::GetNodesInRadiusResponse :datatype-) "rtabmap_ros/GetNodesInRadiusResponse")
(setf (get rtabmap_ros::GetNodesInRadiusResponse :definition-)
      "#request

# If target pose and node_id are all zeros, poses 
# around the latest node in the graph are returned.
# If node_id is not zero, target pose is ignored.

# Node id
int32 node_id

# Target pose:
float32 x
float32 y
float32 z

# Radius, <=0 means that RGBD/LocalRadius will be used
float32 radius

---
#response
int32[] ids
geometry_msgs/Pose[] poses

================================================================================
MSG: geometry_msgs/Pose
# A representation of pose in free space, composed of position and orientation. 
Point position
Quaternion orientation

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: geometry_msgs/Quaternion
# This represents an orientation in free space in quaternion form.

float64 x
float64 y
float64 z
float64 w
")



(provide :rtabmap_ros/GetNodesInRadius "d9919b4d20cd8ddf742167d34c6d6eca")


