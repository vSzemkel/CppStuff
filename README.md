# Coding competitions

For competitions here is utility *competition_init*  
It can initialize new solution file based on small [template](https://github.com/vSzemkel/CppStuff/blob/master/competition_template.cpp) file. Just write:

`$ ./competition_init <task_name>`

This produces file named <task_name>.cpp and empty file for input data: <task_name>.in  
One can embed input data under "Input:" section in comments and later restore input file  
with exactly the same command as for initialization.

`$ ./competition_init <task_name>`  
`Input data for <task_name> restored`

And You can't accidentlly overwrite <task_name>.cpp in a rush with this utility of course..
