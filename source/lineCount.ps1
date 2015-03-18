# Test-Filtering.PS1
Get-ChildItem navmanager, navsys, navsystest -Include *.h,*.c,*.cpp -Recurse | Get-Content | Measure-Object -Word -Line -Character
