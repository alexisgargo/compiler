S -> PARADIGM | PARADIGM STATEMENTS | STATEMENTS PARADIGM | STATEMENTS PARADIGM STATEMENTS
S -> PARADIGM PYSTATEMENTS | PYSTATEMENTS PARADIGM | PYSTATEMENTS PARADIGM PYSTATEMENTS
PARADIGM -> OOP | PP | MIXED

OOP -> PYCLASS | CLASS
CLASS -> PREFIX CLASSCOMPLEMENT | CLASSCOMPLEMENT
CLASSCOMPLEMENT -> IDS _class_ IDS _{_ STATEMENTS _}_ STATEMENTS CLASS | IDS _class_ IDS _{_ STATEMENTS _}_ | IDS _class_ IDS _{_ STATEMENTS _}_ MAIN

MAIN -> PREFIX IDS _main_ _(_ IDS _)_ _{_ STATEMENTS _}_ | IDS _main_ _(_ IDS _)_ _{_ STATEMENTS _}_

PYCLASS -> _noindent_ _class_ IDS INDENTEDBLOCK | _noindent_ _class_ IDS _(_ IDS _)_ INDENTEDBLOCK | _noindent_ _class_ IDS INDENTEDBLOCK PYSTATEMENTS PYCLASS | _noindent_ _class_ IDS _(_ IDS _)_ INDENTEDBLOCK PYSTATEMENTS PYCLASS | _class_ IDS INDENTEDBLOCK | _class_ IDS _(_ IDS _)_ INDENTEDBLOCK | _class_ IDS INDENTEDBLOCK PYSTATEMENTS PYCLASS | _class_ IDS _(_ IDS _)_ INDENTEDBLOCK PYSTATEMENTS PYCLASS

PP -> PYFUNC | FUNC | MAIN | FUNC MAIN | MAIN FUNC

<!-- FUNC -> MAIN | MAIN STATEMENTS FUNC | PREFIX _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ | PREFIX _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ STATEMENTS FUNC | _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ | _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ STATEMENTS FUNC -->

FUNC -> PREFIX _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ | PREFIX _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ STATEMENTS FUNC | _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ | _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ STATEMENTS FUNC

PYFUNC -> _def_ IDS _(_ IDS _)_ INDENTEDBLOCK PYSTATEMENTS PYFUNC | _def_ IDS _(_ IDS _)_ INDENTEDBLOCK | _noindent_ _def_ _main_ _(_ IDS _)_ INDENTEDBLOCK | _noindent_ _def_ IDS _(_ IDS _)_ INDENTEDBLOCK PYSTATEMENTS PYFUNC

MIXED -> PYMIXED | MIXEDN

MIXEDN -> CLASS FUNC MIXEDCOMPLEMENT | FUNC CLASS MIXEDCOMPLEMENT
MIXEDN -> CLASS FUNC | FUNC CLASS
MIXEDCOMPLEMENT -> CLASS | FUNC | MIXEDN

PYMIXED -> PYCLASS PYFUNC | PYFUNC PYCLASS
PYMIXED -> PYCLASS PYFUNC PYMIXEDCOMPLEMENT | PYFUNC PYCLASS PYMIXEDCOMPLEMENT
PYMIXEDCOMPLEMENT -> PYCLASS | PYFUNC | PYMIXED

INDENTEDBLOCK -> _indent_ PYSTATEMENT | _indent_ PYSTATEMENT INDENTEDBLOCK | _indent_ _def_ IDS _(_ IDS _)_
INDENTEDBLOCK -> _indent_ _def_ IDS _(_ INDENTEDBLOCK _)_ INDENTEDBLOCK
PYSTATEMENTS -> PYSTATEMENT | PYSTATEMENT PYSTATEMENTS
PYSTATEMENTS -> _noindent_ PYSTATEMENT | _noindent_ PYSTATEMENT PYSTATEMENTS
PYSTATEMENTS -> PYSTATEMENT INDENTEDBLOCK
PYSTATEMENT -> IDS | IDS _(_ IDS _)_ | IDS _{_ IDS _}_
PYSTATEMENT -> IDS _(_ INDENTEDBLOCK _)_ | IDS _(_ INDENTEDBLOCK _indent_ _)_
PYSTATEMENT -> IDS _{_ INDENTEDBLOCK _}_ | IDS _{_ INDENTEDBLOCK _indent_ _}_

STATEMENTS -> STATEMENT | STATEMENT STATEMENTS
STATEMENT -> PREFIX STATEMENTCOMPLEMENT
STATEMENTCOMPLEMENT -> IDS | IDS _(_ STATEMENTS _)_ | IDS _{_ STATEMENTS _}_ | IDS _(_ STATEMENTS _)_ _{_ STATEMENTS _}_

PREFIX -> _indent_ | _noindent_
IDS -> _id_ IDS | ε
