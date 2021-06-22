
(cl:in-package :asdf)

(defsystem "basicros-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "AddTwoPoints" :depends-on ("_package_AddTwoPoints"))
    (:file "_package_AddTwoPoints" :depends-on ("_package"))
  ))