from linkedlist import LinkedList
from math import ceil


class MemManager:
	"""Gerencia a alocacao e intermedia o acesso a um objeto de memoria."""

	def __init__(self, mem, alloc):
		"""Recebe uma memoria mem e uma funcao alloc que implementa o algoritmo de alocacao."""
		self.mem = mem
		self.alloc = alloc
		self.allocsize = mem.pagesize
		self.size = mem.npages
		self.alloclist = LinkedList()
		self.alloclist.append({'id':None, 'size':self.size, 'base':0, 'occupied':False})

	def allocate(self, id, size):
		"""Aloca memoria de tamanho size para um processo de com o id fornecido."""
		size = ceil(size/self.allocsize)
		pos = self.alloc(self, size)
		new = self.separate(pos, size)
		new['id'] = id
		for i in range(size*self.allocsize):
			self.mem.writebyte((self.allocsize*new['base'])+i, id)

	def delete(self, id):
		"""Libera a memoria alocada para o processo id."""
		entry = self.get_alloc(id)
		entry['id'] = None
		entry['occupied'] = False
		self.join()

	def read(self, id, pos):
		"""Le uma posicao de memoria do processo com o id fornecido."""
		pos += self.allocsize + self.get_alloc(id)['base']
		return self.mem.readbyte(pos)

	def separate(self, pos, size):
		"""Separa uma regiao de memoria de tamanha size de uma faixa de memoria vazia na posicao pos e retorna a regiao separada."""
		freespace = self.alloclist.get(pos)
		new = {'id':None, 'size':size, 'base':freespace['base'], 'occupied':True}
		freespace['base'] += size
		freespace['size'] -= size
		if freespace['size'] == 0:
			self.alloclist.remove(pos)
		self.alloclist.add(pos, new)
		return new

	def join(self):
		"""Percorre a memoria juntando regioes livres adjacentes."""
		if self.alloclist.size <= 1:
			return
		toremove = []
		iterator = iter(self.alloclist)
		prev = next(iterator)
		prev_index = 0
		for i in iterator:
			if prev['occupied'] == False and i['occupied'] == False:
				i['size'] += prev['size']
				i['base'] -= prev['size']
				toremove.append(prev_index)
			prev = i
			prev_index += 1
		for i in reversed(toremove):
			self.alloclist.remove(i)

	def get_alloc(self, id):
		"""Retorna a entrada do processo com o id fornecido na lista de alocacao."""
		for i in self.alloclist:
			if i['id'] == id:
				return i
