if exists('b:current_syntax')|fini|en
sy match Comment   /#.*/
sy match Function  /[+\-~,<>]/ " primitives
sy match Statement /[=.()]/
let b:current_syntax='mlatu'
