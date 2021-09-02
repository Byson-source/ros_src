;; Auto-generated. Do not edit!


(when (boundp 'rtabmap_ros::LoadDatabase)
  (if (not (find-package "RTABMAP_ROS"))
    (make-package "RTABMAP_ROS"))
  (shadow 'LoadDatabase (find-package "RTABMAP_ROS")))
(unless (find-package "RTABMAP_ROS::LOADDATABASE")
  (make-package "RTABMAP_ROS::LOADDATABASE"))
(unless (find-package "RTABMAP_ROS::LOADDATABASEREQUEST")
  (make-package "RTABMAP_ROS::LOADDATABASEREQUEST"))
(unless (find-package "RTABMAP_ROS::LOADDATABASERESPONSE")
  (make-package "RTABMAP_ROS::LOADDATABASERESPONSE"))

(in-package "ROS")





(defclass rtabmap_ros::LoadDatabaseRequest
  :super ros::object
  :slots (_database_path _clear ))

(defmethod rtabmap_ros::LoadDatabaseRequest
  (:init
   (&key
    ((:database_path __database_path) "")
    ((:clear __clear) nil)
    )
   (send-super :init)
   (setq _database_path (string __database_path))
   (setq _clear __clear)
   self)
  (:database_path
   (&optional __database_path)
   (if __database_path (setq _database_path __database_path)) _database_path)
  (:clear
   (&optional (__clear :null))
   (if (not (eq __clear :null)) (setq _clear __clear)) _clear)
  (:serialization-length
   ()
   (+
    ;; string _database_path
    4 (length _database_path)
    ;; bool _clear
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _database_path
       (write-long (length _database_path) s) (princ _database_path s)
     ;; bool _clear
       (if _clear (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _database_path
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _database_path (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; bool _clear
     (setq _clear (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(defclass rtabmap_ros::LoadDatabaseResponse
  :super ros::object
  :slots ())

(defmethod rtabmap_ros::LoadDatabaseResponse
  (:init
   (&key
    )
   (send-super :init)
   self)
  (:serialization-length
   ()
   (+
    0
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;;
   self)
  )

(defclass rtabmap_ros::LoadDatabase
  :super ros::object
  :slots ())

(setf (get rtabmap_ros::LoadDatabase :md5sum-) "686abf12c0ee22118c7e5fcc29bcdabe")
(setf (get rtabmap_ros::LoadDatabase :datatype-) "rtabmap_ros/LoadDatabase")
(setf (get rtabmap_ros::LoadDatabase :request) rtabmap_ros::LoadDatabaseRequest)
(setf (get rtabmap_ros::LoadDatabase :response) rtabmap_ros::LoadDatabaseResponse)

(defmethod rtabmap_ros::LoadDatabaseRequest
  (:response () (instance rtabmap_ros::LoadDatabaseResponse :init)))

(setf (get rtabmap_ros::LoadDatabaseRequest :md5sum-) "686abf12c0ee22118c7e5fcc29bcdabe")
(setf (get rtabmap_ros::LoadDatabaseRequest :datatype-) "rtabmap_ros/LoadDatabaseRequest")
(setf (get rtabmap_ros::LoadDatabaseRequest :definition-)
      "#request

# Local database path on which rtabmap is running.
# If the path doesn't exist, a new database will be created.
string database_path

# If the database already exists, data will be cleared if true.
bool clear

---
#response, return false on rtabmap initialization failure.
")

(setf (get rtabmap_ros::LoadDatabaseResponse :md5sum-) "686abf12c0ee22118c7e5fcc29bcdabe")
(setf (get rtabmap_ros::LoadDatabaseResponse :datatype-) "rtabmap_ros/LoadDatabaseResponse")
(setf (get rtabmap_ros::LoadDatabaseResponse :definition-)
      "#request

# Local database path on which rtabmap is running.
# If the path doesn't exist, a new database will be created.
string database_path

# If the database already exists, data will be cleared if true.
bool clear

---
#response, return false on rtabmap initialization failure.
")



(provide :rtabmap_ros/LoadDatabase "686abf12c0ee22118c7e5fcc29bcdabe")


