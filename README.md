# Coding competitions

## Initialize

For competitions here is utility *competition_init*  
It can initialize new solution file based on small [template](https://github.com/vSzemkel/CppStuff/blob/master/competition_template.cpp) file. Just write:

`$ ./competition_init <task_name>`

This produces file named <task_name>.cpp and empty file for input data: <task_name>.in<br>
One can embed input data under "Input:" section in comments and later restore input file<br>
with exactly the same command as for initialization if there is no <task_name>.in yet.<br>
So you can't accidentlly overwrite <task_name> files in a rush with this utility.

`$ ./competition_init <task_name>`  
`Input data for <task_name> restored`

## Run

Run the solution with command:

`$ ./<task_name>.o < <task_name>.in`

or configure *launch.json* in VS Code with settings:

`"program": "${workspaceFolder}/<task_name>.exe",`  
`"args": ["<", "${workspaceFolder}/<task_name>.in"],`

or, if you got expected output listing, run this way:

`$ time (./<task_name> < <task_name>.in) > my.out && diff my.out expected.out`
