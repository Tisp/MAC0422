import os
import time
from simulator import interface


def main():

	os.chdir("data")
	filename = "test.bin"

	for status in ["0mb", "10mb", "50mb"]:
		try: os.remove("test.bin")
		except: pass
		interface.mount(filename)
		interface.cp(status+".bin", "/"+status+".bin")

		cp_1mb = []
		cp_10mb = []
		cp_30mb = []
		rm_1mb = []
		rm_10mb = []
		rm_30mb = []
		rmdir_empty = []
		rmdir_full = []

		for i in range(30):
			start = time.time()
			interface.cp("1mb.bin", "/1mb.bin")
			interface.flush()
			cp_1mb.append((time.time()-start) * 1000)

			start = time.time()
			interface.rm("/1mb.bin")
			interface.flush()
			rm_1mb.append((time.time()-start) * 1000)

			start = time.time()
			interface.cp("10mb.bin", "/10mb.bin")
			interface.flush()
			cp_10mb.append((time.time()-start) * 1000)

			start = time.time()
			interface.rm("/10mb.bin")
			interface.flush()
			rm_10mb.append((time.time()-start) * 1000)

			start = time.time()
			interface.cp("30mb.bin", "/30mb.bin")
			interface.flush()
			cp_30mb.append((time.time()-start) * 1000)

			start = time.time()
			interface.rm("/30mb.bin")
			interface.flush()
			rm_30mb.append((time.time()-start) * 1000)

			for j in range(30):
				dirname = "/dir" * j
				interface.mkdir(dirname)
			start = time.time()
			interface.rmdir("/dir")
			interface.flush()
			rmdir_empty.append((time.time()-start) * 1000)

			for j in range(30):
				dirname = "/dir" * j
				interface.mkdir(dirname)
				for k in range(210):
					interface.cp("2kb.bin", dirname+"/arq"+str(k))
			start = time.time()
			interface.rmdir("/dir")
			interface.flush()
			rmdir_full.append((time.time()-start) * 1000)

		print("Estado " + status)
		print("cp_1mb =", (sum(cp_1mb)/len(cp_1mb)))
		print("cp_10mb =", (sum(cp_10mb)/len(cp_10mb)))
		print("cp_30mb =", (sum(cp_30mb)/len(cp_30mb)))
		print("rm_1mb =", (sum(rm_1mb)/len(rm_1mb)))
		print("rm_10mb =", (sum(rm_10mb)/len(rm_10mb)))
		print("rm_30mb =", (sum(rm_30mb)/len(rm_30mb)))
		print("rmdir_empty =", (sum(rmdir_empty)/len(rmdir_empty)))
		print("rmdir_full =", (sum(rmdir_full)/len(rmdir_full)))
		print()


if __name__ == '__main__':
	main()
