
#define T_KEYWORD 0
#define T_NUMBER 1
#define T_OPERATOR 2
#define T_ID 3

#define T_STATEMENT_DEF 0
#define T_STATEMENT_DIM 1
#define T_STATEMENT_END 2
#define T_STATEMENT_STOP 3
#define T_STATEMENT_FOR 4
#define T_STATEMENT_NEXT 5
#define T_STATEMENT_GOSUB 6
#define T_STATEMENT_RETURN 7
#define T_STATEMENT_GOTO 8
#define T_STATEMENT_IF 9
#define T_STATEMENT_THEN 10
#define T_STATEMENT_LET 11
#define T_STATEMENT_PRINT 12
#define T_STATEMENT_DATA 13
#define T_STATEMENT_READ 14
#define T_STATEMENT_REM 15

#define T_OPERATOR_ADD 16
#define T_OPERATOR_SUB 17
#define T_OPERATOR_NEG 18
#define T_OPERATOR_MULT 19
#define T_OPERATOR_DIV 20
#define T_OPERATOR_EXP 21
#define T_OPERATOR_EQTO 22
#define T_OPERATOR_NEQTO 23
#define T_OPERATOR_LEQTO 24
#define T_OPERATOR_GEQTO 25
#define T_OPERATOR_LTHAN 26
#define T_OPERATOR_GTHAN 27
#define T_OPERATOR_LBRACKET 28
#define T_OPERATOR_RBRACKET 29

#define T_COMMAND_RUN 30
#define T_COMMAND_STOP 31

typedef struct _token
{
	unsigned char type;
	unsigned int value;
}Token

int main()
{
	
}