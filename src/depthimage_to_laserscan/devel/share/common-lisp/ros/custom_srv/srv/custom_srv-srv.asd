
(cl:in-package :asdf)

(defsystem "custom_srv-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "new" :depends-on ("_package_new"))
    (:file "_package_new" :depends-on ("_package"))
  ))