<!-- S -> PARADIGM | PARADIGM STATEMENTS | STATEMENTS PARADIGM | STATEMENTS PARADIGM STATEMENTS
S -> PARADIGM PYSTATEMENTS | PYSTATEMENTS PARADIGM | PYSTATEMENTS PARADIGM PYSTATEMENTS
PARADIGM -> OOP | PP | MIXED -->

S -> PARADIGM S' | STATEMENTS PARADIGM S' | PYSTATEMENTS PARADIGM S'
S' -> STATEMENTS | PYSTATEMENTS | ε

PARADIGM -> OOP | PP | MIXED

OOP -> PYCLASS | CLASS

CLASS -> PREFIX CLASSCOMPLEMENT

<!-- CLASSCOMPLEMENT -> IDS _class_ IDS _{_ STATEMENTS _}_ STATEMENTS CLASS | IDS _class_ IDS _{_ STATEMENTS _}_ | IDS _class_ IDS _{_ STATEMENTS _}_ MAIN -->

CLASSCOMPLEMENT -> IDS _class_ IDS _{_ STATEMENTS _}_ CLASSCOMPLEMENT'
CLASSCOMPLEMENT' -> MAIN | STATEMENTS CLASS | ε

MAIN -> PREFIX IDS _main_ _(_ IDS _)_ _{_ STATEMENTS _}_

<!-- PYCLASS -> _noindent_ _class_ IDS INDENTEDBLOCK | _noindent_ _class_ IDS _(_ IDS _)_ INDENTEDBLOCK | _noindent_ _class_ IDS INDENTEDBLOCK PYSTATEMENTS PYCLASS | _noindent_ _class_ IDS _(_ IDS _)_ INDENTEDBLOCK PYSTATEMENTS PYCLASS -->

PYCLASS -> _noindent_ _class_ IDS PYCLASS' PYCLASS''
PYCLASS' -> INDENTEDBLOCK | _(_ IDS _)_ INDENTEDBLOCK
PYCLASS'' -> PYSTATEMENTS PYCLASS | ε

PP -> PYFUNC | FUNC

<!-- FUNC -> _noindent_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ STATEMENTS FUNC | _noindent_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ | _noindent_ MAIN -->

FUNC -> MAIN | PREFIX FUNC' FUNC''
FUNC' -> MAIN | IDS _(_ IDS _)_ _{_ STATEMENTS _}_
FUNC'' -> STATEMENTS FUNC | ε

<!-- PYFUNC -> _def_ IDS _(_ IDS _)_ INDENTEDBLOCK PYSTATEMENTS PYFUNC | _def_ IDS _(_ IDS _)_ INDENTEDBLOCK | _noindent_ _def_ _main_ _(_ IDS _)_ INDENTEDBLOCK -->

PYFUNC -> _def_ IDS _(_ IDS _)_ INDENTEDBLOCK PYFUNC' | _noindent_ _def_ _main_ _(_ IDS _)_ INDENTEDBLOCK PYFUNC'
PYFUNC' -> PYSTATEMENTS PYFUNC | ε

MIXED -> PYMIXED | MIXEDN

<!-- MIXEDN -> CLASS FUNC | FUNC CLASS -->

MIXEDN -> CLASS FUNC MIXEDCOMPLEMENT | FUNC CLASS MIXEDCOMPLEMENT
MIXEDCOMPLEMENT -> CLASS | FUNC | MIXEDN | ε

<!-- PYMIXED -> PYCLASS PYFUNC | PYFUNC PYCLASS -->

PYMIXED -> PYCLASS PYFUNC PYMIXEDCOMPLEMENT | PYFUNC PYCLASS PYMIXEDCOMPLEMENT
PYMIXEDCOMPLEMENT -> PYCLASS | PYFUNC | PYMIXED | ε

<!-- INDENTEDBLOCK -> _indent_ PYSTATEMENT | _indent_ PYSTATEMENT INDENTEDBLOCK | _indent_ _def_ IDS _(_ IDS _)_
INDENTEDBLOCK -> _indent_ _def_ IDS _(_ IDS _)_ INDENTEDBLOCK -->

INDENTEDBLOCK -> _indent_ INDENTEDBLOCK' INDENTEDBLOCK''
INDENTEDBLOCK' -> PYSTATEMENT | _def_ IDS _(_ IDS _)_
INDENTEDBLOCK'' -> INDENTEDBLOCK | ε

<!-- PYSTATEMENTS -> PYSTATEMENT | PYSTATEMENT PYSTATEMENTS
PYSTATEMENTS -> _noindent_ PYSTATEMENT | _noindent_ PYSTATEMENT PYSTATEMENTS
PYSTATEMENTS -> PYSTATEMENT INDENTEDBLOCK
PYSTATEMENT -> IDS | IDS _(_ IDS _)_ | IDS _{_ IDS _}_
PYSTATEMENT -> IDS _(_ INDENTEDBLOCK _)_ | IDS _(_ INDENTEDBLOCK _indent_ _)_
PYSTATEMENT -> IDS _{_ INDENTEDBLOCK _}_ | IDS _{_ INDENTEDBLOCK _indent_ _}_ -->

PYSTATEMENTS -> PYSTATEMENT PYSTATEMENTS' | _noindent_ PYSTATEMENT PYSTATEMENTS'
PYSTATEMENTS' -> PYSTATEMENTS | INDENTEDBLOCK | ε

PYSTATEMENT -> IDS PYSTATEMENT'
PYSTATEMENT' -> _(_ PYSTATEMENT'' | _{_ PYSTATEMENT''' | ε
PYSTATEMENT'' -> IDS _)_ | INDENTEDBLOCK PYSTATEMENT''''
PYSTATEMENT''' -> IDS _}_ | INDENTEDBLOCK PYSTATEMENT'''''
PYSTATEMENT'''' -> _)_ | _indent_ _)_
PYSTATEMENT''''' -> _}_ | _indent_ _}_

<!-- STATEMENTS -> STATEMENT | STATEMENT STATEMENTS
STATEMENT -> PREFIX STATEMENTCOMPLEMENT
STATEMENTCOMPLEMENT -> IDS | IDS _(_ STATEMENTS _)_ | IDS _{_ STATEMENTS _}_ | IDS _(_ STATEMENTS _)_ _{_ STATEMENTS _}_ -->

STATEMENTS -> STATEMENT STATEMENTS'
STATEMENTS' -> STATEMENTS | ε
STATEMENT -> PREFIX IDS STATEMENT'
STATEMENT' -> _(_ STATEMENTS _)_ STATEMENT'' | _{_ STATEMENTS _}_ | ε
STATEMENT'' -> _{_ STATEMENTS _}_ | ε

PREFIX -> _indent_ | _noindent_
IDS -> _id_ IDS | ε

S -> PARADIGM S' | STATEMENTS PARADIGM S' | PYSTATEMENTS PARADIGM S'
S' -> STATEMENTS | PYSTATEMENTS | ε

PARADIGM -> OOP | PP | MIXED

OOP -> PYCLASS | CLASS

<!-- added producuction for start of file CLASSCOMPLEMENT -->

CLASS -> PREFIX CLASSCOMPLEMENT | CLASSCOMPLEMENT

CLASSCOMPLEMENT -> IDS _class_ IDS _{_ STATEMENTS _}_ CLASSCOMPLEMENT'
CLASSCOMPLEMENT' -> MAIN | STATEMENTS CLASS | ε

<!-- added producuction for start of file -->

MAIN -> PREFIX IDS _main_ _(_ IDS _)_ _{_ STATEMENTS _}_ | IDS _main_ _(_ IDS _)_ _{_ STATEMENTS _}_

<!-- added producuction for start of file -->

PYCLASS -> _noindent_ _class_ IDS PYCLASS' PYCLASS'' | _class_ IDS PYCLASS' PYCLASS''
PYCLASS' -> INDENTEDBLOCK | _(_ IDS _)_ INDENTEDBLOCK
PYCLASS'' -> PYSTATEMENTS PYCLASS | ε

PP -> PYFUNC | FUNC

<!-- deleted first main production AND deleted one prod -->
<!-- added mandatory id -->

FUNC -> MAIN FUNC' | _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ FUNC' | PREFIX _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ FUNC'

<!-- FUNC' -> _id_ IDS _(_ IDS _)_ _{_ STATEMENTS _}_ -->

FUNC' -> STATEMENTS FUNC | ε

PYFUNC -> _def_ IDS _(_ IDS _)_ INDENTEDBLOCK PYFUNC' | _noindent_ _def_ _main_ _(_ IDS _)_ INDENTEDBLOCK PYFUNC'
PYFUNC' -> PYSTATEMENTS PYFUNC | ε

MIXED -> PYMIXED | MIXEDN

MIXEDN -> CLASS FUNC MIXEDCOMPLEMENT | FUNC CLASS MIXEDCOMPLEMENT
MIXEDCOMPLEMENT -> CLASS | FUNC | MIXEDN | ε

PYMIXED -> PYCLASS PYFUNC PYMIXEDCOMPLEMENT | PYFUNC PYCLASS PYMIXEDCOMPLEMENT
PYMIXEDCOMPLEMENT -> PYCLASS | PYFUNC | PYMIXED | ε

INDENTEDBLOCK -> _indent_ INDENTEDBLOCK' INDENTEDBLOCK''
INDENTEDBLOCK' -> PYSTATEMENT | _def_ IDS _(_ IDS _)_
INDENTEDBLOCK'' -> INDENTEDBLOCK | ε

PYSTATEMENTS -> PYSTATEMENT PYSTATEMENTS' | _noindent_ PYSTATEMENT PYSTATEMENTS'
PYSTATEMENTS' -> PYSTATEMENTS | INDENTEDBLOCK | ε

PYSTATEMENT -> _id_ IDS PYSTATEMENT'
PYSTATEMENT' -> _(_ PYSTATEMENT'' | _{_ PYSTATEMENT''' | ε
PYSTATEMENT'' -> IDS _)_ | INDENTEDBLOCK PYSTATEMENT''''
PYSTATEMENT''' -> IDS _}_ | INDENTEDBLOCK PYSTATEMENT'''''
PYSTATEMENT'''' -> _)_ | _indent_ _)_
PYSTATEMENT''''' -> _}_ | _indent_ _}_

<!-- esta puede estar rarona si  -->
<!-- PREFIX > EPSILON > EPSILON -->

STATEMENTS -> STATEMENT STATEMENTS'
STATEMENTS' -> STATEMENTS | ε
STATEMENT -> PREFIX IDS STATEMENT'
STATEMENT' -> _(_ STATEMENTS _)_ STATEMENT'' | _{_ STATEMENTS _}_ | ε
STATEMENT'' -> _{_ STATEMENTS _}_ | ε

PREFIX -> _indent_ | _noindent_
IDS -> _id_ IDS | ε
