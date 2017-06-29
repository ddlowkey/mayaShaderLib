@echo off
set PYTHONPATH=%HZ_CONFIG%\pythonGlobalLibrary;%PYTHONPATH%
call python %~dp0fkFolder_main.py %*