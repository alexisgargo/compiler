first
IDS = id, ε
PREFIX = indent, noindent

STATEMENT'' = {, ε
STATEMENT' = (, {, ε
STATEMENT = indent, noindent
STATEMENTS' = indent, noindent, ε
STATEMENTS = indent, noindent

PYSTATEMENT''''' = }, indent
PYSTATEMENT'''' = ), indent
PYSTATEMENT''' = id, }, indent
PYSTATEMENT'' = id, ), indent
PYSTATEMENT' = (, {, ε
PYSTATEMENT = id

PYSTATEMENTS' = id, noindent, indent, ε
PYSTATEMENTS = id, noindent

INDENTEDBLOCK'' = indent, ε
INDENTEDBLOCK' = id, def
INDENTEDBLOCK = indent

PYMIXEDCOMPLEMENT = noindent, def, class, ε
PYMIXED = noindent, def, class

MIXEDCOMPLEMENT = indent, noindent, id, class, main, ε
MIXEDN = indent, noindent, id, class

MIXED = indent, noindent, id, class, def

PYFUNC' = id, noindent, ε
PYFUNC = def, noindent

FUNC' = indent, noindent, ε
FUNC = indent, noindent, id

PP'' -> indent, noindent, id, ε
PP' -> main, indent, noindent, ε
PP = def, indent, noindent, id, main

PYCLASS'' = id, noindent, ε
PYCLASS' = indent, (
PYCLASS = noindent, class

MAIN = indent, noindent, id, main

CLASSCOMPLEMENT' = indent, noindent, main, ε
CLASSCOMPLEMENT = id, class

CLASS = indent, noindent, class, id

OOP = indent, noindent, class, id

PARADIGM = indent, noindent, id, class, main, def

S' = indent, noindent, id, ε
S = indent, noindent, id, class, main, def
