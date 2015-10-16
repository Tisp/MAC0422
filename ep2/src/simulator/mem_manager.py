import logging
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

	def allocate(self, uid, size):
		"""Aloca memoria de tamanho size para um processo com o uid fornecido."""
		size = ceil(size/self.allocsize)
		pos = self.alloc(self, size)
		new = self.separate(pos, size)
		new['id'] = uid
		logging.debug("\tAlocando segmento de tamanho {} com base em {} para processo {}".format(size*self.allocsize,new['base']*self.allocsize,uid))
		logging.debug("\tEscrevendo {} nos enderecos de {} a {}".format(uid,new['base']*self.allocsize,new['base']*self.allocsize+size*self.allocsize-1))
		for i in range(size*self.allocsize):
			self.mem.writebyte((self.allocsize*new['base'])+i, uid)

	def delete(self, uid):
		"""Libera a memoria alocada para o processo uid."""
		entry = self.get_alloc(uid)
		logging.debug("\tRemovendo segmento de tamanho {} com base em {} do processo {}".format(entry['size']*self.allocsize,entry['base']*self.allocsize,uid))
		logging.debug("\tEscrevendo 255(-1) nos enderecos de {} a {}".format(entry['base']*self.allocsize,entry['base']*self.allocsize+entry['size']*self.allocsize-1))
		for i in range(entry['size']*self.allocsize):
			self.mem.writebyte((self.allocsize*entry['base'])+i, 255)
		entry['id'] = None
		entry['occupied'] = False
		self.join()

	def read(self, uid, pos):
		"""Le uma posicao de memoria do processo com o uid fornecido."""
		realpos = self.allocsize*self.get_alloc(uid)['base'] + pos
		logging.debug("\tTraduzindo endereco {} do processo {} para {}".format(pos,uid,realpos))
		return self.mem.readbyte(realpos)

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

	def get_alloc(self, uid):
		"""Retorna a entrada do processo com o id fornecido na lista de alocacao."""
		for i in self.alloclist:
			if i['id'] == uid:
				return i
