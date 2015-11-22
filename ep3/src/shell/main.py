from prompt import *
from simulator import interface
import os.path

def main():
	prompt = Prompt()

	while True:
		command = prompt.read()

		if command.is_valid():
			if command.argv[0] == 'sai':
				break

			comm = command.argv[0]
			if comm != 'df' and comm != 'umount': param = command.argv[1]
			if command.argc == 3: param2 = command.argv[2]

			try:
				ret = None
				if comm == 'mount':
					ret = interface.mount(param)
				elif comm == 'cp':
					ret = interface.cp(param, param2)
				elif comm == 'mkdir':
					ret = interface.mkdir(param)
				elif comm == 'rmdir':
					ret = interface.rmdir(param)
				elif comm == 'cat':
					ret = interface.cat(param)
				elif comm == 'touch':
					ret = interface.touch(param)
				elif comm == 'rm':
					ret = interface.rm(param)
				elif comm == 'ls':
					ret = interface.ls(param)
				elif comm == 'find':
					ret = interface.find(param, param2)
				elif comm == 'df':
					ret = interface.df()
				elif comm == 'umount':
					ret = interface.umount()

				if ret is not None: print(ret)
				interface.flush()

			except Exception as ex:
				print("Erro: " + str(ex))
		else:
			print ("%s\n%s" % (command.get_error(), prompt.help()))
			#interface.test()


if __name__ == '__main__':
	main()
