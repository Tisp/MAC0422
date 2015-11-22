class Command:

    argc = 0
    argv = []
    _valid_commands = ['cp',
                       'mkdir',
                       'rmdir',
                       'cat',
                       'touch',
                       'rm',
                       'ls',
                       'find',
                       'df',
                       'umount',
                       'sai']
    _error = ''

    def __init__(self, s):
        self.argv = s.strip().split(' ')
        self.argc = len(self.argv)

    def is_valid(self):
        #verifica se o comando dando e valido
        if self.argv[0] not in self._valid_commands:
            self._error = "Comando invalido"
            return False

        #verifica se os comandos possuem argumentos
        if self.argv[0] != 'sai' and self.argc == 1:
            self._error = 'Numero de argumentos invalido'
            return False
        return True

    def __str__(self):
        return str(self.argv)

    def get_error(self):
        return self._error

class Prompt:

    def read(self):
        command = input('[ep3]: ')
        return Command(command)

    def help(self):
        h =  "Ultilize:\n"
        h += "* mount <arquivo>\n"
        h += "* cp <origem> <destino>\n"
        h += "* mkdir <diretorio>\n"
        h += "* rmdir <diretorio>\n"
        h += "* cat <arquivo>\n"
        h += "* touch <arquivo>\n"
        h += "* rm <arquivo>\n"
        h += "* ls <diretorio>\n"
        h += "* find [<arquivo>|<arquivo>]\n"
        h += "* df\n"
        h += "* umount\n"
        h += "* sai: finaliza o simulador e volta para o shell do sistema operacional."
        return h
