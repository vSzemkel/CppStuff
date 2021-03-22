ECHO OFF
cls
del nrun.log
FOR /L %%N IN (0,1,18) DO interactive_runner.py py.exe dat_bae_testing_tool.py 1 -- dat_bae.exe >> nrun.log
type nrun.log | findstr "code: "
