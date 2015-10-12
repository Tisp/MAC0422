from mem import FileMem, VirtMem
from mem_manager import MemManager
from linkedlist import LinkedList


def pager(virt, new):
	rpage = 0
	vpage_old = virt.pagetable.index({'loc':'ram', 'page':rpage})
	return vpage_old


def alloc(manager, size):
	alloc.base += size
	alloc.counter += 1
	return alloc.counter-1, alloc.base-size


def main():
	ram = FileMem('mem_ram', 10, 2)
	swap = FileMem('mem_swap', 6, 2)
	virt = VirtMem(ram, swap, pager)
	for i in range(virt.size):
		virt.writebyte(i, i*i)
	print(virt)

	print()
	alloc.counter = 0
	alloc.base = 0
	manager = MemManager(virt, alloc)
	for i in range(1, 4):
		manager.create(i*10, i*i)
	print(virt)
	for i in range(1, 4):
		print("proc ", i*10, ": ", sep='', end='')
		for j in range(i*i):
			print(manager.read(i*10, j), end=' ')
		print()

	print()
	print(manager.alloclist)


if __name__ == '__main__':
	main()
