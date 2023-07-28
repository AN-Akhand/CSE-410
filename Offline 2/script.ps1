g++ .\main.cpp -o main.exe

Copy-Item '.\Test Cases\1\scene.txt' .\scene.txt
.\main.exe
Compare-Object (Get-Content .\stage1.txt) (Get-Content '.\Test Cases\1\stage1.txt')
Compare-Object (Get-Content .\stage2.txt) (Get-Content '.\Test Cases\1\stage2.txt')
Compare-Object (Get-Content .\stage3.txt) (Get-Content '.\Test Cases\1\stage3.txt')

Copy-Item '.\Test Cases\2\scene.txt' .\scene.txt
.\main.exe
Compare-Object (Get-Content .\stage1.txt) (Get-Content '.\Test Cases\2\stage1.txt')
Compare-Object (Get-Content .\stage2.txt) (Get-Content '.\Test Cases\2\stage2.txt')
Compare-Object (Get-Content .\stage3.txt) (Get-Content '.\Test Cases\2\stage3.txt')

Copy-Item '.\Test Cases\3\scene.txt' .\scene.txt
.\main.exe
Compare-Object (Get-Content .\stage1.txt) (Get-Content '.\Test Cases\3\stage1.txt')
Compare-Object (Get-Content .\stage2.txt) (Get-Content '.\Test Cases\3\stage2.txt')
Compare-Object (Get-Content .\stage3.txt) (Get-Content '.\Test Cases\3\stage3.txt')

Copy-Item '.\Test Cases\4\scene.txt' .\scene.txt
.\main.exe
Compare-Object (Get-Content .\stage1.txt) (Get-Content '.\Test Cases\4\stage1.txt')
Compare-Object (Get-Content .\stage2.txt) (Get-Content '.\Test Cases\4\stage2.txt')
Compare-Object (Get-Content .\stage3.txt) (Get-Content '.\Test Cases\4\stage3.txt')

