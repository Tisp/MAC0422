import logging
import time
from mem import FileMem, VirtMem
from mem_manager import MemManager
from proc_manager import ProcessManager
import allocators
import pagers


def main(ramfile, swapfile, pagesize, filename, pager, allocator, debugtime, extradebug=False):
	#parametros de debug
	#filename = 'testdata/input1'
	#ramfile = 'mem_ram'
	#swapfile = 'mem_swap'
	#pagesize = 16
	#debugtime = 2
	#allocator = 1
	#pager = 4
	extradebug = False
	nowait = False

	#configurando logger
	if extradebug:
		logging.basicConfig(format='%(message)s', level=logging.DEBUG)
	else:
		logging.basicConfig(format='%(message)s', level=logging.CRITICAL)

	#leitura dos parametros
	inputfile = open(filename)
	ramsize,swapsize = (int(x) for x in inputfile.readline().split())

	inputlines = []
	for i in inputfile:
		line = i.split()
		new = (int(line[0]), int(line[2]), int(line[3]), [(int(x),int(y)) for x,y in zip(line[4::2],line[5::2])])
		inputlines.append(new)

	allocator_nums = {1:'firstfit', 2:'nextfit', 3:'quickfit'}
	pager_nums = {1:'nru', 2:'fifo', 3:'secondchance', 4:'lru'}
	allocator = getattr(allocators, allocator_nums[allocator])
	pager = getattr(pagers, pager_nums[pager])

	#inicializacao das estruturas
	ram = FileMem(ramfile, ramsize, pagesize)
	swap = FileMem(swapfile, swapsize, pagesize)
	virt = VirtMem(ram, swap, pager)
	mem_man = MemManager(virt, allocator)
	proc_man = ProcessManager(mem_man)
	pagers.lru.usecounter = [0] * virt.npages

	#loop principal
	seconds = 0;
	while proc_man.size()>0 or len(inputlines)>0:
		logging.info("-"*50 + "Iteracao {}".format(seconds) + "-"*50)
		start_time = time.time()

		while len(inputlines)>0 and inputlines[0][0]==seconds:
			proc_man.create(*inputlines[0][1:])
			del inputlines[0]
		proc_man.runall(seconds)
		pager(virt, None, True)
		if seconds%pagers.cleartime == 0:
			virt.clear_read()
		proc_man.clean(seconds)
		seconds += 1

		if seconds%debugtime == 0 and not extradebug:
			print("Tempo: {}s".format(seconds))
			print("Mem virtual: {}".format(virt))
			print("Ram: {}".format(ram))
			print("Swap: {}".format(swap))
			print("Lista ligada de alocacao de memoria:")
			for i in mem_man.alloclist:
				print("\tId={}, ocupado={}, base={}, size={}".format(i['id'],i['occupied'],i['base'],i['size']))
			print()

		logging.info("\nLista de processos:")
		for i,x in enumerate(proc_man.proclist):
			if x is not None:
				logging.info("\tProcesso {}: endtime={}, accesslist={}".format(i,x['endtime'],x['accesslist']))
		logging.info("Lista de alocacao:")
		for i in mem_man.alloclist:
			logging.info("\tId={}, ocupado={}, base={}, size={}".format(i['id'],i['occupied'],i['base'],i['size']))
		logging.info("Mem virtual: {}".format(virt))
		logging.info("Ram e swap: {} {}".format(ram,swap))
		logging.info("Mapa de paginas: {}".format([x['loc']+'-'+str(x['page']) for x in virt.pagetable]))
		logging.info("Bits de leitura: {}\n".format(virt.readpages))

		while time.time()-start_time < 1 and not nowait:
			time.sleep(0.1)


if __name__ == '__main__':
	main(None, None, None, None, None, None, None)
