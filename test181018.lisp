(defun hello-world ()
  (format t "Hello, Common Lisp World!~%"))

(defun make-cd (title artist rating ripped)
  (list :title title
        :artist artist
        :rating rating
        :ripped ripped))

(defvar *db* nil)

(defun add-record (cd)
  (push cd *db*))

(defun reset-db ()
  (setf *db* nil))

(defun dump-db ()
  (dolist (cd *db*)
    (format t "~{~a~10t: ~a~%~}~%" cd)))

(defun prompt-read (prompt)
  (format *query-io* "~a: " prompt)
  (force-output *query-io*)
  (read-line *query-io*))

(defun prompt-for-cd ()
  (make-cd (prompt-read "Title")
           (prompt-read "Artist")
           (or (parse-integer (prompt-read "Rating") :junk-allowed t)
               0)
           (y-or-n-p "Ripped [y/n]: ")))

(defun add-cds ()
  (loop (add-record (prompt-for-cd))
        (if (not (y-or-n-p "Another? [y/n]: "))
            (return))))

(defun save-db (filename)
  (with-open-file (out filename
                       :direction :output
                       :if-exists :supersede)
    (with-standard-io-syntax
      (print *db* out))))

(defun load-db (filename)
  (with-open-file (in filename
                      :direction :input)
    (with-standard-io-syntax
      (setf *db* (read in)))))

(defun select-by-artist (artist)
  (remove-if-not #'(lambda (cd) (equal (getf cd :artist)
                                       artist))
                 *db*))

(defun select-by-title (title)
  (remove-if-not #'(lambda (cd) (equal (getf cd :title)
                                       title))
                 *db*))

(defun select-by-rating (rating)
  (remove-if-not #'(lambda (cd) (equal (getf cd :rating)
                                       rating))
                 *db*))

(defun select-by-ripped (ripped)
  (remove-if-not #'(lambda (cd) (equal (getf cd :ripped)
                                       ripped))
                 *db*))

(defun select (select-fn)
  (remove-if-not select-fn *db*))

(defun artist-selector (artist)
  #'(lambda (cd) (equal (getf cd :artist) artist)))

(defun title-selector (title)
  #'(lambda (cd) (equal (getf cd :title) title)))

(defun rating-selector (rating)
  #'(lambda (cd) (equal (getf cd :rating) rating)))

(defun ripped-selector (ripped)
  #'(lambda (cd) (equal (getf cd :ripped) ripped)))

(defun where (&key title artist rating (ripped nil ripped-p))
  #'(lambda (cd)
      (and (if title (equal (getf cd :title) title) t)
           (if artist (equal (getf cd :artist) artist) t)
           (if rating (equal (getf cd :rating) rating) t)
           (if ripped-p (equal (getf cd :ripped) ripped) t))))

(defun update (selector-fn &key title artist rating (ripped nil ripped-p))
  (setf *db*
        (mapcar #'(lambda (cd)
                    (when (funcall selector-fn cd)
                      (if title (setf (getf cd :title) title))
                      (if artist (setf (getf cd :artist) artist))
                      (if rating (setf (getf cd :rating) rating))
                      (if ripped-p (setf (getf cd :ripped) ripped)))
                    cd)
                *db*)))


(defun delete-cds (selector-fn)
  (setf *db* (remove-if selector-fn *db*)))

(defmacro backwards (expr)
  (reverse expr))

(defun make-comparsion-expr (field value)
  `(equal (getf cd ,field) ,value))

(defun make-comparsion-list (fields)
  (loop while fields
        collecting (make-comparsion-expr (pop fields) (pop fields))))

(defmacro new-where (&rest clauses)
  `#'(lambda (cd)
       (and ,@(make-comparsion-list clauses))))

;;; test1 function
(defun test1 ()
  (add-record (make-cd "Rose" "Kathy Mattea" 7 t))
  (add-record (make-cd "Fly" "Dixie Chicks" 8 t))
  (add-record (make-cd "Home" "Dixie Chicks" 9 t)))

