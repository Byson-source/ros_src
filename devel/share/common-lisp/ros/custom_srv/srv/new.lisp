; Auto-generated. Do not edit!


(cl:in-package custom_srv-srv)


;//! \htmlinclude new-request.msg.html

(cl:defclass <new-request> (roslisp-msg-protocol:ros-message)
  ((name1
    :reader name1
    :initarg :name1
    :type cl:string
    :initform "")
   (name2
    :reader name2
    :initarg :name2
    :type cl:string
    :initform ""))
)

(cl:defclass new-request (<new-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <new-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'new-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name custom_srv-srv:<new-request> is deprecated: use custom_srv-srv:new-request instead.")))

(cl:ensure-generic-function 'name1-val :lambda-list '(m))
(cl:defmethod name1-val ((m <new-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader custom_srv-srv:name1-val is deprecated.  Use custom_srv-srv:name1 instead.")
  (name1 m))

(cl:ensure-generic-function 'name2-val :lambda-list '(m))
(cl:defmethod name2-val ((m <new-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader custom_srv-srv:name2-val is deprecated.  Use custom_srv-srv:name2 instead.")
  (name2 m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <new-request>) ostream)
  "Serializes a message object of type '<new-request>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'name1))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'name1))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'name2))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'name2))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <new-request>) istream)
  "Deserializes a message object of type '<new-request>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'name1) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'name1) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'name2) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'name2) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<new-request>)))
  "Returns string type for a service object of type '<new-request>"
  "custom_srv/newRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'new-request)))
  "Returns string type for a service object of type 'new-request"
  "custom_srv/newRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<new-request>)))
  "Returns md5sum for a message object of type '<new-request>"
  "bedb2791e8aadec249d6cb1fea325ad5")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'new-request)))
  "Returns md5sum for a message object of type 'new-request"
  "bedb2791e8aadec249d6cb1fea325ad5")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<new-request>)))
  "Returns full string definition for message of type '<new-request>"
  (cl:format cl:nil "string name1~%string name2~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'new-request)))
  "Returns full string definition for message of type 'new-request"
  (cl:format cl:nil "string name1~%string name2~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <new-request>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'name1))
     4 (cl:length (cl:slot-value msg 'name2))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <new-request>))
  "Converts a ROS message object to a list"
  (cl:list 'new-request
    (cl:cons ':name1 (name1 msg))
    (cl:cons ':name2 (name2 msg))
))
;//! \htmlinclude new-response.msg.html

(cl:defclass <new-response> (roslisp-msg-protocol:ros-message)
  ((fullname
    :reader fullname
    :initarg :fullname
    :type cl:string
    :initform ""))
)

(cl:defclass new-response (<new-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <new-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'new-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name custom_srv-srv:<new-response> is deprecated: use custom_srv-srv:new-response instead.")))

(cl:ensure-generic-function 'fullname-val :lambda-list '(m))
(cl:defmethod fullname-val ((m <new-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader custom_srv-srv:fullname-val is deprecated.  Use custom_srv-srv:fullname instead.")
  (fullname m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <new-response>) ostream)
  "Serializes a message object of type '<new-response>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'fullname))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'fullname))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <new-response>) istream)
  "Deserializes a message object of type '<new-response>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'fullname) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'fullname) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<new-response>)))
  "Returns string type for a service object of type '<new-response>"
  "custom_srv/newResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'new-response)))
  "Returns string type for a service object of type 'new-response"
  "custom_srv/newResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<new-response>)))
  "Returns md5sum for a message object of type '<new-response>"
  "bedb2791e8aadec249d6cb1fea325ad5")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'new-response)))
  "Returns md5sum for a message object of type 'new-response"
  "bedb2791e8aadec249d6cb1fea325ad5")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<new-response>)))
  "Returns full string definition for message of type '<new-response>"
  (cl:format cl:nil "string fullname~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'new-response)))
  "Returns full string definition for message of type 'new-response"
  (cl:format cl:nil "string fullname~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <new-response>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'fullname))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <new-response>))
  "Converts a ROS message object to a list"
  (cl:list 'new-response
    (cl:cons ':fullname (fullname msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'new)))
  'new-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'new)))
  'new-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'new)))
  "Returns string type for a service object of type '<new>"
  "custom_srv/new")