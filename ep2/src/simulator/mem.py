import logging
from io import SEEK_SET


class Mem:
	"""Interface basica de objetos de memoria."""

	def __init__(self, size, pagesize):
		"""Recebe o tamanho total e o tamanho da pagina."""
		self.size = size
		self.pagesize = pagesize
		self.npages = size//pagesize

	def readbyte(self, pos):
		"""Retorna o conteudo da posicao de memoria pos."""
		pass

	def writebyte(self, pos, data):
		"""Escreve data na posicao de memoria pos."""
		pass

	def readpage(self, page):
		"""Le e retorna o conteudo da pagina de indice page."""
		ret = []
		for i in range(self.pagesize):
			pos = self.pagesize*page + i
			ret.append(self.readbyte(pos))
		return ret

	def writepage(self, page, data):
		"""Escreve a pagina data na posicao de indice page."""
		for i in range(self.pagesize):
			pos = self.pagesize*page + i
			self.writebyte(pos, data[i])

	def __str__(self):
		ret = ''
		for i in range(self.size):
			ret += repr(self.readbyte(i)) + ', '
		return '[' + ret.strip(', ') + ']'


class FileMem(Mem):
	"""Interface para lidar com arquivos como se fossem memoria."""

	def __init__(self, file, size, pagesize):
		super().__init__(size, pagesize)
		self.file = open(file, 'w+b', 0)
		written = self.file.write(bytes([255]*size))
		assert written == size

	def readbyte(self, pos):
		self.file.seek(pos, SEEK_SET)
		return int(self.file.read(1)[0])

	def writebyte(self, pos, data):
		self.file.seek(pos, SEEK_SET)
		self.file.write(bytes([data]))

	def readpage(self, page):
		self.file.seek(page*self.pagesize, SEEK_SET)
		return [int(x) for x in self.file.read(self.pagesize)]

	def writepage(self, page, data):
		self.file.seek(page*self.pagesize, SEEK_SET)
		self.file.write(bytes(data))


class VirtMem(Mem):
	"""Junta duas memorias (uma ram e uma swap) em uma memoria virtual."""

	def __init__(self, ram, swap, pager):
		"""Recebe as memorias ram e swap e uma funcao pager que implementa o algoritmo de subtituicao de pagina."""
		super().__init__(ram.size+swap.size, ram.pagesize)
		self.ram = ram
		self.swap = swap
		self.pager = pager
		self.readpages = []
		for i in range(ram.npages):
			self.readpages.append(False)
		self.pagetable = []
		for i in range(ram.npages):
			self.pagetable.append({'loc':'ram', 'page':i})
		for i in range(swap.npages):
			self.pagetable.append({'loc':'swap', 'page':i})

	def readbyte(self, vpos):
		rpos = self.fetch(vpos)
		self.readpages[rpos//self.pagesize] = True
		logging.debug("\t\tLendo endereco {} na posicao da ram {}".format(vpos,rpos))
		return self.ram.readbyte(rpos)

	def writebyte(self, vpos, data):
		rpos = self.fetch(vpos)
		self.readpages[rpos//self.pagesize] = True
		self.ram.writebyte(rpos, data)

	def clear_read(self):
		"""Limpa os bits de leitura das paginas."""
		logging.debug("Limpando bits de leitura de pagina")
		for i in range(self.ram.npages):
			self.readpages[i] = False

	def fetch(self, vpos):
		"""Retorna o endereco da ram onde esta a posicao de memoria virtual vpos (trazendo-a para a ram se necessario)."""
		vpage = vpos // self.pagesize
		offset = vpos % self.pagesize
		if self.pagetable[vpage]['loc'] == 'ram':
			rpage = self.pagetable[vpage]['page']
		else:
			vpage_toremove = self.pager(self, vpage)
			logging.debug("\t\tPage fault no endereco {}, trocando pagina {} da ram com a pagina {} do swap".format(vpos,self.pagetable[vpage_toremove]['page'],self.pagetable[vpage]['page']))
			self.swap_page(vpage, vpage_toremove)
			rpage = self.pagetable[vpage]['page']
		return self.pagesize*rpage + offset

	def swap_page(self, newpage, oldpage):
		"""Troca uma pagina que esta no swap(newpage) com uma pagina que esta na ram(oldpage)."""
		assert self.pagetable[oldpage]['loc']=='ram' and self.pagetable[newpage]['loc']=='swap'
		realpage = self.pagetable[oldpage]['page']
		swappage = self.pagetable[newpage]['page']
		olddata = self.ram.readpage(realpage)
		newdata = self.swap.readpage(swappage)
		self.ram.writepage(realpage, newdata)
		self.swap.writepage(swappage, olddata)
		self.pagetable[newpage] = {'loc':'ram', 'page':realpage}
		self.pagetable[oldpage] = {'loc':'swap', 'page':swappage}

	def __str__(self):
		"""Le a memoria virtual sem mecher nas paginas, apenas para debug."""
		ret = ''
		for i in range(self.size):
			offset = i % self.pagesize
			vpage = i // self.pagesize
			if self.pagetable[vpage]['loc'] == 'ram':
				dev = self.ram
			if self.pagetable[vpage]['loc'] == 'swap':
				dev = self.swap
			rpage = self.pagetable[vpage]['page']
			rpos = self.pagesize*rpage + offset
			ret += str(dev.readbyte(rpos)) + ", "
		return '[' + ret.strip(', ') + ']'
