import sys

for line in sys.stdin:
	line = line.strip()
	if len(line)<5:
		continue;
	items =[]
	items = line.split("\t")
	if len(items)!=6:
		continue;
	words = items[5].strip();
	word_item =[]
	word_item = words.split();
	for it in word_item:
		it=it.strip()
		if len(it)>3 and len(it)<9:
			print items[0]+"\t",items[1]+"\t",items[2]+"\t",items[3]+"\t",items[4]+"\t",it
