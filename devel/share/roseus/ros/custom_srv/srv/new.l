;; Auto-generated. Do not edit!


(when (boundp 'custom_srv::new)
  (if (not (find-package "CUSTOM_SRV"))
    (make-package "CUSTOM_SRV"))
  (shadow 'new (find-package "CUSTOM_SRV")))
(unless (find-package "CUSTOM_SRV::NEW")
  (make-package "CUSTOM_SRV::NEW"))
(unless (find-package "CUSTOM_SRV::NEWREQUEST")
  (make-package "CUSTOM_SRV::NEWREQUEST"))
(unless (find-package "CUSTOM_SRV::NEWRESPONSE")
  (make-package "CUSTOM_SRV::NEWRESPONSE"))

(in-package "ROS")





(defclass custom_srv::newRequest
  :super ros::object
  :slots (_name1 _name2 ))

(defmethod custom_srv::newRequest
  (:init
   (&key
    ((:name1 __name1) "")
    ((:name2 __name2) "")
    )
   (send-super :init)
   (setq _name1 (string __name1))
   (setq _name2 (string __name2))
   self)
  (:name1
   (&optional __name1)
   (if __name1 (setq _name1 __name1)) _name1)
  (:name2
   (&optional __name2)
   (if __name2 (setq _name2 __name2)) _name2)
  (:serialization-length
   ()
   (+
    ;; string _name1
    4 (length _name1)
    ;; string _name2
    4 (length _name2)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _name1
       (write-long (length _name1) s) (princ _name1 s)
     ;; string _name2
       (write-long (length _name2) s) (princ _name2 s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _name1
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _name1 (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _name2
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _name2 (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass custom_srv::newResponse
  :super ros::object
  :slots (_fullname ))

(defmethod custom_srv::newResponse
  (:init
   (&key
    ((:fullname __fullname) "")
    )
   (send-super :init)
   (setq _fullname (string __fullname))
   self)
  (:fullname
   (&optional __fullname)
   (if __fullname (setq _fullname __fullname)) _fullname)
  (:serialization-length
   ()
   (+
    ;; string _fullname
    4 (length _fullname)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _fullname
       (write-long (length _fullname) s) (princ _fullname s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _fullname
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _fullname (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass custom_srv::new
  :super ros::object
  :slots ())

(setf (get custom_srv::new :md5sum-) "bedb2791e8aadec249d6cb1fea325ad5")
(setf (get custom_srv::new :datatype-) "custom_srv/new")
(setf (get custom_srv::new :request) custom_srv::newRequest)
(setf (get custom_srv::new :response) custom_srv::newResponse)

(defmethod custom_srv::newRequest
  (:response () (instance custom_srv::newResponse :init)))

(setf (get custom_srv::newRequest :md5sum-) "bedb2791e8aadec249d6cb1fea325ad5")
(setf (get custom_srv::newRequest :datatype-) "custom_srv/newRequest")
(setf (get custom_srv::newRequest :definition-)
      "string name1
string name2
---
string fullname

")

(setf (get custom_srv::newResponse :md5sum-) "bedb2791e8aadec249d6cb1fea325ad5")
(setf (get custom_srv::newResponse :datatype-) "custom_srv/newResponse")
(setf (get custom_srv::newResponse :definition-)
      "string name1
string name2
---
string fullname

")



(provide :custom_srv/new "bedb2791e8aadec249d6cb1fea325ad5")


