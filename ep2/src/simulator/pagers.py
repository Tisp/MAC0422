def fifo(virt, new, notifify=False):
	if notifify: return
	fifo.last = (fifo.last+1) % virt.ram.npages
	vpage_old = virt.pagetable.index({'loc':'ram', 'page':fifo.last})
	return vpage_old

def secondchance(virt, new, notifify=False):
	if notifify:
			return
	secondchance.last = (secondchance.last+1) % virt.ram.npages
	if virt.readpages[secondchance.last] == False:
		vpage_old = virt.pagetable.index({'loc':'ram', 'page':secondchance.last})
		return vpage_old
	else:
		virt.readpages[secondchance.last] = False
		return secondchance(virt, new)

def nru(virt, new, notifify=False):
	if notifify: return
	for i,x in enumerate(virt.readpages):
		if x == False:
			return virt.pagetable.index({'loc':'ram', 'page':i})
	nru.last = (nru.last+1) % virt.ram.npages
	return virt.pagetable.index({'loc':'ram', 'page':nru.last})

def lru(virt, new, notifify=False):
	if notifify:
		for i,x in enumerate(virt.readpages):
			if x:
				virtpage = virt.pagetable.index({'loc':'ram', 'page':i})
				lru.usecounter[virtpage] += 1
	else:
		smallest_value = -1
		for i in range(virt.ram.npages):
			virtpage = virt.pagetable.index({'loc':'ram', 'page':i})
			if lru.usecounter[virtpage] < smallest_value or smallest_value == -1:
				smallest_value = lru.usecounter[virtpage]
				ret = virtpage
		return ret


fifo.last = -1
nru.last = -1
secondchance.last = -1
cleartime = 3
