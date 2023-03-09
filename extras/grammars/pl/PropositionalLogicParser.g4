parser grammar PropositionalLogicParser;

start : formula EOF ;

formula : LeftParenthesis formula RightParenthesis  #parentheses

    | True                                          #plConstantTrue
    | False                                         #plConstantFalse

    | NegationOperator formula                      #plNegation
    | formula ConjunctionOperator formula           #plConjunction
    | formula DisjunctionOperator formula           #plDisjunction
    | formula ImplicationOperator formula           #plImplication
    | formula IffOperator formula                   #plIff

    | proposition                                   #plAtom
    ;

proposition : Identifier ;
