
(cl:in-package :asdf)

(defsystem "ros_service_assignment-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "RectangleAreaService" :depends-on ("_package_RectangleAreaService"))
    (:file "_package_RectangleAreaService" :depends-on ("_package"))
  ))