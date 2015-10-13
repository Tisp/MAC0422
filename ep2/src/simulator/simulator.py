from mem import FileMem, VirtMem
from mem_manager import MemManager
from linkedlist import LinkedList
from proc_manager import ProcessManager


def pager(virt, new):
	rpage = 0
	vpage_old = virt.pagetable.index({'loc':'ram', 'page':rpage})
	return vpage_old


def alloc(manager, size):
	return manager.alloclist.size - 1


def main():
	ram = FileMem('mem_ram', 10, 2)
	swap = FileMem('mem_swap', 6, 2)
	virt = VirtMem(ram, swap, pager)
	for i in range(virt.size):
		virt.writebyte(i, i*i)
	print(virt)

	print()
	manager = MemManager(virt, alloc)
	print(manager.alloclist)
	for i in range(2, 4):
		manager.allocate(i*10, i*i-1)
		print(manager.alloclist)
	manager.allocate(200, 3)
	print(manager.alloclist)
	print(virt)

	print()
	for i in [200, 30]:
		manager.delete(i)
		print(manager.alloclist)

	print()
	procman = ProcessManager(manager)
	proc = procman.create(5, [[0,1], [1,4], [3,2], [5,0], [6,0]])
	print(manager.alloclist)
	for i in range(6):
		procman.runall(i)
	print(procman.size())


if __name__ == '__main__':
	main()
