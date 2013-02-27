(defun check-c-script-shebang ()
  (save-excursion
    (goto-char 1)
    (re-search-forward "^#!.*\\(c-script\\)" 50 t)
    )
  )

(add-to-list 'interpreter-mode-alist
             '("c-script" . c++-mode))

(require 'flymake)


(defun display-error-message ()
  (message (get-char-property (point) 'help-echo)))
(defadvice flymake-goto-prev-error (after flymake-goto-prev-error-display-message)
  (display-error-message))
(defadvice flymake-goto-next-error (after flymake-goto-next-error-display-message)
  (display-error-message))
(ad-activate 'flymake-goto-prev-error 'flymake-goto-prev-error-display-message)
(ad-activate 'flymake-goto-next-error 'flymake-goto-next-error-display-message)


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



(defun indent-all ()
  (interactive)
  (indent-region (point-min) (point-max) nil)
  )
(fset 'perl-expand
      "\C-r/*\C-n\C-a\C-@\C-s*/\C-a\C-[w\C-n\C-n\C-p\C-y\C-u\C-[xshell-command-on\C-i\C-mperl -M5.010\C-m")
(global-set-key "\C-x\C-g" 'goto-line)
(global-set-key "\C-c\C-c" 'comment-region)
(global-set-key [(meta return)] 'indent-all)
(global-set-key  (kbd "ESC RET") 'indent-all)
(global-set-key  (kbd "C-<return>") 'compile)
(global-set-key "\C-xp" 'perl-expand)
(display-time)
