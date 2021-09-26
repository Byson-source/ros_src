; Auto-generated. Do not edit!


(cl:in-package cpp-msg)


;//! \htmlinclude FibonacciGoal.msg.html

(cl:defclass <FibonacciGoal> (roslisp-msg-protocol:ros-message)
  ((order
    :reader order
    :initarg :order
    :type cl:integer
    :initform 0)
   (strings
    :reader strings
    :initarg :strings
    :type (cl:vector cl:string)
   :initform (cl:make-array 0 :element-type 'cl:string :initial-element "")))
)

(cl:defclass FibonacciGoal (<FibonacciGoal>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <FibonacciGoal>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'FibonacciGoal)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name cpp-msg:<FibonacciGoal> is deprecated: use cpp-msg:FibonacciGoal instead.")))

(cl:ensure-generic-function 'order-val :lambda-list '(m))
(cl:defmethod order-val ((m <FibonacciGoal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader cpp-msg:order-val is deprecated.  Use cpp-msg:order instead.")
  (order m))

(cl:ensure-generic-function 'strings-val :lambda-list '(m))
(cl:defmethod strings-val ((m <FibonacciGoal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader cpp-msg:strings-val is deprecated.  Use cpp-msg:strings instead.")
  (strings m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <FibonacciGoal>) ostream)
  "Serializes a message object of type '<FibonacciGoal>"
  (cl:let* ((signed (cl:slot-value msg 'order)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'strings))))
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
   (cl:slot-value msg 'strings))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <FibonacciGoal>) istream)
  "Deserializes a message object of type '<FibonacciGoal>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'order) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'strings) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'strings)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:aref vals i) __ros_str_idx) (cl:code-char (cl:read-byte istream))))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<FibonacciGoal>)))
  "Returns string type for a message object of type '<FibonacciGoal>"
  "cpp/FibonacciGoal")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'FibonacciGoal)))
  "Returns string type for a message object of type 'FibonacciGoal"
  "cpp/FibonacciGoal")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<FibonacciGoal>)))
  "Returns md5sum for a message object of type '<FibonacciGoal>"
  "4b6aa96928806e04ff9792cb6e8bfd28")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'FibonacciGoal)))
  "Returns md5sum for a message object of type 'FibonacciGoal"
  "4b6aa96928806e04ff9792cb6e8bfd28")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<FibonacciGoal>)))
  "Returns full string definition for message of type '<FibonacciGoal>"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#goal definition~%int32 order~%string[] strings~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'FibonacciGoal)))
  "Returns full string definition for message of type 'FibonacciGoal"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#goal definition~%int32 order~%string[] strings~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <FibonacciGoal>))
  (cl:+ 0
     4
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'strings) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4 (cl:length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <FibonacciGoal>))
  "Converts a ROS message object to a list"
  (cl:list 'FibonacciGoal
    (cl:cons ':order (order msg))
    (cl:cons ':strings (strings msg))
))
