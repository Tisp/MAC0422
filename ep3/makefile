all: ep3

ep3:
	echo '#!/bin/bash' > ep3
	echo 'export PYTHONPATH="src"' >> ep3
	echo 'python3 src/shell/main.py "$$@"' >> ep3
	chmod +x ep3

clean:
	rm -f ep3
