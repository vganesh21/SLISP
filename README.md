# Sldraw Graphical Lisp Interpeter
ece-3574-project-3-vganesh21 created by GitHub Classroom

## Introduction
A C++ program is a collection of statements, each of which contains expressions, sequence of characters that when evaluated give a value. For example in the following code consisting of a single C++ statement.

`int x = ((1+2)*3)/4;`

the statement allocates a stack variable named `x` and assigns its value to the result of the expression `((1+2)*3)/4`. Some programming languages only have expressions. For example most Lisp-family languages, like scheme, consist solely of expressions. This makes their syntax less complicated.

The syntax of a programming language is the rules that govern when a string of characters represents a valid sequence in the language. Semantics is the meaning of the sequence computationally, the result it produces. Some languages have a complicated syntax – C++ notoriously so. Other are very simple.

The Lisp family of languages use prefix notation to represent an expression. Prefix notation puts the operator first. For example the prefix notation of the expression above is `(/  (* (+ 1 2) 3) 4)`. In general the syntax is `(PROC ARG1 ARG2 ... ARGN)`, where `PROC` is a procedure with arguments `ARG1, ARG2, etc.`, where each argument can also be an expression. This is all the syntax there is in slisp. A slisp program then is just one, possibly very complex, expression. For example the following program is roughly equivalent to the C++ one above.

`(define x (/  (* (+ 1 2) 3) 4))`

It computes the result of the numerical expression and assigns the symbol x to have that value in the environment. The environment is a mapping from known symbols to either atoms or procedures. When the program starts there is a default environment that gets updated as the program runs adding symbol-atom mappings.

Consider another example, a program that finds the max of two numbers:

`(begin
 (define a 1)
 (define b pi)
 (if (< a b) b a)
 )`
 
The outermost expression is a special form named begin, that simply evaluates each argument in turn. Its first argument is an expression that when evaluated adds a symbol a to the environment with a value of 1. The second argument of begin is another expression, this time a special form `define`, that adds a symbol `b` to the environment with a value of the built-in symbol `pi` (that is, the symbol pi is in the default environment). The third argument is an expression that evaluates the expression `(< a b)` evaluating the expression `b` if the former evaluates to true, else evaluating the expression `a`. In this case, since 1 < pi, the if expression evaluates to pi (3.14159…). Notice white-space (e.g. tabs, spaces, and newlines) is unimportant in the syntax, but can be used to make the code more readable.

There are two equivalent views of the above syntax, as a list of lists or as a tree, called the abstract syntax tree or AST.

![AST](../master/images/ast.png)

When viewed as an AST, the evaluation of the program (the outer-most expression), corresponds to a post-order traversal of the tree, where the children are considered in order left-to-right. Each leaf expression, which in our language must be an atom, is evaluated (to itself). Then the special-form or procedure is evaluated with its arguments. This continues in the post-order traversal until the root of the AST is evaluated, giving the final result of the program, in this case the expression consisting of a single atom, the numerical value of `pi` (the max of 1 and pi). If at any time during the traversal this process cannot be completed, the program is invalid and an error is emitted (this will be specified more concretely below). Such an invalid program might be syntactically correct, but not semantically correct. For example suppose the programmer forgot to define a value for `a`, as in

`(begin
 (define b pi)
 (if (< a b) b a)
 )`
 
This is not a semantically valid program in our language and so interpreting it should produce an error.

The process of converting from the stream of text characters that constitute the candidate program to an AST is called parsing. This is typically broken up into two steps, tokenization and AST construction. The tokenization step converts the stream of characters into a list of tokens, where each token is a syntactically meaningful string. In our language this means splitting the stream into tokens consisting of `(`, `)`, or strings containing no white-space. For example the stream `(+ a ( - 4))` would become the list

`"(", "+", "a", "(", "-", "4", ")", ")"`
This can be implemented as a finite state machine operating on the input stream to produce the token stream.

The process of AST construction then uses the token list to build the AST. Every time a `(` token is encountered a new node in the AST is created using the following token in the list. Its children are then constructed recursively in the same fashion. This is called a recursive descent parser since it builds the AST top-down in a recursive fashion.

Simple syntax makes languages much easier to learn, since there is less to remember, and easier to program in. This makes lisp/scheme syntax a good candidate for scripting languages, programs written to extend the run-time capabilities of larger programs. In fact we will be using slisp in such a fashion. Scripting languages are generally interpreted rather than compiled. An interpreter reads the source code and computes it’s result and side effects, without converting (compiling) to machine code. Interpreters then are programs that read programs and produce output. They can usually be invoked a few different ways, for example reading the program to be interpreted from a file or interactively with user input. The latter is called a Read-Eval-Print-Loop or REPL.

## Language Specification for slisp
For this project our language will be kept relatively simple (we will extend it in next project). It can be specified as follows:

An `Atom` has a type and a value. The type may be one of `None`, `Boolean`, `Number`, or `Symbol`. The type `None` indicates the expression has no value. The possible values of a `Boolean` are `True` or `False`. The possible values of a `Number` are any IEEE double floating point value strictly parsed. The possible values of a `Symbol` is any string, not containing white-space, not possible to parse as a `Number`, not beginning with a numerical digit, and not one of the special forms defined below.

Examples of Numbers are: `1`, `6.02`, `-12`, `1e-4`

Examples of Symbols are: `a`, `length`, `start`

An _Expression_ is an `Atom` or a special form, followed by a (possibly empty) list of Expressions surrounded by parenthesis and separated by spaces. When an expression consists only of an atom the parenthesis may be omitted.


`<atom>`
`(<atom>)`
`(<atom> <expression> <expression> ...)`

There are three special-form expressions that begin with `define`, `begin`, and `if`. All other expressions are of the form `(<symbol> <expression> <expression> ...)` where the symbol names a _Procedure_. Procedures take the one or more arguments and return an expression according to their name. For example the expression `(+ a b c)` where `a`, `b`, and `c` are atoms representing numbers or expressions evaluating to such an atom and the result is an expression consisting of a single number atom. This expression is m-ary meaning it can take m arguments. Some expressions are binary, meaning they take only two arguments, i.e. `(- a b)` subtracts `b` from `a`. Other are unary, e.g. (not True). Thus all procedures have an arity of `1,2,...m`.

The _Environment_ is a mapping from symbols to either an Expression or a Procedure. The process of evaluating an Expression may modify the Environment (a side-effect) and results in an expression, which consists of a single Atom.

Our language has the following special-forms:

* `(define <symbol> <expression>)` adds a mapping from the symbol to the result of the expression in the environment. It is an error to redefine a symbol. This evaluates to the expression the symbol is defined as (maps to in the environment).
* `(begin <expression> <expression> ...)` evaluates each expression in order, evaluating to the last.
* `(if <expression1> <expression2> <expression3>)` evaluates to the result of `<expression2>` if `<expression1>` evaluates to True, else it evaluates to the result of `<expression3>`. It is an error if `<expression1>` does not result in a Boolean Atom. The conditional expression is always evaluated. The others are conditionally evaluated. Thus side effects are conditional.
* an Expression type `Point` with a value of two coordinates of type `Number`. Points are rendered as filled circles of radius two when drawn.
* an Expression type `Line` with a value of two `Points`.
* an Expression type `Arc` with a value of two `Points` and a `Number`. The first `Point` is the center of the arc, the second is the starting point of the arc, and the Number is the spanning angle in radians.
* an m-ary special form `draw` taking m arguments of a graphical type: `Point`, `Line`, or `Arc` and returning an _Expression_ of type `None`. This form draws each argument when in a graphical environment, upon successful completion of a call to eval(). It has no visual effect otherwise.


Our language has the following procedures:

* `not`, unary expression of Booleans, return the logical negation of the argument.
* `and`, m-ary expression of Booleans, return the logical conjunction of the arguments. There is no short circuit. All expressions are evaluated.
* `or`, m-ary expression of Booleans, return the logical disjunction of the arguments. There is no short circuit. All expressions are evaluated.
* `<`, binary expression of Numbers, returns True if the first argument is numerically less than the second, else False
* `<=`, binary expression of Numbers, returns True if the first argument is numerically less than or equal to the second, else False
* `>`, binary expression of Numbers, returns True if the first argument is numerically greater than the second, else False
* `>=`, binary expression of Numbers, returns True if the first argument is numerically greater than or equal to the second, else False
* `=`, binary expression of Numbers, returns True if the first argument is numerically equal to the second,, else False
* `+`, m-ary expression of Numbers, returns the sum of the arguments
* `-`, unary expression of Numbers, returns the negative of the argument
* `-`, binary expression of Numbers, return the first argument minus the second
* `*`, m-ary expression of Number arguments, returns the product of the arguments
* `/`, binary expression of Numbers, return the first argument divided by the second
* `log10`, unary expression of Numbers, computes the common (base-10) logarithm of the argument.
* `pow`, binary expression of Numbers, base and exponent, computes the result of raising base to the power exponent.
* a built-in binary procedure `point` taking two Numbers and returning a Point
* a built-in binary procedure `line` taking two Points and returning a Line
* a built-in tertiary procedure `arc` taking two Points and a spanning angle in radians, and returning an Arc

It is an error to evaluate a procedure with an incorrect arity or incorrect argument type.

Our language has the following built-in symbol:

* `pi`, a Number, evaluates to the numerical value of pi, given by `atan2(0, -1)`

Our language also supports comments using the traditional lisp notation. Any content after and including the character ; up to a newline is considered a comment and ignored by the parser.

## Text-based Interpreter Program Specifications

The interpreter module needs some user interface code. This should be a command-line application that compiles to an executable named `slisp.exe` on Windows and just `slisp` on mac/linux. The executable should be usable in one of three ways:

For executing short simple programs, pass a flag `-e` followed by a quoted string with the program. For example (> is the prompt):

`> slisp -e "(+ 1 (- 3) 12)"`

This should evaluate the program in the string and print the result in the format below or produce an appropriate error message, beginning with `“Error”`, if the program cannot be parsed or encounters a semantic error. If an error occurs slisp should return `EXIT_FAILURE` from main, otherwise it should return `EXIT_SUCCESS`.

For executing programs stored in external files, provide the file-name containing the slisp program as a command-line argument. For example, assuming a file named `mycode.slp` is in the current working directory with the executable:

`> slisp mycode.slp`

This should evaluate the program in the file and print the result in the format below or produce an appropriate error message, beginning with `“Error”`, if the program cannot be parsed or encounters a semantic error. If an error occurs slisp should return `EXIT_FAILURE` from main, otherwise it should return `EXIT_SUCCESS`.

For interactive execution of programs using a REPL, just type the executable name:

`> slisp`


## Graphical Program Specifications
The program `sldraw` should be a Qt application implemented in sldraw.cpp that compiles to an executable named sldraw.exe on Windows and just sldraw on mac/linux. It should instantiate and show the MainWindow class with a minimum size of `800x600`, then enter the Qt event loop. This program optionally takes a single command-line argument containing a script file name to read and evaluate, modifying the environment, before any user input can be enetered. If a parsing or semantic error occurs the program should still start but an error message shown in the MessageWidget.

![GUI Layout](../master/images/app_annotated.png)

After starting sldraw, the user should be able to type expressions into the REPLWidget. Informational messages, such as the resulting expression, should be displayed in the MessageWidget, replacing any existing text. Errors, either syntax or semantic, should also be displayed in the MessageWidget, but the highlight color of the widget should change to a red color and the text should be selected. Expressions that have graphical side effects should appear in the canvas window immediately after they are evaluated. For example running the `test_car.slp` example like so would produce the following window, ready for interaction.

`> ./sldraw /vagrant/tests/test_car`

![test_car.slp](../master/images/app_car.png)
