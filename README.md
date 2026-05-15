# simple virtual machine

the machine has stack and 7 instructions

1. push <num> # push a number on to the stack

2. pop # pop off the first number on the stack

3. add # pop off the top 2 items on the stack and push their sum on to the stack. (remember you can add negative numbers, so you have subtraction covered too). You can also get multiplication my creating a loop using some of the other instructions with this one.

4. ifeq <address> # examine the top of the stack, if it's 0, continue, else, jump to <address> where <address> is a lable

5. jump <address> # jump to a lable

6. print # print the value at the top of the stack

7. dup # push a copy of what's at the top of the stack back onto the stack.

# Run code
```bash
svm [filename]

```
