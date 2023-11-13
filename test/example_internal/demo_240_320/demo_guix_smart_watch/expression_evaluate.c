/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <math.h>
#include "gx_api.h"
#include "demo_guix_smart_watch.h"

#define  PRECISSION 10

/* Define the struct of stack to hold operand. */
typedef struct STACK_OPERAND_STRUCT{
    double operand[128];
    int    top;
}STACK_OPERAND;

/* Define the struct of stack to hold operator. */
typedef struct STACK_OPERATION_STRUCT{
    char operation[128];
    int  top;
}STACK_OPERATION;

UINT popOperand(STACK_OPERAND *stack, double *operand);
UINT pushOperand(STACK_OPERAND *stack, double operand);
UINT popOperation(STACK_OPERATION *stack, char *operation);
UINT pushOperation(STACK_OPERATION *stack, char operation);
double calculate(double a, char operation, double b);
UINT priority_compare(char operation1, char operation2);
GX_CHAR *dtoa(double ans);

/* Evaluate the arithmetic expression. */
GX_CHAR *expression_evaluate(GX_CHAR *expression, GX_BOOL persent_flag)
{
    STACK_OPERAND   stack_operand;
    STACK_OPERATION stack_operation;
    INT             len = string_length_get(expression, MAX_EXPRESSION_LENGTH);
    double          num, ans, a, b;
    GX_CHAR         operation;
    GX_BOOL         is_decimal;
    GX_VALUE        decimal_len;
    GX_CHAR         ch;
    GX_VALUE        index = 0;

    /* Initialize stack. */
    stack_operand.top = -1;
    stack_operation.top = -1;

    while (index < len)
    {
        /* Read operand to stack operand. */
        ch = expression[index];

        if ((ch >= '0') && (ch <= '9'))
        {
            num = 0;
            decimal_len = 0;
            is_decimal = GX_FALSE;

            while (((ch >= '0') && (ch <= '9')) || ch == '.')
            {
                if (is_decimal)
                {
                    decimal_len++;
                }

                if (ch == '.')
                {
                    is_decimal = GX_TRUE;
                }
                else
                {
                    num = num * 10 + ch - '0';
                }

                index++;
                ch = expression[index];
            }

            if (is_decimal)
            {
                while (decimal_len--)
                {
                    num /= 10;
                }
            }

            pushOperand(&stack_operand, num);
        }
        else
        {
            /* Push current operation to stack_operation when it's priority higher than the stack top operation. */
            if ((stack_operation.top == -1) || !priority_compare(stack_operation.operation[stack_operation.top], ch))
            {
                pushOperation(&stack_operation, ch);
            }
            else
            {
                /* Pop out two operands and one operation and push the calculate result to stack_operand,
                then push the current operation to the stack_operation. */
                if (stack_operand.top < 1)
                {
                    return "Error Format";
                }

                popOperand(&stack_operand, &b);
                popOperand(&stack_operand, &a);
                popOperation(&stack_operation, &operation);
                ans = calculate(a, operation, b);

                pushOperand(&stack_operand, ans);
                pushOperation(&stack_operation, ch);
            }
            index++;
        }
    }

    while (stack_operation.top >= 0)
    {
        if (stack_operand.top < 1)
        {
            return "Error Format";
        }
        popOperand(&stack_operand, &b);
        popOperand(&stack_operand, &a);
        popOperation(&stack_operation, &operation);
        ans = calculate(a, operation, b);

        pushOperand(&stack_operand, ans);
    }

    /* Pop out evaluate result. */
    popOperand(&stack_operand, &ans);

    if (persent_flag)
    {
        ans /= 100;
    }

    /* Round to the 10th decimal. */
    if (ans >= 0)
    {
        ans += 0.00000000005;
    }
    else
    {
        ans += -0.00000000005;
    }

    return dtoa(ans);
}

/* Pop an operand from operand stack. */
UINT popOperand(STACK_OPERAND *stack, double *operand)
{
GX_VALUE top;
    
    top = stack -> top;
    if(top >= 0)
    {
        *operand = stack -> operand[top];
        stack -> top --;
        return 0;
    }
    return 1;
}

/* Push an operand to operand stack. */
UINT pushOperand(STACK_OPERAND *stack, double operand)
{
    stack -> top ++;
    stack -> operand[stack -> top] = operand;
    return 0;
}

/* Pop an operator from operation stack. */
UINT popOperation(STACK_OPERATION *stack, char *operation)
{
GX_VALUE top;

    top = stack -> top;
    if (top >= 0)
    {
        *operation = stack -> operation[top];
        stack -> top --;
        return 0;
    }
    return 1;
}

/* Push an operator to operation stack. */
UINT pushOperation(STACK_OPERATION *stack, char operation)
{
    stack -> top ++;
    stack -> operation[stack -> top] = operation;
    return 0;
}

/* Calculate simple expression that contain 2 opepands and 1 operation. */
double calculate(double a, char operation, double b)
{
    switch(operation)
    {
    case '+':
        return a + b;

    case '-':
        return a - b;

    case '*':
        return a * b;

    case '/':
        return a / b;
    }
    return 0;
}

/* Judge whether the priority of operation1 is larger or equal to operation2. */
UINT priority_compare(char operation1, char operation2)
{
    if ((operation1 == '*') ||(operation1 == '/'))
    {
        return 1;
    }
    else if ((operation2 == '+') ||(operation2 == '-'))
    {
        return 1;
    }
    return 0;
}

/* Convert a double value to an ASCII string. */
GX_CHAR *dtoa(double ans)
{
INT     digit_num = 1;
INT     integer;
INT     decimal[PRECISSION];
INT     index = 0;
INT     precision = PRECISSION;
double  temp;
int     result_index = 0;
int     exponent = 0;
GX_BOOL exp = GX_FALSE;
static GX_CHAR result[128];

    if (ans < 0)
    {
        result[result_index++] = '-';
        ans *= -1;
    }
    if (ans >= 10000000000000000.)
    {
        exp = GX_TRUE;
    }

    if (!exp)
    {
        /* Get integer number. */
        temp = ans;
        while (temp >= 10)
        {
            temp /= 10;
            digit_num++;
        }

        /* Save the integer part to 'result' array. */
        if (digit_num > 1)
        {
            temp = pow(10, digit_num - 1);
            for (index = 0; index < digit_num; index++)
            {
                integer = (INT)(ans / temp);
                result[result_index++] = integer + '0';
                ans -= integer * temp;
                temp /= 10;
            }
        }
        else
        {
            integer = (INT)ans;
            result[result_index++] = integer + '0';
            ans -= integer;
        }
    }
    else
    {

        while (ans >= 10)
        {
            ans /= 10;
            exponent++;
        }

        result[result_index++] = (INT)ans + '0';
        ans -= (INT)ans;
    }

    /* Save the decimal to 'decimal' array. */
    for (index = 0; index < precision; index++)
    {
        decimal[index] = (INT)(ans * 10);
        ans = ans * 10 - decimal[index];
    }

    for (index = precision - 1; index >= 0; index--)
    {
        if (decimal[index] == 0)
        {
            precision--;
        }
        else
        {
            break;
        }
    }

    if (precision)
    {
        result[result_index++] = '.';
    }

    /* Save the decimal to 'result' array. */
    for (index = 0; index < precision; index++)
    {
        result[result_index++] = decimal[index] + '0';
    }

    if (!exp)
    {
        result[result_index] = '\0';
    }
    else
    {
        result[result_index++] = 'E';
        result[result_index++] = '+';
        gx_utility_ltoa(exponent, result + result_index, 3);
    }

    return result;
}