g++ .\main.cpp -o main.exe

.\main.exe '.\Test Cases\1\scene.txt'
Compare-Object (Get-Content .\stage1.txt) (Get-Content '.\Test Cases\1\stage1.txt')
Compare-Object (Get-Content .\stage2.txt) (Get-Content '.\Test Cases\1\stage2.txt')
Compare-Object (Get-Content .\stage3.txt) (Get-Content '.\Test Cases\1\stage3.txt')

.\main.exe '.\Test Cases\2\scene.txt'
Compare-Object (Get-Content .\stage1.txt) (Get-Content '.\Test Cases\2\stage1.txt')
Compare-Object (Get-Content .\stage2.txt) (Get-Content '.\Test Cases\2\stage2.txt')
Compare-Object (Get-Content .\stage3.txt) (Get-Content '.\Test Cases\2\stage3.txt')

.\main.exe '.\Test Cases\3\scene.txt'
Compare-Object (Get-Content .\stage1.txt) (Get-Content '.\Test Cases\3\stage1.txt')
Compare-Object (Get-Content .\stage2.txt) (Get-Content '.\Test Cases\3\stage2.txt')
Compare-Object (Get-Content .\stage3.txt) (Get-Content '.\Test Cases\3\stage3.txt')

.\main.exe '.\Test Cases\4\scene.txt'
Compare-Object (Get-Content .\stage1.txt) (Get-Content '.\Test Cases\4\stage1.txt')
Compare-Object (Get-Content .\stage2.txt) (Get-Content '.\Test Cases\4\stage2.txt')
Compare-Object (Get-Content .\stage3.txt) (Get-Content '.\Test Cases\4\stage3.txt')

