;; Auto-generated. Do not edit!


(when (boundp 'ros_service_assignment::RectangleAreaService.srv)
  (if (not (find-package "ROS_SERVICE_ASSIGNMENT"))
    (make-package "ROS_SERVICE_ASSIGNMENT"))
  (shadow 'RectangleAreaService.srv (find-package "ROS_SERVICE_ASSIGNMENT")))
(unless (find-package "ROS_SERVICE_ASSIGNMENT::RECTANGLEAREASERVICE.SRV")
  (make-package "ROS_SERVICE_ASSIGNMENT::RECTANGLEAREASERVICE.SRV"))
(unless (find-package "ROS_SERVICE_ASSIGNMENT::RECTANGLEAREASERVICE.SRVREQUEST")
  (make-package "ROS_SERVICE_ASSIGNMENT::RECTANGLEAREASERVICE.SRVREQUEST"))
(unless (find-package "ROS_SERVICE_ASSIGNMENT::RECTANGLEAREASERVICE.SRVRESPONSE")
  (make-package "ROS_SERVICE_ASSIGNMENT::RECTANGLEAREASERVICE.SRVRESPONSE"))

(in-package "ROS")





(defclass ros_service_assignment::RectangleAreaService.srvRequest
  :super ros::object
  :slots (_a _b ))

(defmethod ros_service_assignment::RectangleAreaService.srvRequest
  (:init
   (&key
    ((:a __a) 0.0)
    ((:b __b) 0.0)
    )
   (send-super :init)
   (setq _a (float __a))
   (setq _b (float __b))
   self)
  (:a
   (&optional __a)
   (if __a (setq _a __a)) _a)
  (:b
   (&optional __b)
   (if __b (setq _b __b)) _b)
  (:serialization-length
   ()
   (+
    ;; float64 _a
    8
    ;; float64 _b
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _a
       (sys::poke _a (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _b
       (sys::poke _b (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _a
     (setq _a (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _b
     (setq _b (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(defclass ros_service_assignment::RectangleAreaService.srvResponse
  :super ros::object
  :slots (_area ))

(defmethod ros_service_assignment::RectangleAreaService.srvResponse
  (:init
   (&key
    ((:area __area) 0.0)
    )
   (send-super :init)
   (setq _area (float __area))
   self)
  (:area
   (&optional __area)
   (if __area (setq _area __area)) _area)
  (:serialization-length
   ()
   (+
    ;; float64 _area
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _area
       (sys::poke _area (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _area
     (setq _area (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(defclass ros_service_assignment::RectangleAreaService.srv
  :super ros::object
  :slots ())

(setf (get ros_service_assignment::RectangleAreaService.srv :md5sum-) "9d875babe5658f30073750c15b75fb29")
(setf (get ros_service_assignment::RectangleAreaService.srv :datatype-) "ros_service_assignment/RectangleAreaService.srv")
(setf (get ros_service_assignment::RectangleAreaService.srv :request) ros_service_assignment::RectangleAreaService.srvRequest)
(setf (get ros_service_assignment::RectangleAreaService.srv :response) ros_service_assignment::RectangleAreaService.srvResponse)

(defmethod ros_service_assignment::RectangleAreaService.srvRequest
  (:response () (instance ros_service_assignment::RectangleAreaService.srvResponse :init)))

(setf (get ros_service_assignment::RectangleAreaService.srvRequest :md5sum-) "9d875babe5658f30073750c15b75fb29")
(setf (get ros_service_assignment::RectangleAreaService.srvRequest :datatype-) "ros_service_assignment/RectangleAreaService.srvRequest")
(setf (get ros_service_assignment::RectangleAreaService.srvRequest :definition-)
      "float64 a
float64 b
---
float64 area
")

(setf (get ros_service_assignment::RectangleAreaService.srvResponse :md5sum-) "9d875babe5658f30073750c15b75fb29")
(setf (get ros_service_assignment::RectangleAreaService.srvResponse :datatype-) "ros_service_assignment/RectangleAreaService.srvResponse")
(setf (get ros_service_assignment::RectangleAreaService.srvResponse :definition-)
      "float64 a
float64 b
---
float64 area
")



(provide :ros_service_assignment/RectangleAreaService.srv "9d875babe5658f30073750c15b75fb29")


