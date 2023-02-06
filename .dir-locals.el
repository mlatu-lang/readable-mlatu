((nil . ((eval . (progn
		   (evil-define-key nil evil-normal-state-map (kbd "C-;")
		     (lambda () (interactive)
		       (split-window-right)
		       (other-window 1)
		       (term "/bin/bash")
		       (execute-kbd-macro "cd ~/repos/mlatu\n")
		       (execute-kbd-macro "\n bye\n clear\n make repl\n ./repl\n"))))))))
