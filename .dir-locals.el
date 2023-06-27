((nil . ((eval . (progn
		   (defun run-mlatu ()
		     (interactive)
		     (term-command "cd ~/repos/mlatu" "make repl\n ./repl")
		     (other-window -1))
		   (local-keybind-mode 1)
		   (evil-define-key 'normal local-keybind-mode-map (kbd "C-;") #'run-mlatu))))))
