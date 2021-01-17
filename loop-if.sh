a=2
if echo $a
then
while [$a -lt 8]
do
	a=$a+1
	echo a=$a
done
fi
