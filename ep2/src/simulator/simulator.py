from mem import *


def pager(virt, new):
	rpage = 0
	vpage_old = virt.table.index({'loc':'ram', 'page':rpage})
	return vpage_old


def main():
	mf1 = FileMem('mem_ram', 12, 3)
	mf2 = FileMem('mem_swap', 6, 3)
	mv = VirtMem(mf1, mf2, pager)

	print("===== inicializando array")
	for i in range(mv.size):
		print("pos=", i, " inicial=", mv.readbyte(i), end=" ", sep="")
		mv.writebyte(i, i)
		print("novo=", mv.readbyte(i), sep="")

	print("\n===== lendo, re-escrevendo e escrevendo 200 aleatoriamente")
	for i in [9,1,16,4,17, 13]:
		mv.writebyte(i, mv.readbyte(i))
		print("pos=", i, " antigo=", mv.readbyte(i), end=" ", sep="")
		mv.writebyte(i, 200)
		print("novo=", mv.readbyte(i), sep="")

	print("\n===== lendo resultado")
	print("virt = ", end="")
	for i in range(mv.size): print(mv.readbyte(i), end=" ")
	print("\nram = ", end="")
	for i in range(mf1.size): print(mf1.readbyte(i), end=" ")
	print("\nswap = ", end="")
	for i in range(mf2.size): print(mf2.readbyte(i), end=" ")
	print()


if __name__ == '__main__':
	main()
