ECHO OFF
FOR /L %%N IN (0,1,100) DO interactive_runner.py py.exe dat_bae_testing_tool.py 1 -- dat_bae.exe >> nrun.log
type nrun.log | findstr "code: "
