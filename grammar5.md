first+

S -> PARADIGM S' | STATEMENTS PARADIGM S' | PYSTATEMENTS PARADIGM S'
indent, noindent, id, class, main, def | indent, noindent | id, noindent

S' -> STATEMENTS | PYSTATEMENTS | ε
indent, noindent | id, noindent | ε, $

PARADIGM -> OOP | PP | MIXED
indent, noindent, class, id | def, indent, noindent, id, main | indent, noindent, id, class, def

OOP -> PYCLASS | CLASS
noindent, class | indent, noindent, class, id

CLASS -> PREFIX CLASSCOMPLEMENT | CLASSCOMPLEMENT
indent, noindent | id class

CLASSCOMPLEMENT -> IDS _class_ IDS _{_ STATEMENTS _}_ CLASSCOMPLEMENT'
id, class

CLASSCOMPLEMENT' -> MAIN | STATEMENTS CLASS | ε
indent, noindent, id, main | indent, noindent | indent, noindent, id, $, ε

MAIN -> PREFIX IDS _main_ _(_ IDS _)_ _{_ STATEMENTS _}_ | IDS _main_ _(_ IDS _)_ _{_ STATEMENTS _}_
indent, noindent | id, main

PYCLASS -> _noindent_ _class_ IDS PYCLASS' PYCLASS'' | _class_ IDS PYCLASS' PYCLASS''
noindent | class

PYCLASS' -> INDENTEDBLOCK | _(_ IDS _)_ INDENTEDBLOCK
indent | (

PYCLASS'' -> PYSTATEMENTS PYCLASS | ε
id, noindent | indent, noindent, id, $, ε

PP -> PYFUNC | FUNC
def, noindent | indent, noindent, id

PP' -> MAIN | ε
indent, noindent, id, main | indent, noindent, id, $, ε

PP'' -> FUNC | ε
indent, noindent, id | indent, noindent, id, $, ε

FUNC -> _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ FUNC' | PREFIX _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ FUNC'
id | indent, noindent

FUNC' -> STATEMENTS FUNC | ε
indent, noindent | indent, noindent, id, main, class, $, ε

PYFUNC -> _def_ IDS _(_ IDS _)_ INDENTEDBLOCK PYFUNC' | _noindent_ _def_ _main_ _(_ IDS _)_ INDENTEDBLOCK PYFUNC'
def | noindent

PYFUNC' -> PYSTATEMENTS PYFUNC | ε
id, noindent | indent, noindent, id, class $, ε

MIXED -> PYMIXED | MIXEDN
noindent, def, class | indent, noindent, id, class

MIXEDN -> CLASS FUNC MIXEDCOMPLEMENT | FUNC CLASS MIXEDCOMPLEMENT
indent, noindent, class, id | indent, noindent, id

MIXEDCOMPLEMENT -> CLASS | FUNC | MIXEDN | MAIN | ε
indent, noindent, class, id | indent, noindent, id | indent, noindent, id, class | indent, noindent, id, main | indent, noindent, id, $, ε

PYMIXED -> PYCLASS PYFUNC PYMIXEDCOMPLEMENT | PYFUNC PYCLASS PYMIXEDCOMPLEMENT
noindent, class | def, noindent

PYMIXEDCOMPLEMENT -> PYCLASS | PYFUNC | PYMIXED | ε
noindent, class | def, noindent | noindent, def, class | indent, noindent, id, $, ε

INDENTEDBLOCK -> _indent_ INDENTEDBLOCK' INDENTEDBLOCK''
indent

INDENTEDBLOCK' -> PYSTATEMENT | _def_ IDS _(_ IDS _)_
id | def

INDENTEDBLOCK'' -> INDENTEDBLOCK | ε
indent | indent, noindent, id, $, ε

PYSTATEMENTS -> PYSTATEMENT PYSTATEMENTS' | _noindent_ PYSTATEMENT PYSTATEMENTS'
id | noindent

PYSTATEMENTS' -> PYSTATEMENTS | INDENTEDBLOCK | ε
id, noindent | indent | indent, noindent, id, class, main, def, $, ε

PYSTATEMENT -> _id_ IDS PYSTATEMENT'
id

PYSTATEMENT' -> _(_ PYSTATEMENT'' | _{_ PYSTATEMENT''' | ε
( | { | indent, noindent, id, class, main, def, $, ε

PYSTATEMENT'' -> IDS _)_ | INDENTEDBLOCK PYSTATEMENT''''
id, ) | indent

PYSTATEMENT''' -> IDS _}_ | INDENTEDBLOCK PYSTATEMENT'''''
id, } | indent

PYSTATEMENT'''' -> _)_ | _indent_ _)_
) | indent

PYSTATEMENT''''' -> _}_ | _indent_ _}_
} | indent

STATEMENTS -> STATEMENT STATEMENTS'
indent, noindent

STATEMENTS' -> STATEMENTS | ε
indent, noindent | indent, noindent, class, id, main, def, }, ), $, ε

STATEMENT -> PREFIX IDS STATEMENT'
indent, noindent

STATEMENT' -> _(_ STATEMENTS _)_ STATEMENT'' | _{_ STATEMENTS _}_ | ε
( | { | indent, noindent, class, id, main, def, }, ), $, ε

STATEMENT'' -> _{_ STATEMENTS _}_ | ε
{ | indent, noindent, class, id, main, def, }, ), $, ε

PREFIX -> _indent_ | _noindent_
indent | noindent

IDS -> _id_ IDS | ε
id | class, {, main, ), indent, (, noindent, $, id, def, }, ε
