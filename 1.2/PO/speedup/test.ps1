cd .\src
Set-Culture en-US
Invoke-Expression $compile
for($n=1; $n -le 5; $n++) {
	echo $n":"
	for($i=1; $i -le 20; $i++) {
		(Measure-Command {Invoke-Expression "Get-Content .\tests\test$n.in | $command"}).totalSeconds
	}
}
cd ..