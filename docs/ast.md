## Structure AST

### File : Racine

- [ClassDecl](#classdecl) (n)

### ClassDecl

- Modifieurs (public, private)
- Nom (String)
- Nom de la superclasse (String) (optionnel)
- [Field](#field) (n)
- [Constructor](#constructor) (n)
- [Method](#method) (n)

### Field

- Modifieurs (public, private, static)
- [Type](#type) (1)
- Nom (String)
- [Expression](#expression) (1) (optionnel)

### Constructor

- [Type](#type) (1)
- [Parameter](#parameter) (n)
- [Statement](#statement) (1)

### Method

- Modifieurs (public, private)
- Nom (String)
- [Type](#type) (1)
- [Parameter](#parameter) (n)

### Parameter

- [Type](#type) (1)
- Nom (String)

### Type

- Nom du type (String)

### Statement

Pas un vrai noeud mais une interface pour les types suivants :
- [Expression](#expression)
- [ReturnStmt](#returnstmt)
- [IfStmt](#ifstmt)
- [ForStmt](#forstmt)
- [Block](#block)
- [VarDecl](#vardecl)

### ReturnStmt

- [Expression](#expression) (1) (optionnel)

### IfStmt

- Condition : [Expression](#expression) (1)
- Instruction du if : [Statement](#statement) (1)
- Instruction du else : [Statement](#statement) (1) (optionnel)

### ForStmt

- [Statement](#statement) (1) (optionnel)
- [Expression](#expression) (1) (optionnel)
- [Expression](#expression) (1) (optionnel)
- Instruction du for : [Statement](#statement) (1)

### Block

- [Statement](#statement) (n)

### VarDecl

- [Type](#type) (1)
- Nom (String)
- [Expression](#expression) (1) (optionnel)

### Expression

Pas un vrai noeud mais une interface pour les types suivants :
- [Literal](#literal)
- Nom de variable (String)
- [BinaryOp](#binaryop)
- [UnaryOp](#unaryop)
- [CallExpr](#callexpr)
- [InstanceOf](#instanceof)
- [NewExpr](#newexpr)
- [FieldAccess](#fieldaccess)
- [Cast](#cast)

### Literal

L'un des objets suivants :
- Entier (int)
- Booléen (boolean)
- Chaîne de caractères (String)

### BinaryOp

- Opérateur (int)
- [Expression](#expression) (1)
- [Expression](#expression) (1)

### UnaryOp

- Opérateur (int)
- [Expression](#expression) (1)

### CallExpr

- Appellé : [Expression](#expression) (1)
- Arguments : [Expression](#expression) (n)

### InstanceOf

- [Expression](#expression) (1)
- [Type](#type) (1)

### NewExpr

- [Type](#type) (1)
- Arguments : [Expression](#expression) (n)

### FieldAccess

- [Expression](#expression) (1)
- Nom du champ (String)

### Cast

- [Type](#type) (1)
- [Expression](#expression) (1)