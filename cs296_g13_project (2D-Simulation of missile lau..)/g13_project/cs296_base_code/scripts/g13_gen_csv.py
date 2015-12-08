import subprocess
import re
subprocess.call(['mkdir','-p','data'])
subprocess.call(['rm','-f','./data/g13_lab09data.csv'])
outf=open("data/g13_lab09data.csv",'a')
for it in range(1,501):
	for rerun in range (1,51):
		p=subprocess.Popen(["./mybins/cs296_13_exe",str(it)],stdout=subprocess.PIPE)
		out=p.communicate()[0]
		match=re.findall(r"[0-9.]+",str(out))
		match.insert(1,str(rerun))
		out_str=",".join(match)+"\n"
		#print (out_str)
		outf.write(out_str)
