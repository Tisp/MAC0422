class Command:

    argc = 0
    argv = []
    _valid_commands = ['carrega', 'espaco', 'substitui', 'executa', 'sai']
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
        command = input('[ep2]: ')
        return Command(command)

    def help(self):
        h =  "Ultilize:\n"
        h += "* carrega <arquivo>\n"
        h += "* espaco <num> [1-3]\n"
        h += "* substitui <num> [1-4]\n"
        h += "* executa <intervalo>\n"
        h += "* sai: finaliza o simulador e volta para o shell do sistema operacional."
        return h
