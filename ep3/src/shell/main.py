from prompt import *
from simulator import filesystem, diretory, interface
import os.path
import subprocess




def main():
	prompt = Prompt()

	mounted = False

	while True:
		command = prompt.read()

		if command.is_valid():
			if command.argv[0] == 'sai':
				break

			if command.argv[0] == 'mount':
				if not mounted and not os.path.isfile(command.argv[1]):
					#Monta o sistema de arquivos
					create_filesystem(command.argv[1])

				#Monta os sistema de arquivo
				interface.mount(command.argv[1])
				mounted = True

			#O disco esta montado, pode rodar os comandos
			if mounted:
				comm = command.argv[0]
				param = command.argv[1]
				if command.argc == 3: param2 = command.argv[2]

				try:
					if comm == 'cp':
						interface.cp(param, param2)
					elif comm == 'mkdir':
						interface.mkdir(param)
					elif comm == 'rmdir':
						interface.rmdir(param)
					elif comm == 'cat'
						interface.cat(param)
					elif comm == 'touch':
						interface.touch(param)
					elif comm == 'rm'
						interface.rm(param)
					elif comm == 'ls':
						interface.ls(param)
					elif comm == 'find':
						interface.find(param)
					elif comm == 'df':
						interface.df()
					elif comm == 'umount':
						interface.umount
				except:
					print("Command error")
		else:
			print ("%s\n%s" % (command.get_error(), prompt.help()))


if __name__ == '__main__':
	main()
