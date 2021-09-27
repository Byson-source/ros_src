
(cl:in-package :asdf)

(defsystem "cpp-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "AddThreeInts" :depends-on ("_package_AddThreeInts"))
    (:file "_package_AddThreeInts" :depends-on ("_package"))
    (:file "srvDemo" :depends-on ("_package_srvDemo"))
    (:file "_package_srvDemo" :depends-on ("_package"))
  ))