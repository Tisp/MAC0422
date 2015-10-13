from mem import FileMem, VirtMem
from mem_manager import MemManager
from linkedlist import LinkedList
from proc_manager import ProcessManager


def pager(virt, new):
	"""Implementacao do fifo."""
	pager.last = (pager.last+1) % virt.ram.npages
	vpage_old = virt.pagetable.index({'loc':'ram', 'page':pager.last})
	return vpage_old


def alloc(manager, size):
	"""Implementacao do first-fit."""
	for i,x in enumerate(manager.alloclist):
		if x['occupied']==False and x['size']>=size:
			return i


def main():
	#parametros do programa
	filename = "testdata/input0"
	ramfile = 'mem_ram'
	swapfile = 'mem_swap'
	pagesize = 2

	#leitura dos parametros
	inputfile = open(filename)
	ramsize,swapsize = (int(x) for x in inputfile.readline().split())

	inputlines = []
	for i in inputfile:
		line = i.split()
		new = (int(line[0]), int(line[2]), int(line[3]), [(int(x),int(y)) for x,y in zip(line[4::2],line[5::2])])
		inputlines.append(new)

	#inicializacao das estruturas
	pager.last = 0
	ram = FileMem(ramfile, ramsize, pagesize)
	swap = FileMem(swapfile, swapsize, pagesize)
	virt = VirtMem(ram, swap, pager)
	mem_man = MemManager(virt, alloc)
	proc_man = ProcessManager(mem_man)

	#loop principal
	time = 0;
	while proc_man.size()>0 or len(inputlines)>0:
		print("tempo:", time)
		proc_man.clean(time)
		while len(inputlines)>0 and inputlines[0][0]==time:
			proc_man.create(*inputlines[0][1:])
			del inputlines[0]
		proc_man.runall(time)
		time += 1
		print([(i,x) for i,x in enumerate(proc_man.proclist) if x is not None])
		print(mem_man.alloclist)
		print(virt)
		print(ram)
		print(swap)
		print()

	#ram = FileMem('mem_ram', 10, 2)
	#swap = FileMem('mem_swap', 6, 2)
	#pager.last = 0
	#virt = VirtMem(ram, swap, pager)
	#for i in range(virt.size):
		#virt.writebyte(i, i*i)
	#print(virt)
	#print(ram)
	#print(swap)

	#print()
	#manager = MemManager(virt, alloc)
	#print(manager.alloclist)
	#for i in range(2, 4):
		#manager.allocate(i*10, i*i-1)
		#print(manager.alloclist)
	#manager.allocate(200, 3)
	#print(manager.alloclist)
	#print(virt)

	#print()
	#for i in [200, 30]:
		#manager.delete(i)
		#print(manager.alloclist)

	#print()
	#procman = ProcessManager(manager)
	#proc = procman.create(5, 8, [[0,1], [1,4], [3,2], [5,0], [6,0]])
	#print(manager.alloclist)
	#for i in range(9):
		#procman.runall(i)
	#print(procman.size())


if __name__ == '__main__':
	main()
