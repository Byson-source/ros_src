; Auto-generated. Do not edit!


(cl:in-package cpp-msg)


;//! \htmlinclude RtabmapReprocessGoal.msg.html

(cl:defclass <RtabmapReprocessGoal> (roslisp-msg-protocol:ros-message)
  ((databasepaths
    :reader databasepaths
    :initarg :databasepaths
    :type (cl:vector cl:string)
   :initform (cl:make-array 0 :element-type 'cl:string :initial-element ""))
   (goal_path
    :reader goal_path
    :initarg :goal_path
    :type cl:string
    :initform ""))
)

(cl:defclass RtabmapReprocessGoal (<RtabmapReprocessGoal>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <RtabmapReprocessGoal>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'RtabmapReprocessGoal)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name cpp-msg:<RtabmapReprocessGoal> is deprecated: use cpp-msg:RtabmapReprocessGoal instead.")))

(cl:ensure-generic-function 'databasepaths-val :lambda-list '(m))
(cl:defmethod databasepaths-val ((m <RtabmapReprocessGoal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader cpp-msg:databasepaths-val is deprecated.  Use cpp-msg:databasepaths instead.")
  (databasepaths m))

(cl:ensure-generic-function 'goal_path-val :lambda-list '(m))
(cl:defmethod goal_path-val ((m <RtabmapReprocessGoal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader cpp-msg:goal_path-val is deprecated.  Use cpp-msg:goal_path instead.")
  (goal_path m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <RtabmapReprocessGoal>) ostream)
  "Serializes a message object of type '<RtabmapReprocessGoal>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'databasepaths))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((__ros_str_len (cl:length ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) ele))
   (cl:slot-value msg 'databasepaths))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'goal_path))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'goal_path))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <RtabmapReprocessGoal>) istream)
  "Deserializes a message object of type '<RtabmapReprocessGoal>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'databasepaths) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'databasepaths)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:aref vals i) __ros_str_idx) (cl:code-char (cl:read-byte istream))))))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'goal_path) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'goal_path) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<RtabmapReprocessGoal>)))
  "Returns string type for a message object of type '<RtabmapReprocessGoal>"
  "cpp/RtabmapReprocessGoal")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'RtabmapReprocessGoal)))
  "Returns string type for a message object of type 'RtabmapReprocessGoal"
  "cpp/RtabmapReprocessGoal")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<RtabmapReprocessGoal>)))
  "Returns md5sum for a message object of type '<RtabmapReprocessGoal>"
  "e6b31abfb6499b91347d6e40d9d2808c")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'RtabmapReprocessGoal)))
  "Returns md5sum for a message object of type 'RtabmapReprocessGoal"
  "e6b31abfb6499b91347d6e40d9d2808c")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<RtabmapReprocessGoal>)))
  "Returns full string definition for message of type '<RtabmapReprocessGoal>"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#goal definition~%string[] databasepaths~%string goal_path~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'RtabmapReprocessGoal)))
  "Returns full string definition for message of type 'RtabmapReprocessGoal"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#goal definition~%string[] databasepaths~%string goal_path~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <RtabmapReprocessGoal>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'databasepaths) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4 (cl:length ele))))
     4 (cl:length (cl:slot-value msg 'goal_path))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <RtabmapReprocessGoal>))
  "Converts a ROS message object to a list"
  (cl:list 'RtabmapReprocessGoal
    (cl:cons ':databasepaths (databasepaths msg))
    (cl:cons ':goal_path (goal_path msg))
))
