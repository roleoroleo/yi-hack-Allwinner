@ECHO OFF
SETLOCAL EnableDelayedExpansion

FOR /F %%x in ('dir /B /D /A:D *') do (
    SET DIR=%%x
    FOR /F "tokens=*" %%g IN ('type !DIR!\filename') do (SET FILENAME=%%g)
    DEL !DIR!\home_!FILENAME!.gz
)
