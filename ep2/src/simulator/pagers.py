def fifo(virt, new):
	"""Implementacao do fifo."""
	fifo.last = (fifo.last+1) % virt.ram.npages
	vpage_old = virt.pagetable.index({'loc':'ram', 'page':fifo.last})
	return vpage_old


fifo.last = -1
