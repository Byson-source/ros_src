; Auto-generated. Do not edit!


(cl:in-package cpp-msg)


;//! \htmlinclude RtabmapReprocessFeedback.msg.html

(cl:defclass <RtabmapReprocessFeedback> (roslisp-msg-protocol:ros-message)
  ((state
    :reader state
    :initarg :state
    :type cl:integer
    :initform 0))
)

(cl:defclass RtabmapReprocessFeedback (<RtabmapReprocessFeedback>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <RtabmapReprocessFeedback>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'RtabmapReprocessFeedback)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name cpp-msg:<RtabmapReprocessFeedback> is deprecated: use cpp-msg:RtabmapReprocessFeedback instead.")))

(cl:ensure-generic-function 'state-val :lambda-list '(m))
(cl:defmethod state-val ((m <RtabmapReprocessFeedback>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader cpp-msg:state-val is deprecated.  Use cpp-msg:state instead.")
  (state m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <RtabmapReprocessFeedback>) ostream)
  "Serializes a message object of type '<RtabmapReprocessFeedback>"
  (cl:let* ((signed (cl:slot-value msg 'state)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <RtabmapReprocessFeedback>) istream)
  "Deserializes a message object of type '<RtabmapReprocessFeedback>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'state) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<RtabmapReprocessFeedback>)))
  "Returns string type for a message object of type '<RtabmapReprocessFeedback>"
  "cpp/RtabmapReprocessFeedback")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'RtabmapReprocessFeedback)))
  "Returns string type for a message object of type 'RtabmapReprocessFeedback"
  "cpp/RtabmapReprocessFeedback")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<RtabmapReprocessFeedback>)))
  "Returns md5sum for a message object of type '<RtabmapReprocessFeedback>"
  "7a2f37ef2ba405f0c7a15cc72663d6f0")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'RtabmapReprocessFeedback)))
  "Returns md5sum for a message object of type 'RtabmapReprocessFeedback"
  "7a2f37ef2ba405f0c7a15cc72663d6f0")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<RtabmapReprocessFeedback>)))
  "Returns full string definition for message of type '<RtabmapReprocessFeedback>"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#feedback~%int32 state~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'RtabmapReprocessFeedback)))
  "Returns full string definition for message of type 'RtabmapReprocessFeedback"
  (cl:format cl:nil "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======~%#feedback~%int32 state~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <RtabmapReprocessFeedback>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <RtabmapReprocessFeedback>))
  "Converts a ROS message object to a list"
  (cl:list 'RtabmapReprocessFeedback
    (cl:cons ':state (state msg))
))
