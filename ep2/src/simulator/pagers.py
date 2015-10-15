def fifo(virt, new):
	"""Implementacao do fifo."""
	fifo.last = (fifo.last+1) % virt.ram.npages
	vpage_old = virt.pagetable.index({'loc':'ram', 'page':fifo.last})
	return vpage_old

def secondchance(virt, new):
	"""Implementacao do second chance."""
	secondchance.last = (secondchance.last+1) % virt.ram.npages
	if virt.readpages[secondchance.last] == False:
		vpage_old = virt.pagetable.index({'loc':'ram', 'page':secondchance.last})
		return vpage_old
	else:
		virt.readpages[secondchance.last] = False
		return secondchance(virt, new)


fifo.last = -1
secondchance.last = -1
cleartime = 3
