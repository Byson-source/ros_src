;; Auto-generated. Do not edit!


(when (boundp 'T::RectangleAreaService)
  (if (not (find-package "T"))
    (make-package "T"))
  (shadow 'RectangleAreaService (find-package "T")))
(unless (find-package "T::RECTANGLEAREASERVICE")
  (make-package "T::RECTANGLEAREASERVICE"))
(unless (find-package "T::RECTANGLEAREASERVICEREQUEST")
  (make-package "T::RECTANGLEAREASERVICEREQUEST"))
(unless (find-package "T::RECTANGLEAREASERVICERESPONSE")
  (make-package "T::RECTANGLEAREASERVICERESPONSE"))

(in-package "ROS")





(defclass T::RectangleAreaServiceRequest
  :super ros::object
  :slots (_a _b ))

(defmethod T::RectangleAreaServiceRequest
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

(defclass T::RectangleAreaServiceResponse
  :super ros::object
  :slots (_area ))

(defmethod T::RectangleAreaServiceResponse
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

(defclass T::RectangleAreaService
  :super ros::object
  :slots ())

(setf (get T::RectangleAreaService :md5sum-) "9d875babe5658f30073750c15b75fb29")
(setf (get T::RectangleAreaService :datatype-) "T/RectangleAreaService")
(setf (get T::RectangleAreaService :request) T::RectangleAreaServiceRequest)
(setf (get T::RectangleAreaService :response) T::RectangleAreaServiceResponse)

(defmethod T::RectangleAreaServiceRequest
  (:response () (instance T::RectangleAreaServiceResponse :init)))

(setf (get T::RectangleAreaServiceRequest :md5sum-) "9d875babe5658f30073750c15b75fb29")
(setf (get T::RectangleAreaServiceRequest :datatype-) "T/RectangleAreaServiceRequest")
(setf (get T::RectangleAreaServiceRequest :definition-)
      "float64 a
float64 b
---
float64 area
")

(setf (get T::RectangleAreaServiceResponse :md5sum-) "9d875babe5658f30073750c15b75fb29")
(setf (get T::RectangleAreaServiceResponse :datatype-) "T/RectangleAreaServiceResponse")
(setf (get T::RectangleAreaServiceResponse :definition-)
      "float64 a
float64 b
---
float64 area
")



(provide :T/RectangleAreaService "9d875babe5658f30073750c15b75fb29")


