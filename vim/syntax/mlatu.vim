if exists('b:current_syntax')|fini|en|sy clear|sy sync fromstart
sy match comment   /#.*/
sy match function  /[+\-~,<>]/ " primitives
sy match statement /[=;()]/
let b:current_syntax='mlatu'
