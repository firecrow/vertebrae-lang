import re
o=open("incr.c", "r")
s=o.read()
sections = re.split("\/\* ----- (.*) -----.*\n",s, re.MULTILINE)
keys = int(len(sections)/2)
for idx in range(0, keys):
  slot = idx*2
  print(slot)
  title = sections[slot+1]
  print(title)
  content = sections[slot]

  fname=title+"_incr.c"
  f=open(fname, "w")
  f.write(content)
  f.close()
