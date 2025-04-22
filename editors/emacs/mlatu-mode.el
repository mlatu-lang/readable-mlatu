;; -*- lexical-binding: t; -*-
(defvar mlatu-keywords
      '(("~\\|,\\|<\\|>\\|\\+\\|-\\|" . 'font-lock-constant-face)
	("=\\|\\."                    . 'font-lock-keyword-face)
	("#wield .+\n"                . '(0 font-lock-type-face t))))

(defvar mlatu-syntax-table
      (let ((syntable (make-syntax-table)))
	(modify-syntax-entry ?# "<" syntable)
	(modify-syntax-entry ?\n ">" syntable)
	syntable))

(define-derived-mode mlatu-mode fundamental-mode "mlatu"
  "major mode for highlighting mlatu files"
  (setq font-lock-defaults '(mlatu-keywords))
  (set-syntax-table mlatu-syntax-table))

(add-to-list 'auto-mode-alist '("\\.mlt\\'" . mlatu-mode))
(provide 'mlatu-mode)
