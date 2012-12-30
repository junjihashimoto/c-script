(defun check-c-script-shebang ()
  (save-excursion
    (goto-char 1)
    (re-search-forward "^#!.*\\(c-script\\)" 50 t)
    )
  )

(add-to-list 'interpreter-mode-alist
             '("c-script" . c++-mode))

(require 'flymake)
(defun flymake-c-script-init ()
  (let* ((temp-file  (flymake-init-create-temp-buffer-copy
		      'flymake-create-temp-inplace))
         (local-file (file-relative-name
                      temp-file
                      (file-name-directory buffer-file-name))))
    (list "c-script" (list "-syntax" local-file))))

(push '("\\.cpp$" flymake-c-script-init) flymake-allowed-file-name-masks)

(setq flymake-gui-warnings-enabled nil)
(global-set-key "\M-p" 'flymake-goto-prev-error)
(global-set-key "\M-n" 'flymake-goto-next-error)


(add-hook 'c++-mode-hook '(lambda ()
			    (if (check-c-script-shebang)
				(flymake-mode t)
			      )
			    )
	  )



