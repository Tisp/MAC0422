class Node:
	"""Representa um no da lista ligada."""

	def __init__(self, data, prev, next):
		self.data = data
		self.next = next
		self.prev = prev


class LinkedList:
	"""Implementacao de uma lista duplamente ligada com cabeca."""

	def __init__(self):
		self.head = Node(None, None, None)
		self.size = 0

	def add(self, pos, data):
		self.size += 1
		if pos == 0:
			prev = self.head
		else:
			prev = self.get_node(pos-1)
		new = Node(data, prev, prev.next)
		if prev.next is not None:
			prev.next.prev = new
		prev.next = new

	def remove(self, pos):
		self.size -= 1
		node = self.get_node(pos)
		if node.prev is not None:
			node.prev.next = node.next
		if node.next is not None:
			node.next.prev = node.prev

	def get(self, pos):
		return self.get_node(pos).data

	def append(self, data):
		self.add(self.size, data)

	def get_node(self, pos):
		ret = self.head.next
		for i in range(pos):
			ret = ret.next
		return ret

	def __iter__(self):
		curr = self.head.next
		for i in range(self.size):
			yield curr.data
			curr = curr.next

	def __str__(self):
		ret = ''
		for i in self:
			ret += repr(i) + ', '
		return '[' + ret.strip(', ') + ']'
